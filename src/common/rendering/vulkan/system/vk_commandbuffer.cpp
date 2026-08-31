/*
**  Vulkan backend
**  Copyright (c) 2016-2020 Magnus Norddahl
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
*/

#include "vk_commandbuffer.h"
#include "vk_renderdevice.h"
#include "zvulkan/vulkanswapchain.h"
#include "zvulkan/vulkanbuilders.h"
#include "vulkan/textures/vk_framebuffer.h"
#include "vulkan/renderer/vk_renderstate.h"
#include "vulkan/renderer/vk_postprocess.h"
#include "hw_clock.h"
#include "v_video.h"

extern int rendered_commandbuffers;
int current_rendered_commandbuffers;

extern bool gpuStatActive;
extern bool keepGpuStatActive;
extern FString gpuStatOutput;

VkCommandBufferManager::VkCommandBufferManager(VulkanRenderDevice* fb) : fb(fb)
{
	mNumFrameSlots = fb->GetFramesInFlight();

	for (int s = 0; s < mNumFrameSlots; s++)
	{
		FrameSlot& slot = mFrameSlots[s];

		slot.commandPool = CommandPoolBuilder()
			.QueueFamily(fb->device->GraphicsFamily)
			.DebugName("mCommandPool")
			.Create(fb->device.get());

		for (auto& semaphore : slot.submitSemaphores)
			semaphore.reset(new VulkanSemaphore(fb->device.get()));

		for (auto& fence : slot.submitFences)
			fence.reset(new VulkanFence(fb->device.get()));

		for (int i = 0; i < maxConcurrentSubmitCount; i++)
			slot.waitFences[i] = slot.submitFences[i]->fence;
	}

	if (fb->device->GraphicsTimeQueries)
	{
		mTimestampQueryPool = QueryPoolBuilder()
			.QueryType(VK_QUERY_TYPE_TIMESTAMP, MaxTimestampQueries)
			.Create(fb->device.get());

		GetDrawCommands()->resetQueryPool(mTimestampQueryPool.get(), 0, MaxTimestampQueries);
	}
}

VkCommandBufferManager::~VkCommandBufferManager()
{
	// Free command buffers before their pools are destroyed
	mTransferCommands.reset();
	mDrawCommands.reset();
	TransferDeleteList.reset();
	DrawDeleteList.reset();
	for (int s = 0; s < mNumFrameSlots; s++)
	{
		mFrameSlots[s].pendingTransferDelete.reset();
		mFrameSlots[s].pendingDrawDelete.reset();
	}
}

VulkanCommandBuffer* VkCommandBufferManager::GetTransferCommands()
{
	if (!mTransferCommands)
	{
		mTransferCommands = mFrameSlots[mFrameSlot].commandPool->createBuffer();
		mTransferCommands->SetDebugName("VulkanRenderDevice.mTransferCommands");
		mTransferCommands->begin();
	}
	return mTransferCommands.get();
}

VulkanCommandBuffer* VkCommandBufferManager::GetDrawCommands()
{
	if (!mDrawCommands)
	{
		mDrawCommands = mFrameSlots[mFrameSlot].commandPool->createBuffer();
		mDrawCommands->SetDebugName("VulkanRenderDevice.mDrawCommands");
		mDrawCommands->begin();
	}
	return mDrawCommands.get();
}

void VkCommandBufferManager::BeginFrame()
{
	if (mNextTimestampQuery > 0)
	{
		GetDrawCommands()->resetQueryPool(mTimestampQueryPool.get(), 0, mNextTimestampQuery);
		mNextTimestampQuery = 0;
	}
}

void VkCommandBufferManager::FlushCommands(VulkanCommandBuffer** commands, size_t count, bool finish, bool lastsubmit)
{
	FrameSlot& slot = mFrameSlots[mFrameSlot];
	int currentIndex = slot.numSubmits % maxConcurrentSubmitCount;

	if (slot.numSubmits >= maxConcurrentSubmitCount)
	{
		vkWaitForFences(fb->device->device, 1, &slot.submitFences[currentIndex]->fence, VK_TRUE, std::numeric_limits<uint64_t>::max());
		vkResetFences(fb->device->device, 1, &slot.submitFences[currentIndex]->fence);
	}

	QueueSubmit submit;

	for (size_t i = 0; i < count; i++)
		submit.AddCommandBuffer(commands[i]);

	if (slot.numSubmits > 0)
		submit.AddWait(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, slot.submitSemaphores[(slot.numSubmits - 1) % maxConcurrentSubmitCount].get());

	if (finish && fb->GetFramebufferManager()->PresentImageIndex != -1)
	{
		submit.AddWait(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, fb->GetFramebufferManager()->GetSwapChainImageAvailableSemaphore());
		submit.AddSignal(fb->GetFramebufferManager()->GetRenderFinishedSemaphore());
	}

	if (!lastsubmit)
		submit.AddSignal(slot.submitSemaphores[currentIndex].get());

	submit.Execute(fb->device.get(), fb->device->GraphicsQueue, slot.submitFences[currentIndex].get());
	slot.numSubmits++;
}

void VkCommandBufferManager::FlushCommands(bool finish, bool lastsubmit, bool uploadOnly)
{
	if (!uploadOnly)
		fb->GetRenderState()->EndRenderPass();

	if ((!uploadOnly && mDrawCommands) || mTransferCommands)
	{
		VulkanCommandBuffer* commands[2];
		size_t count = 0;

		if (mTransferCommands)
		{
#ifdef __MOBILE__
			// Mali overlaps transfer and draw stages; without this the streamed
			// vertex/index/uniform copies race the draws that read them (2D flicker)
			PipelineBarrier()
				.AddMemory(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT | VK_ACCESS_INDEX_READ_BIT | VK_ACCESS_UNIFORM_READ_BIT | VK_ACCESS_SHADER_READ_BIT)
				.Execute(mTransferCommands.get(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT | VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
#endif
			mTransferCommands->end();
			commands[count++] = mTransferCommands.get();
			TransferDeleteList->Add(std::move(mTransferCommands));
		}

		if (!uploadOnly && mDrawCommands)
		{
			mDrawCommands->end();
			commands[count++] = mDrawCommands.get();
			DrawDeleteList->Add(std::move(mDrawCommands));
		}

		FlushCommands(commands, count, finish, lastsubmit);

		current_rendered_commandbuffers += (int)count;
	}
}

void VkCommandBufferManager::WaitForCommands(bool finish, bool uploadOnly)
{
	if (finish)
	{
		Finish.Reset();
		Finish.Clock();

		fb->GetFramebufferManager()->AcquireImage();
	}

	FlushCommands(finish, true, uploadOnly);

	if (finish)
	{
		if (!fb->GetVSync())
			fb->FPSLimit();
		fb->GetFramebufferManager()->QueuePresent();
	}

	if (finish && !uploadOnly && mNumFrameSlots > 1)
	{
		// Frame pipelining: park this frame's objects in its slot and only wait for the
		// oldest in-flight frame, so the CPU records the next frame while the GPU renders
		FrameSlot& current = mFrameSlots[mFrameSlot];
		current.pendingTransferDelete = std::move(TransferDeleteList);
		current.pendingDrawDelete = std::move(DrawDeleteList);

		mFrameSlot = (mFrameSlot + 1) % mNumFrameSlots;

		WaitForSlot(mFrameSlots[mFrameSlot]);
		ResetSlotPool(mFrameSlots[mFrameSlot]); // its command buffers died with the pending lists
		TransferDeleteList = std::make_unique<DeleteList>();
		DrawDeleteList = std::make_unique<DeleteList>();
	}
	else
	{
		WaitForSlot(mFrameSlots[mFrameSlot]);
		if (!uploadOnly && mNumFrameSlots > 1)
			DrainFrameSlots(); // mid-frame full flush: CPU is about to touch shared data
		DeleteFrameObjects(uploadOnly);
		if (finish)
			ResetSlotPool(mFrameSlots[mFrameSlot]); // frame boundary: everything freed above
	}

	if (finish)
	{
		Finish.Unclock();
		rendered_commandbuffers = current_rendered_commandbuffers;
		current_rendered_commandbuffers = 0;
	}
}

void VkCommandBufferManager::WaitForSlot(FrameSlot& slot)
{
	int numWaitFences = min(slot.numSubmits, (int)maxConcurrentSubmitCount);

	if (numWaitFences > 0)
	{
		vkWaitForFences(fb->device->device, numWaitFences, slot.waitFences, VK_TRUE, std::numeric_limits<uint64_t>::max());
		vkResetFences(fb->device->device, numWaitFences, slot.waitFences);
	}

	slot.numSubmits = 0;
	slot.pendingTransferDelete.reset();
	slot.pendingDrawDelete.reset();
}

// Only call when every command buffer from this slot's pool has been freed
void VkCommandBufferManager::ResetSlotPool(FrameSlot& slot)
{
	// Return command memory to the driver; Mali keeps growing the pool otherwise
	vkResetCommandPool(fb->device->device, slot.commandPool->pool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
}

void VkCommandBufferManager::DrainFrameSlots()
{
	for (int s = 0; s < mNumFrameSlots; s++)
		WaitForSlot(mFrameSlots[s]);
}

void VkCommandBufferManager::DeleteFrameObjects(bool uploadOnly)
{
	TransferDeleteList = std::make_unique<DeleteList>();
	if (!uploadOnly)
		DrawDeleteList = std::make_unique<DeleteList>();
}

void VkCommandBufferManager::PushGroup(const FString& name)
{
	if (!gpuStatActive)
		return;

	if (mNextTimestampQuery < MaxTimestampQueries && fb->device->GraphicsTimeQueries)
	{
		TimestampQuery q;
		q.name = name;
		q.startIndex = mNextTimestampQuery++;
		q.endIndex = 0;
		GetDrawCommands()->writeTimestamp(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, mTimestampQueryPool.get(), q.startIndex);
		mGroupStack.push_back(timeElapsedQueries.size());
		timeElapsedQueries.push_back(q);
	}
}

void VkCommandBufferManager::PopGroup()
{
	if (!gpuStatActive || mGroupStack.empty())
		return;

	TimestampQuery& q = timeElapsedQueries[mGroupStack.back()];
	mGroupStack.pop_back();

	if (mNextTimestampQuery < MaxTimestampQueries && fb->device->GraphicsTimeQueries)
	{
		q.endIndex = mNextTimestampQuery++;
		GetDrawCommands()->writeTimestamp(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, mTimestampQueryPool.get(), q.endIndex);
	}
}

void VkCommandBufferManager::UpdateGpuStats()
{
	uint64_t timestamps[MaxTimestampQueries];
	if (mNextTimestampQuery > 0)
		mTimestampQueryPool->getResults(0, mNextTimestampQuery, sizeof(uint64_t) * mNextTimestampQuery, timestamps, sizeof(uint64_t), VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT);

	double timestampPeriod = fb->device->PhysicalDevice.Properties.Properties.limits.timestampPeriod;

	gpuStatOutput = "";
	for (auto& q : timeElapsedQueries)
	{
		if (q.endIndex <= q.startIndex)
			continue;

		int64_t timeElapsed = max(static_cast<int64_t>(timestamps[q.endIndex] - timestamps[q.startIndex]), (int64_t)0);
		double timeNS = timeElapsed * timestampPeriod;

		FString out;
		out.Format("%s=%04.2f ms\n", q.name.GetChars(), timeNS / 1000000.0f);
		gpuStatOutput += out;
	}
	timeElapsedQueries.clear();
	mGroupStack.clear();

	gpuStatActive = keepGpuStatActive;
	keepGpuStatActive = false;
}

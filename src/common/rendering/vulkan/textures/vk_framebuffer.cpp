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

#include <zvulkan/vulkanobjects.h>
#include <zvulkan/vulkandevice.h>
#include <zvulkan/vulkanbuilders.h>
#include <zvulkan/vulkanswapchain.h>
#include "vulkan/system/vk_renderdevice.h"
#include "vulkan/system/vk_commandbuffer.h"
#include "vulkan/renderer/vk_postprocess.h"
#include "vk_framebuffer.h"

CVAR(Bool, vk_hdr, false, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
CVAR(Bool, vk_exclusivefullscreen, false, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
#ifdef __MOBILE__
// MAILBOX present: uncapped and avoids FIFO's driver blocking (big win on Adreno), paces worse on Mali
CVAR(Bool, vk_mailbox, false, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
extern bool zvulkan_prefer_mailbox;
#endif

VkFramebufferManager::VkFramebufferManager(VulkanRenderDevice* fb) : fb(fb)
{
	SwapChain = VulkanSwapChainBuilder()
		.Create(fb->device.get());

	for (int i = 0; i < fb->GetFramesInFlight(); i++)
	{
		SwapChainImageAvailableSemaphores.push_back(SemaphoreBuilder()
			.DebugName("SwapChainImageAvailableSemaphore")
			.Create(fb->device.get()));

		RenderFinishedSemaphores.push_back(SemaphoreBuilder()
			.DebugName("RenderFinishedSemaphore")
			.Create(fb->device.get()));
	}
}

VkFramebufferManager::~VkFramebufferManager()
{
}

VulkanSemaphore* VkFramebufferManager::GetSwapChainImageAvailableSemaphore()
{
	return SwapChainImageAvailableSemaphores[fb->GetCommands()->GetFrameSlot()].get();
}

VulkanSemaphore* VkFramebufferManager::GetRenderFinishedSemaphore()
{
	return RenderFinishedSemaphores[fb->GetCommands()->GetFrameSlot()].get();
}

void VkFramebufferManager::AcquireImage()
{
	bool exclusiveFullscreen = fb->IsFullscreen() && vk_exclusivefullscreen;
#ifdef __MOBILE__
	static bool currentMailbox = false;
	bool mailboxChanged = currentMailbox != vk_mailbox;
	currentMailbox = zvulkan_prefer_mailbox = vk_mailbox;
#else
	const bool mailboxChanged = false;
#endif
	if (SwapChain->Lost() || mailboxChanged || fb->GetClientWidth() != CurrentWidth || fb->GetClientHeight() != CurrentHeight || fb->GetVSync() != CurrentVSync || CurrentHdr != vk_hdr || CurrentExclusiveFullscreen != exclusiveFullscreen)
	{
		// In-flight frames may still reference the old swapchain framebuffers
		fb->GetCommands()->DrainFrameSlots();

		Framebuffers.clear();

		CurrentWidth = fb->GetClientWidth();
		CurrentHeight = fb->GetClientHeight();
		CurrentVSync = fb->GetVSync();
		CurrentHdr = vk_hdr;
		CurrentExclusiveFullscreen = exclusiveFullscreen;

		SwapChain->Create(CurrentWidth, CurrentHeight, CurrentVSync ? 2 : 3, CurrentVSync, CurrentHdr, CurrentExclusiveFullscreen);
	}

	PresentImageIndex = SwapChain->AcquireImage(GetSwapChainImageAvailableSemaphore());
	if (PresentImageIndex != -1)
	{
		fb->GetPostprocess()->DrawPresentTexture(fb->mOutputLetterbox, true, false);
	}
}

void VkFramebufferManager::QueuePresent()
{
	if (PresentImageIndex != -1)
		SwapChain->QueuePresent(PresentImageIndex, GetRenderFinishedSemaphore());
}

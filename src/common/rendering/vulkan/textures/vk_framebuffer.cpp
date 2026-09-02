/*
** vk_framebuffer.cpp
**
** Vulkan backend
**
**---------------------------------------------------------------------------
**
** Copyright 2017-2025 GZDoom Maintainers and Contributors
** Copyright 2025-2026 UZDoom Maintainers and Contributors
**
** SPDX-License-Identifier: GPL-3.0-or-later
**
**---------------------------------------------------------------------------
**
** Copyright 2016-2020 Magnus Norddahl
**
** SPDX-License-Identifier: Zlib
**
**---------------------------------------------------------------------------
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
	}
}

VkFramebufferManager::~VkFramebufferManager()
{
}

VulkanSemaphore* VkFramebufferManager::GetSwapChainImageAvailableSemaphore()
{
	return SwapChainImageAvailableSemaphores[fb->GetCommands()->GetFrameSlot()].get();
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

		RenderFinishedSemaphores.clear();
		for (int i = 0; i < SwapChain->ImageCount(); i++)
		{
			RenderFinishedSemaphores.push_back(SemaphoreBuilder()
				.DebugName("RenderFinishedSemaphore")
				.Create(fb->device.get()));
		}
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
		SwapChain->QueuePresent(PresentImageIndex, RenderFinishedSemaphores[PresentImageIndex].get());
}

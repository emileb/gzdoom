/*
** gl_framebuffer.cpp
** Implementation of the non-hardware specific parts of the
** OpenGL frame buffer
**
**---------------------------------------------------------------------------
** Copyright 2010-2020 Christoph Oelckers
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**---------------------------------------------------------------------------
**
*/ 

#include "v_video.h"
#include "m_png.h"
#include "templates.h"
#include "i_time.h"
#include "hw_clock.h"
#include "hw_vrmodes.h"
#include "hw_skydome.h"
#include "hw_viewpointbuffer.h"
#include "hw_lightbuffer.h"
#include "r_videoscale.h"
#include "v_draw.h"
#include "printf.h"
#include "flatvertices.h"
#include "hw_cvars.h"

#include "gl_sysfb.h"

#include "gles_headers.h"
#include "gles_framebuffer.h"
#include "gles_renderstate.h"
#include "gles_buffers.h"

void Draw2D(F2DDrawer *drawer, FRenderState &state);

extern bool vid_hdr_active;

namespace OpenGLESRenderer
{

//==========================================================================
//
//
//
//==========================================================================

OpenGLESFrameBuffer::OpenGLESFrameBuffer(void *hMonitor, bool fullscreen) : 
	Super(hMonitor, fullscreen) 
{
	// Make sure all global variables tracking OpenGL context state are reset..
	gles_RenderState.Reset();
}

OpenGLESFrameBuffer::~OpenGLESFrameBuffer()
{

}

//==========================================================================
//
// Initializes the GL renderer
//
//==========================================================================

void OpenGLESFrameBuffer::InitializeState()
{
	glDepthFunc(GL_LESS);

	glEnable(GL_DITHER);
	glDisable(GL_CULL_FACE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetViewportRects(nullptr);

	mVertexData = new FFlatVertexBuffer(GetWidth(), GetHeight());
	mSkyData = new FSkyVertexBuffer;
	mViewpoints = new HWViewpointBuffer;
	mLights = new FLightBuffer();
}

//==========================================================================
//
// Updates the screen
//
//==========================================================================

void OpenGLESFrameBuffer::Update()
{

	Swap();
	Super::Update();
}

void OpenGLESFrameBuffer::CopyScreenToBuffer(int width, int height, uint8_t* scr)
{

}

//===========================================================================
//
// Camera texture rendering
//
//===========================================================================

void OpenGLESFrameBuffer::RenderTextureView(FCanvasTexture* tex, std::function<void(IntRect &)> renderFunc)
{

}

//===========================================================================
//
// 
//
//===========================================================================

const char* OpenGLESFrameBuffer::DeviceName() const 
{
	return "GLES2";
}

//==========================================================================
//
// Swap the buffers
//
//==========================================================================


void OpenGLESFrameBuffer::Swap()
{

	SwapBuffers();

	gles_RenderState.ClearLastMaterial();

}

//==========================================================================
//
// Enable/disable vertical sync
//
//==========================================================================

void OpenGLESFrameBuffer::SetVSync(bool vsync)
{

}

//===========================================================================
//
//
//===========================================================================

void OpenGLESFrameBuffer::SetTextureFilterMode()
{

}

IHardwareTexture *OpenGLESFrameBuffer::CreateHardwareTexture(int numchannels) 
{ 
	//return new FHardwareTexture(numchannels);
}

void OpenGLESFrameBuffer::PrecacheMaterial(FMaterial *mat, int translation)
{

}

IVertexBuffer *OpenGLESFrameBuffer::CreateVertexBuffer()
{ 
	return new GLESVertexBuffer;
}

IIndexBuffer *OpenGLESFrameBuffer::CreateIndexBuffer()
{ 
	return new GLESIndexBuffer;
}

IDataBuffer *OpenGLESFrameBuffer::CreateDataBuffer(int bindingpoint, bool ssbo, bool needsresize)
{
	return new GLESDataBuffer(bindingpoint, ssbo);
}

void OpenGLESFrameBuffer::BlurScene(float amount)
{

}

void OpenGLESFrameBuffer::SetViewportRects(IntRect *bounds)
{
	Super::SetViewportRects(bounds);
}

void OpenGLESFrameBuffer::UpdatePalette()
{

}

FRenderState* OpenGLESFrameBuffer::RenderState()
{
	return &gles_RenderState;
}

void OpenGLESFrameBuffer::AmbientOccludeScene(float m5)
{

}

void OpenGLESFrameBuffer::FirstEye()
{

}

void OpenGLESFrameBuffer::NextEye(int eyecount)
{

}

void OpenGLESFrameBuffer::SetSceneRenderTarget(bool useSSAO)
{

}

void OpenGLESFrameBuffer::UpdateShadowMap()
{

}

void OpenGLESFrameBuffer::WaitForCommands(bool finish)
{

}

void OpenGLESFrameBuffer::SetSaveBuffers(bool yes)
{

}

//===========================================================================
//
// 
//
//===========================================================================

void OpenGLESFrameBuffer::BeginFrame()
{
	SetViewportRects(nullptr);

}

//===========================================================================
// 
//	Takes a screenshot
//
//===========================================================================

TArray<uint8_t> OpenGLESFrameBuffer::GetScreenshotBuffer(int &pitch, ESSType &color_type, float &gamma)
{

}

//===========================================================================
// 
// 2D drawing
//
//===========================================================================

void OpenGLESFrameBuffer::Draw2D()
{

	::Draw2D(twod, gles_RenderState);

}

void OpenGLESFrameBuffer::PostProcessScene(bool swscene, int fixedcm, float flash, const std::function<void()> &afterBloomDrawEndScene2D)
{

}

//==========================================================================
//
// OpenGLESFrameBuffer :: WipeStartScreen
//
// Called before the current screen has started rendering. This needs to
// save what was drawn the previous frame so that it can be animated into
// what gets drawn this frame.
//
//==========================================================================

FTexture *OpenGLESFrameBuffer::WipeStartScreen()
{

	return 0;
}

//==========================================================================
//
// OpenGLESFrameBuffer :: WipeEndScreen
//
// The screen we want to animate to has just been drawn.
//
//==========================================================================

FTexture *OpenGLESFrameBuffer::WipeEndScreen()
{

	return 0;
}

}

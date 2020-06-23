// 
//---------------------------------------------------------------------------
//
// Copyright(C) 2009-2016 Christoph Oelckers
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/
//
//--------------------------------------------------------------------------
//
/*
** gl_renderstate.cpp
** Render state maintenance
**
*/

#include "gles_headers.h"
#include "templates.h"
#include "hw_cvars.h"
#include "flatvertices.h"
#include "hw_lightbuffer.h"
#include "gles_buffers.h"
#include "gles_renderstate.h"
#include "hw_clock.h"
#include "hwrenderer/data/hw_viewpointbuffer.h"
#include "v_video.h"

namespace OpenGLESRenderer
{

FGLESRenderState gles_RenderState;

static VSMatrix identityMatrix(1);

static void matrixToGL(const VSMatrix &mat, int loc)
{
	glUniformMatrix4fv(loc, 1, false, (float*)&mat);
}

//==========================================================================
//
// This only gets called once upon setup.
// With OpenGL the state is persistent and cannot be cleared, once set up.
//
//==========================================================================

void FGLESRenderState::Reset()
{
	FRenderState::Reset();
	mVertexBuffer = mCurrentVertexBuffer = nullptr;
	mGlossiness = 0.0f;
	mSpecularLevel = 0.0f;
	mShaderTimer = 0.0f;

	stRenderStyle = DefaultRenderStyle();
	stSrcBlend = stDstBlend = -1;
	stBlendEquation = -1;
	stAlphaTest = 0;
	mLastDepthClamp = true;

	mEffectState = 0;
	activeShader = nullptr;

	mCurrentVertexBuffer = nullptr;
	mCurrentVertexOffsets[0] = mVertexOffsets[0] = 0;
	mCurrentIndexBuffer = nullptr;

}

//==========================================================================
//
// Apply shader settings
//
//==========================================================================

bool FGLESRenderState::ApplyShader()
{

	return true;
}


//==========================================================================
//
// Apply State
//
//==========================================================================

void FGLESRenderState::ApplyState()
{

}

void FGLESRenderState::ApplyBuffers()
{

}

void FGLESRenderState::Apply()
{
	ApplyState();
	ApplyBuffers();
	ApplyShader();
}

//===========================================================================
// 
//	Binds a texture to the renderer
//
//===========================================================================

void FGLESRenderState::ApplyMaterial(FMaterial *mat, int clampmode, int translation, int overrideshader)
{

}

//==========================================================================
//
// Apply blend mode from RenderStyle
//
//==========================================================================

void FGLESRenderState::ApplyBlendMode()
{
	static int blendstyles[] = { GL_ZERO, GL_ONE, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA };
	static int renderops[] = { 0, GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1 };

	int srcblend = blendstyles[mRenderStyle.SrcAlpha%STYLEALPHA_MAX];
	int dstblend = blendstyles[mRenderStyle.DestAlpha%STYLEALPHA_MAX];
	int blendequation = renderops[mRenderStyle.BlendOp & 15];

	if (blendequation == -1)	// This was a fuzz style.
	{
		srcblend = GL_DST_COLOR;
		dstblend = GL_ONE_MINUS_SRC_ALPHA;
		blendequation = GL_FUNC_ADD;
	}

	// Checks must be disabled until all draw code has been converted.
	//if (srcblend != stSrcBlend || dstblend != stDstBlend)
	{
		stSrcBlend = srcblend;
		stDstBlend = dstblend;
		glBlendFunc(srcblend, dstblend);
	}
	//if (blendequation != stBlendEquation)
	{
		stBlendEquation = blendequation;
		glBlendEquation(blendequation);
	}

}

//==========================================================================
//
// API dependent draw calls
//
//==========================================================================

static int dt2gl[] = { GL_POINTS, GL_LINES, GL_TRIANGLES, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP };

void FGLESRenderState::Draw(int dt, int index, int count, bool apply)
{
	if (apply)
	{
		Apply();
	}
	drawcalls.Clock();
	glDrawArrays(dt2gl[dt], index, count);
	drawcalls.Unclock();
}

void FGLESRenderState::DrawIndexed(int dt, int index, int count, bool apply)
{
	if (apply)
	{
		Apply();
	}
	drawcalls.Clock();
	glDrawElements(dt2gl[dt], count, GL_UNSIGNED_INT, (void*)(intptr_t)(index * sizeof(uint32_t)));
	drawcalls.Unclock();
}

void FGLESRenderState::SetDepthMask(bool on)
{
	glDepthMask(on);
}

void FGLESRenderState::SetDepthFunc(int func)
{
	static int df2gl[] = { GL_LESS, GL_LEQUAL, GL_ALWAYS };
	glDepthFunc(df2gl[func]);
}

void FGLESRenderState::SetDepthRange(float min, float max)
{
	glDepthRangef(min, max);
}

void FGLESRenderState::SetColorMask(bool r, bool g, bool b, bool a)
{
	glColorMask(r, g, b, a);
}

void FGLESRenderState::SetStencil(int offs, int op, int flags = -1)
{
	static int op2gl[] = { GL_KEEP, GL_INCR, GL_DECR };

	glStencilFunc(GL_EQUAL, screen->stencilValue + offs, ~0);		// draw sky into stencil
	glStencilOp(GL_KEEP, GL_KEEP, op2gl[op]);		// this stage doesn't modify the stencil

	if (flags != -1)
	{
		bool cmon = !(flags & SF_ColorMaskOff);
		glColorMask(cmon, cmon, cmon, cmon);						// don't write to the graphics buffer
		glDepthMask(!(flags & SF_DepthMaskOff));
	}
}

void FGLESRenderState::ToggleState(int state, bool on)
{
	if (on)
	{
		glEnable(state);
	}
	else
	{
		glDisable(state);
	}
}

void FGLESRenderState::SetCulling(int mode)
{
	if (mode != Cull_None)
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(mode == Cull_CCW ? GL_CCW : GL_CW);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void FGLESRenderState::EnableClipDistance(int num, bool state)
{

}

void FGLESRenderState::Clear(int targets)
{
	// This always clears to default values.
	int gltarget = 0;
	if (targets & CT_Depth)
	{
		gltarget |= GL_DEPTH_BUFFER_BIT;
		glClearDepthf(1);
	}
	if (targets & CT_Stencil)
	{
		gltarget |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(0);
	}
	if (targets & CT_Color)
	{
		gltarget |= GL_COLOR_BUFFER_BIT;
		glClearColor(screen->mSceneClearColor[0], screen->mSceneClearColor[1], screen->mSceneClearColor[2], screen->mSceneClearColor[3]);
	}
	glClear(gltarget);
}

void FGLESRenderState::EnableStencil(bool on)
{
	ToggleState(GL_STENCIL_TEST, on);
}

void FGLESRenderState::SetScissor(int x, int y, int w, int h)
{
	if (w > -1)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, y, w, h);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
	}
}

void FGLESRenderState::SetViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

void FGLESRenderState::EnableDepthTest(bool on)
{
	ToggleState(GL_DEPTH_TEST, on);
}

void FGLESRenderState::EnableMultisampling(bool on)
{

}

void FGLESRenderState::EnableLineSmooth(bool on)
{

}

//==========================================================================
//
//
//
//==========================================================================
void FGLESRenderState::ClearScreen()
{

	screen->mViewpoints->Set2D(*this, SCREENWIDTH, SCREENHEIGHT);
	SetColor(0, 0, 0);
	Apply();


	glDisable(GL_DEPTH_TEST);

	glDrawArrays(GL_TRIANGLE_STRIP, FFlatVertexBuffer::FULLSCREEN_INDEX, 4);

	glEnable(GL_DEPTH_TEST);
}



//==========================================================================
//
// Below are less frequently altrered state settings which do not get
// buffered by the state object, but set directly instead.
//
//==========================================================================

bool FGLESRenderState::SetDepthClamp(bool on)
{
	mLastDepthClamp = on;
	return on;
}

}

//
//---------------------------------------------------------------------------
//
// Copyright(C) 2016 Alexey Lysiuk
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

//
// Fast approXimate Anti-Aliasing (FXAA) post-processing
//

#include "hw_fxaashader.h"

EXTERN_CVAR(Int, gl_fxaa)

void FFXAALumaShader::Bind(IRenderQueue *q)
{
	if (!mShader)
	{
		mShader.reset(screen->CreateShaderProgram());
		mShader->Compile(IShaderProgram::Vertex, "shaders/glsl/screenquad.vp", "", 330);
		mShader->Compile(IShaderProgram::Fragment, "shaders/glsl/fxaa.fp", "#define FXAA_LUMA_PASS\n", 330);
		mShader->Link("shaders/glsl/fxaa");
	}

	mShader->Bind(q);
}

static int GetMaxVersion()
{
	return screen->glslversion >= 4.f ? 400 : 330;
}

static FString GetDefines()
{
	int quality;

	switch (gl_fxaa)
	{
	default:
	case FFXAAShader::Low:     quality = 10; break;
	case FFXAAShader::Medium:  quality = 12; break;
	case FFXAAShader::High:    quality = 29; break;
	case FFXAAShader::Extreme: quality = 39; break;
	}

	const int gatherAlpha = GetMaxVersion() >= 400 ? 1 : 0;

	// TODO: enable FXAA_GATHER4_ALPHA on OpenGL earlier than 4.0
	// when GL_ARB_gpu_shader5/GL_NV_gpu_shader5 extensions are supported

	FString result;
	result.Format(
		"#define FXAA_QUALITY__PRESET %i\n"
		"#define FXAA_GATHER4_ALPHA %i\n",
		quality, gatherAlpha);

	return result;
}

void FFXAAShader::Bind(IRenderQueue *q)
{
	assert(gl_fxaa > 0 && gl_fxaa < Count);
	auto &shader = mShaders[gl_fxaa];

	if (!shader)
	{
		FString prolog = Uniforms.CreateDeclaration("Uniforms", UniformBlock::Desc()) + GetDefines();
		const int maxVersion = GetMaxVersion();

		shader.reset(screen->CreateShaderProgram());
		shader->Compile(IShaderProgram::Vertex, "shaders/glsl/screenquad.vp", "", maxVersion);
		shader->Compile(IShaderProgram::Fragment, "shaders/glsl/fxaa.fp", prolog, maxVersion);
		shader->Link("shaders/glsl/fxaa");
		shader->SetUniformBufferLocation(Uniforms.BindingPoint(), "Uniforms");
		Uniforms.Init();
	}

	shader->Bind(q);
}

// 
//---------------------------------------------------------------------------
//
// Copyright(C) 2016 Magnus Norddahl
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

#include "v_video.h"
#include "hwrenderer/utility/hw_cvars.h"
#include "hw_ambientshader.h"

void FLinearDepthShader::Bind(IRenderQueue *q)
{
	bool multisample = (gl_multisample > 1);
	if (mMultisample != multisample)
		mShader.reset();

	if (!mShader)
	{
		FString prolog = Uniforms.CreateDeclaration("Uniforms", UniformBlock::Desc());
		if (multisample) prolog += "#define MULTISAMPLE\n";

		mShader.reset(screen->CreateShaderProgram());
		mShader->Compile(IShaderProgram::Vertex, "shaders/glsl/screenquad.vp", "", 330);
		mShader->Compile(IShaderProgram::Fragment, "shaders/glsl/lineardepth.fp", prolog, 330);
		mShader->Link("shaders/glsl/lineardepth");
		mShader->SetUniformBufferLocation(Uniforms.BindingPoint(), "Uniforms");
		Uniforms.Init();
		mMultisample = multisample;
	}
	mShader->Bind(q);
}

void FSSAOShader::Bind(IRenderQueue *q)
{
	bool multisample = (gl_multisample > 1);
	if (mCurrentQuality != gl_ssao || mMultisample != multisample)
		mShader.reset();

	if (!mShader)
	{
		FString prolog = Uniforms.CreateDeclaration("Uniforms", UniformBlock::Desc());
		prolog += GetDefines(gl_ssao, multisample);

		mShader.reset(screen->CreateShaderProgram());
		mShader->Compile(IShaderProgram::Vertex, "shaders/glsl/screenquad.vp", "", 330);
		mShader->Compile(IShaderProgram::Fragment, "shaders/glsl/ssao.fp", prolog, 330);
		mShader->Link("shaders/glsl/ssao");
		mShader->SetUniformBufferLocation(Uniforms.BindingPoint(), "Uniforms");
		Uniforms.Init();
		mMultisample = multisample;
	}
	mShader->Bind(q);
}

FString FSSAOShader::GetDefines(int mode, bool multisample)
{
	// Must match quality values in FGLRenderBuffers::CreateAmbientOcclusion
	int numDirections, numSteps;
	switch (gl_ssao)
	{
	default:
	case LowQuality:    numDirections = 2; numSteps = 4; break;
	case MediumQuality: numDirections = 4; numSteps = 4; break;
	case HighQuality:   numDirections = 8; numSteps = 4; break;
	}

	FString defines;
	defines.Format(R"(
		#define USE_RANDOM_TEXTURE
		#define RANDOM_TEXTURE_WIDTH 4.0
		#define NUM_DIRECTIONS %d.0
		#define NUM_STEPS %d.0
	)", numDirections, numSteps);

	if (multisample)
		defines += "\n#define MULTISAMPLE\n";
	return defines;
}

void FDepthBlurShader::Bind(IRenderQueue *q, bool vertical)
{
	auto &shader = mShader[vertical];
	if (!shader)
	{
		FString prolog = Uniforms[vertical].CreateDeclaration("Uniforms", UniformBlock::Desc());
		if (vertical)
			prolog += "#define BLUR_VERTICAL\n";
		else
			prolog += "#define BLUR_HORIZONTAL\n";

		shader.reset(screen->CreateShaderProgram());
		shader->Compile(IShaderProgram::Vertex, "shaders/glsl/screenquad.vp", "", 330);
		shader->Compile(IShaderProgram::Fragment, "shaders/glsl/depthblur.fp", prolog, 330);
		shader->Link("shaders/glsl/depthblur");
		shader->SetUniformBufferLocation(Uniforms[vertical].BindingPoint(), "Uniforms");
		Uniforms[vertical].Init();
	}
	shader->Bind(q);
}

void FSSAOCombineShader::Bind(IRenderQueue *q)
{
	bool multisample = (gl_multisample > 1);
	if (mMultisample != multisample)
		mShader.reset();

	if (!mShader)
	{
		FString prolog = Uniforms.CreateDeclaration("Uniforms", UniformBlock::Desc());
		if (multisample)
			prolog += "#define MULTISAMPLE\n";

		mShader.reset(screen->CreateShaderProgram());
		mShader->Compile(IShaderProgram::Vertex, "shaders/glsl/screenquad.vp", "", 330);
		mShader->Compile(IShaderProgram::Fragment, "shaders/glsl/ssaocombine.fp", prolog, 330);
		mShader->Link("shaders/glsl/ssaocombine");
		mShader->SetUniformBufferLocation(Uniforms.BindingPoint(), "Uniforms");
		Uniforms.Init();
		mMultisample = multisample;
	}
	mShader->Bind(q);
}

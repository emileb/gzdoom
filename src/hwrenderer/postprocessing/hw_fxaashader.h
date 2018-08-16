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

#ifndef __GL_FXAASHADER_H__
#define __GL_FXAASHADER_H__

#include "hwrenderer/postprocessing/hw_shaderprogram.h"
#include "hwrenderer/postprocessing/hw_postprocess_cvars.h"

class FFXAALumaShader
{
public:
	void Bind(IRenderQueue *q);

private:
	std::unique_ptr<IShaderProgram> mShader;
};


class FFXAAShader : public IFXAAShader
{
public:
	void Bind(IRenderQueue *q);

	struct UniformBlock
	{
		FVector2 ReciprocalResolution;
		float Padding0, Padding1;

		static std::vector<UniformFieldDesc> Desc()
		{
			return
			{
				{ "ReciprocalResolution", UniformType::Vec2, offsetof(UniformBlock, ReciprocalResolution) },
				{ "Padding0", UniformType::Float, offsetof(UniformBlock, Padding0) },
				{ "Padding1", UniformType::Float, offsetof(UniformBlock, Padding1) }
			};
		}
	};

	ShaderUniforms<UniformBlock, POSTPROCESS_BINDINGPOINT> Uniforms;

private:
	std::unique_ptr<IShaderProgram> mShaders[Count];
};

#endif // __GL_FXAASHADER_H__

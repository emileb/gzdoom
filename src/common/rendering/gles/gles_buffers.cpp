/*
** gl_buffers.cpp
** Low level vertex buffer class
**
**---------------------------------------------------------------------------
** Copyright 2018-2020 Christoph Oelckers
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
**/

#include <algorithm>
#include "gles_headers.h"
#include "gles_buffers.h"
#include "gles_renderstate.h"
#include "v_video.h"
#include "flatvertices.h"

namespace OpenGLESRenderer
{

//==========================================================================
//
// basic buffer implementation
//
//==========================================================================

static inline void InvalidateBufferState()
{
	gles_RenderState.ResetVertexBuffer();	// force rebinding of buffers on next Apply call.
}

GLESBuffer::GLESBuffer(int usetype)
	: mUseType(usetype)
{
	//glGenBuffers(1, &mBufferId);
}

GLESBuffer::~GLESBuffer()
{
	if(memory)
		free(memory);
}

void GLESBuffer::Bind()
{
	//glBindBuffer(mUseType, mBufferId);
}


void GLESBuffer::SetData(size_t size, const void *data, bool staticdata)
{
	Bind();

	if(memory)
		free(memory);

	memory = (char*)malloc(size);

	InvalidateBufferState();
}

void GLESBuffer::SetSubData(size_t offset, size_t size, const void *data)
{
	Bind();
	glBufferSubData(mUseType, offset, size, data);
}

void GLESBuffer::Map()
{
	map = (FFlatVertex*)memory;
}

void GLESBuffer::Unmap()
{

}

void *GLESBuffer::Lock(unsigned int size)
{
	// This initializes this buffer as a static object with no data.
	SetData(size, nullptr, true);
	return memory;
}

void GLESBuffer::Unlock()
{
	Bind();
	//glUnmapBuffer(mUseType);
	InvalidateBufferState();
}


void GLESBuffer::Resize(size_t newsize)
{
	assert(!nomap);	// only mappable buffers can be resized. 
	if (newsize > buffersize && !nomap)
	{

		InvalidateBufferState();
	}
}


//===========================================================================
//
// Vertex buffer implementation
//
//===========================================================================

void GLESVertexBuffer::SetFormat(int numBindingPoints, int numAttributes, size_t stride, const FVertexBufferAttribute *attrs)
{
	static int VFmtToGLFmt[] = { GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_UNSIGNED_BYTE, 0 };
	static uint8_t VFmtToSize[] = {4, 3, 2, 1, 4, 4};
	
	mStride = stride;
	mNumBindingPoints = numBindingPoints;
	
	for(int i = 0; i < numAttributes; i++)
	{
		if (attrs[i].location >= 0 && attrs[i].location < VATTR_MAX)
		{
			auto & attrinf = mAttributeInfo[attrs[i].location];
			attrinf.format = VFmtToGLFmt[attrs[i].format];
			attrinf.size = VFmtToSize[attrs[i].format];
			attrinf.offset = attrs[i].offset;
			attrinf.bindingpoint = attrs[i].binding;
		}
	}
}

void GLESVertexBuffer::Bind(int *offsets)
{
	int i = 0;

	// This is what gets called from RenderState.Apply. It shouldn't be called anywhere else if the render state is in use
	GLESBuffer::Bind();
	for(auto &attrinf : mAttributeInfo)
	{
		if (attrinf.size == 0)
		{
			glDisableVertexAttribArray(i);
		}
		else
		{
			glEnableVertexAttribArray(i);
			size_t ofs = offsets == nullptr ? attrinf.offset : attrinf.offset + mStride * offsets[attrinf.bindingpoint];
			glVertexAttribPointer(i, attrinf.size, attrinf.format, attrinf.format != GL_FLOAT, (GLsizei)mStride, (void*)(intptr_t)ofs);
		}
		i++;
	}
}

void GLESDataBuffer::BindRange(FRenderState *state, size_t start, size_t length)
{
	//glBindBufferRange(mUseType, mBindingPoint, mBufferId, start, length);
}

void GLESDataBuffer::BindBase()
{
	//glBindBufferBase(mUseType, mBindingPoint, mBufferId);
}


GLESVertexBuffer::GLESVertexBuffer() : GLESBuffer(GL_ARRAY_BUFFER) {}
GLESIndexBuffer::GLESIndexBuffer() : GLESBuffer(GL_ELEMENT_ARRAY_BUFFER) {}
GLESDataBuffer::GLESDataBuffer(int bindingpoint, bool is_ssbo) : GLESBuffer(false), mBindingPoint(bindingpoint) {}

}
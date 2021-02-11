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
#include "gl_load.h"
#include "gl_buffers.h"
#include "gl_renderstate.h"
#include "v_video.h"
#include "flatvertices.h"

namespace OpenGLRenderer
{

//==========================================================================
//
// basic buffer implementation
//
//==========================================================================

static inline void InvalidateBufferState()
{
	gl_RenderState.ResetVertexBuffer();	// force rebinding of buffers on next Apply call.
}

GLBuffer::GLBuffer(int usetype)
	: mUseType(usetype)
{
	if ((usetype != GL_ARRAY_BUFFER) && (usetype != GL_ELEMENT_ARRAY_BUFFER))
		isMemory = true;
	
	if(!isMemory)
		glGenBuffers(1, &mBufferId);
}

GLBuffer::~GLBuffer()
{
	if (mBufferId != 0)
	{
		glBindBuffer(mUseType, mBufferId);
		//glUnmapBuffer(mUseType);
		glBindBuffer(mUseType, 0);
		glDeleteBuffers(1, &mBufferId);
	}
}

void GLBuffer::Bind()
{
	if (!isMemory)
	{
		glBindBuffer(mUseType, mBufferId);
		//glBufferData(mUseType, buffersize, map, GL_STREAM_DRAW);
	}
}


void GLBuffer::SetData(size_t size, const void* data, bool staticdata)
{

	if (memory)
		delete[] memory;

	memory = (char*)(new uint64_t[size/8 + 16]);

	if (data)
		memcpy(memory, data, size);

	map = memory;

	Bind();

	if (!isMemory)
	{
	
		//glBufferData(mUseType, size, memory, staticdata ? GL_STATIC_DRAW : GL_STREAM_DRAW);
		
		if (data != nullptr)
		{
			//glBufferData(mUseType, size, data, staticdata ? GL_STATIC_DRAW : GL_STREAM_DRAW);
			glBufferData(mUseType, size, data,  GL_STREAM_DRAW);
		}
		else
		{
			
			mPersistent = screen->BuffersArePersistent() && !staticdata;
			if (mPersistent)
			{
				glBufferData(mUseType, size, nullptr, GL_STREAM_DRAW);
				//glBufferStorage(mUseType, size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
				//map = glMapBufferRange(mUseType, 0, size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
			}
			else
			{
				//glBufferData(mUseType, size, nullptr, staticdata ? GL_STATIC_DRAW : GL_STREAM_DRAW);
				glBufferData(mUseType, size, memory, GL_STREAM_DRAW);
				//map = nullptr;
			}
			if (!staticdata) nomap = false;
		}
	}

	buffersize = size;
	InvalidateBufferState();
}

void GLBuffer::SetSubData(size_t offset, size_t size, const void *data)
{
	Bind();
	
	memcpy(memory + offset, data, size);
	
	if (!isMemory)
	{
		glBufferSubData(mUseType, offset, size, data);
	}
}

void GLBuffer::Upload(size_t start, size_t size)
{
	Bind();
	//glUnmapBuffer(mUseType);
	glBufferSubData(mUseType, start, size, memory + start);
	//glBufferSubData(mUseType, 0, start, memory);
	//glBufferSubData(mUseType, 0, size, memory);
	//glBufferSubData(mUseType, 0, buffersize/4, memory);
}

void GLBuffer::Map()
{

	map = memory;
	
 	assert(map != 0);
	
	if (!isMemory)
	{
		assert(nomap == false);	// do not allow mapping of static buffers. Vulkan cannot do that so it should be blocked in OpenGL, too.
		if (!mPersistent && !nomap)
		{
			Bind();
			//map = (FFlatVertex*)glMapBufferRange(mUseType, 0, buffersize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
			InvalidateBufferState();
		}
	}
}

void GLBuffer::Unmap()
{
	if (!isMemory)
	{
		assert(nomap == false);
		if (!mPersistent && map != nullptr)
		{
			Bind();
			//glUnmapBuffer(mUseType);
			//glBufferData(mUseType, buffersize, map, GL_STREAM_DRAW);

			//glBindBuffer(mUseType, 0);
			InvalidateBufferState();
			map = nullptr;
		}
		else
		{
			Bind();
			//glUnmapBuffer(mUseType);
			glBufferData(mUseType, buffersize, map, GL_STREAM_DRAW);
		}
	}
}

void *GLBuffer::Lock(unsigned int size)
{
	// This initializes this buffer as a static object with no data.
	SetData(size, nullptr, true);

	return map;
	/*
	else
	{
		return glMapBufferRange(mUseType, 0, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	}
	*/
}

void GLBuffer::Unlock()
{

	//map = nullptr;
	
	if (!isMemory)
	{
		Bind();

		glBufferData(mUseType, buffersize, map, GL_DYNAMIC_DRAW);
		//glUnmapBuffer(mUseType);
		InvalidateBufferState();
	}
	
}

void GLBuffer::Resize(size_t newsize)
{
	assert(!nomap);	// only mappable buffers can be resized. 
	if (newsize > buffersize && !nomap)
	{
		/*
		// reallocate the buffer with twice the size
		unsigned int oldbuffer = mBufferId;

		// first unmap the old buffer
		Bind();
		glUnmapBuffer(mUseType);

		glGenBuffers(1, &mBufferId);
		SetData(newsize, nullptr, false);
		glBindBuffer(GL_COPY_READ_BUFFER, oldbuffer);

		// copy contents and delete the old buffer.
		glCopyBufferSubData(GL_COPY_READ_BUFFER, mUseType, 0, 0, buffersize);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glDeleteBuffers(1, &oldbuffer);
		buffersize = newsize;
		InvalidateBufferState();
		*/
	}
}


//===========================================================================
//
// Vertex buffer implementation
//
//===========================================================================

void GLVertexBuffer::SetFormat(int numBindingPoints, int numAttributes, size_t stride, const FVertexBufferAttribute *attrs)
{
	static int VFmtToGLFmt[] = { GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_UNSIGNED_BYTE, GL_INT };
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

void GLVertexBuffer::Bind(int *offsets)
{
	int i = 0;

	// This is what gets called from RenderState.Apply. It shouldn't be called anywhere else if the render state is in use
	GLBuffer::Bind();
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

void GLDataBuffer::BindRange(FRenderState *state, size_t start, size_t length)
{
	if (mBindingPoint == 3)// VIEWPOINT_BINDINGPOINT
	{
		static_cast<FGLRenderState*>(state)->ApplyViewport(memory + start);
	}
	else
	{
		//glBindBufferRange(mUseType, mBindingPoint, mBufferId, start, length);
	} 
}

void GLDataBuffer::BindBase()
{
	//glBindBufferBase(mUseType, mBindingPoint, mBufferId);
}


GLVertexBuffer::GLVertexBuffer() : GLBuffer(GL_ARRAY_BUFFER) {}
GLIndexBuffer::GLIndexBuffer() : GLBuffer(GL_ELEMENT_ARRAY_BUFFER) {}
GLDataBuffer::GLDataBuffer(int bindingpoint, bool is_ssbo) : GLBuffer(0), mBindingPoint(bindingpoint) {}

}
#pragma once

#include "buffers.h"

#ifdef _MSC_VER
// silence bogus warning C4250: 'GLVertexBuffer': inherits 'GLBuffer::GLBuffer::SetData' via dominance
// According to internet infos, the warning is erroneously emitted in this case.
#pragma warning(disable:4250) 
#endif

namespace OpenGLESRenderer
{

class GLESBuffer : virtual public IBuffer
{
protected:
	const int mUseType;
	unsigned int mBufferId;
	int mAllocationSize = 0;
	bool mPersistent = false;
	bool nomap = true;

	char *memory = 0;

	GLESBuffer(int usetype);
	~GLESBuffer();
	void SetData(size_t size, const void *data, bool staticdata) override;
	void SetSubData(size_t offset, size_t size, const void *data) override;
	void Map() override;
	void Unmap() override;
	void Resize(size_t newsize) override;
	void *Lock(unsigned int size) override;
	void Unlock() override;

public:
	void Bind();
};


class GLESVertexBuffer : public IVertexBuffer, public GLESBuffer
{
	// If this could use the modern (since GL 4.3) binding system, things would be simpler... :(
	struct GLVertexBufferAttribute
	{
		int bindingpoint;
		int format;
		int size;
		int offset;
	};

	int mNumBindingPoints;
	GLVertexBufferAttribute mAttributeInfo[VATTR_MAX] = {};	// Thanks to OpenGL's state system this needs to contain info about every attribute that may ever be in use throughout the entire renderer.
	size_t mStride = 0;

public:
	GLESVertexBuffer();
	void SetFormat(int numBindingPoints, int numAttributes, size_t stride, const FVertexBufferAttribute *attrs) override;
	void Bind(int *offsets);
};

class GLESIndexBuffer : public IIndexBuffer, public GLESBuffer
{
public:
	GLESIndexBuffer();
};

class GLESDataBuffer : public IDataBuffer, public GLESBuffer
{
	int mBindingPoint;
public:
	GLESDataBuffer(int bindingpoint, bool is_ssbo);
	void BindRange(FRenderState* state, size_t start, size_t length);
	void BindBase();
};

}
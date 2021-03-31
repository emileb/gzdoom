

#include "gles_system.h"
#include "tarray.h"
#include "doomtype.h"
#include "v_video.h"


#if USE_GLES2

PFNGLMAPBUFFERRANGEEXTPROC glMapBufferRange = NULL;
PFNGLUNMAPBUFFEROESPROC glUnmapBuffer = NULL;
#ifdef __ANDROID__
#include <dlfcn.h>

static void* LoadGLES2Proc(const char* name)
{
	static void *glesLib = NULL;

	if(!glesLib)
	{
		int flags = RTLD_LOCAL | RTLD_NOW;

		glesLib = dlopen("libGLESv2_CM.so", flags);
		if(!glesLib)
		{
			glesLib = dlopen("libGLESv2.so", flags);
		}
	}

	void * ret = NULL;
	ret =  dlsym(glesLib, name);

	if(!ret)
	{
		//LOGI("Failed to load: %s", name);
	}
	else
	{
		//LOGI("Loaded %s func OK", name);
	}

	return ret;
}

#elif defined _WIN32

#include <windows.h>

static HMODULE opengl32dll;
static PROC(WINAPI* getprocaddress)(LPCSTR name);

static void* LoadGLES2Proc(const char* name)
{

	HINSTANCE hGetProcIDDLL = LoadLibraryA("libGLESv2.dll");
	
	int error =	GetLastError();

	void* addr = GetProcAddress(hGetProcIDDLL, name);
	if (!addr)
	{
		//exit(1);
	}
	else
	{
		return addr;
	}
}

#endif

#endif // USE_GLES2

static TArray<FString>  m_Extensions;


static void CollectExtensions()
{
	const char* supported = (char*)glGetString(GL_EXTENSIONS);

	if (nullptr != supported)
	{
		char* extensions = new char[strlen(supported) + 1];
		strcpy(extensions, supported);

		char* extension = strtok(extensions, " ");

		while (extension)
		{
			m_Extensions.Push(FString(extension));
			extension = strtok(nullptr, " ");
		}

		delete[] extensions;
	}
}


static bool CheckExtension(const char* ext)
{
	for (unsigned int i = 0; i < m_Extensions.Size(); ++i)
	{
		if (m_Extensions[i].CompareNoCase(ext) == 0) return true;
	}

	return false;
}

namespace OpenGLESRenderer
{
	RenderContextGLES gles;

	void InitGLES()
	{

#if USE_GLES2

		if (!gladLoadGLES2Loader(&LoadGLES2Proc))
		{
			exit(-1);
		}

		glMapBufferRange = (PFNGLMAPBUFFERRANGEEXTPROC)LoadGLES2Proc("glMapBufferRange");
		glUnmapBuffer = (PFNGLUNMAPBUFFEROESPROC)LoadGLES2Proc("glUnmapBuffer");

#else
		static bool first = true;

		if (first)
		{
			if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
			{
				//I_FatalError("Failed to load OpenGL functions.");
			}
		}
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
#endif
		CollectExtensions();

		Printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
		Printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
		Printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
		Printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		Printf("GL_EXTENSIONS:\n");
		for (unsigned i = 0; i < m_Extensions.Size(); i++)
		{
			Printf(" %s\n", m_Extensions[i].GetChars());
		}


		gles.flags = RFL_NO_CLIP_PLANES;

#if USE_GLES2
		gles.useMappedBuffers = false;
		gles.depthStencilAvailable = CheckExtension("GL_OES_packed_depth_stencil");
		gles.npotAvailable = CheckExtension("GL_OES_texture_npot");
		gles.forceGLSLv100 = true;
		gles.maxuniforms = 1024 * 16;
		gles.max_texturesize = 1024 * 4;
		gles.maxlights = 32; // TODO, calcualte this
#else
		gles.useMappedBuffers = true;
		gles.depthStencilAvailable = true;
		gles.npotAvailable = true;
		gles.forceGLSLv100 = false;

		gles.maxuniforms = 1024 * 16;
		gles.max_texturesize = 1024 * 4;
		gles.maxlights = 32; // TODO, calcualte this
#endif
		gles.modelstring = (char*)glGetString(GL_RENDERER);
		gles.vendorstring = (char*)glGetString(GL_VENDOR);
		gles.numlightvectors = (gles.maxlights * LIGHT_VEC4_NUM);
	}

}

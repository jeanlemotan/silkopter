#include "QStdAfx.h"
#include "video/GLES/GLES_Interface.h"

#if defined Q_WINDOWS
#	define NOMINMAX
#	define NOWINRES
#	define NOMCX
#	include <windows.h>
#	include <process.h>
#elif defined Q_POSIX_API
#	include <pthread.h>
#	include <signal.h>
#	include <sched.h>
#	include <unistd.h>
#endif

#if defined(Q_WINDOWS)
#	include <GL/gl.h>
#	include <GL/glu.h>
#	include "video/GLES/std_glext.h"
#elif defined(Q_ANDROID) || defined Q_LINUX
#	define GL_GLEXT_PROTOTYPES
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#elif defined(Q_IOS)
#	include <OpenGLES/ES2/gl.h>
#	include <OpenGLES/ES2/glext.h>
#endif

#if defined(_WIN32)
#	define GL_FUN(x) this->m_funcs->_##x
#else
#	define GL_FUN(x) x
#endif

#if defined(_WIN32)
#	define GL_FUN2(interf, x) interf->m_funcs->_##x
#else
#	define GL_FUN2(interf, x) x
#endif


namespace q
{
namespace video
{
namespace gles
{
	class GL_Ext_Functions
	{
	public:
#if defined(Q_WINDOWS)
		PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC _glRenderbufferStorageMultisample;
		PFNGLBLITFRAMEBUFFERPROC _glBlitFramebuffer;
		PFNGLCHECKFRAMEBUFFERSTATUSPROC _glCheckFramebufferStatus;
		PFNGLCLEARDEPTHFPROC _glClearDepthf;
		PFNGLCOMPRESSEDTEXIMAGE2DPROC _glCompressedTexImage2D;
		PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC _glCompressedTexSubImage2D;
		PFNGLBINDFRAMEBUFFERPROC _glBindFramebuffer;
		PFNGLBINDRENDERBUFFERPROC _glBindRenderbuffer;
		PFNGLACTIVETEXTUREPROC _glActiveTexture;
		PFNGLBLENDCOLORPROC _glBlendColor;
		PFNGLBLENDEQUATIONPROC _glBlendEquation;
		PFNGLBLENDFUNCSEPARATEPROC _glBlendFuncSeparate;
		PFNGLBINDBUFFERPROC _glBindBuffer;
		PFNGLBUFFERDATAPROC _glBufferData;
		PFNGLBUFFERSUBDATAPROC _glBufferSubData;
		PFNGLDELETEBUFFERSPROC _glDeleteBuffers;
		PFNGLGENBUFFERSPROC _glGenBuffers;
		PFNGLGETBUFFERPARAMETERIVPROC _glGetBufferParameteriv;
		PFNGLGETBUFFERPOINTERVPROC _glGetBufferPointerv;
		PFNGLGETBUFFERSUBDATAPROC _glGetBufferSubData;
		PFNGLISBUFFERPROC _glIsBuffer;
		PFNGLMAPBUFFERPROC _glMapBuffer;
		PFNGLUNMAPBUFFERPROC _glUnmapBuffer;
		PFNGLATTACHSHADERPROC _glAttachShader;
		PFNGLBINDATTRIBLOCATIONPROC _glBindAttribLocation;
		PFNGLBLENDEQUATIONSEPARATEPROC _glBlendEquationSeparate;
		PFNGLCOMPILESHADERPROC _glCompileShader;
		PFNGLCREATEPROGRAMPROC _glCreateProgram;
		PFNGLCREATESHADERPROC _glCreateShader;
		PFNGLDELETEPROGRAMPROC _glDeleteProgram;
		PFNGLDELETESHADERPROC _glDeleteShader;
		PFNGLDETACHSHADERPROC _glDetachShader;
		PFNGLDISABLEVERTEXATTRIBARRAYPROC _glDisableVertexAttribArray;
		PFNGLDRAWBUFFERSPROC _glDrawBuffers;
		PFNGLENABLEVERTEXATTRIBARRAYPROC _glEnableVertexAttribArray;
		PFNGLGETACTIVEATTRIBPROC _glGetActiveAttrib;
		PFNGLGETACTIVEUNIFORMPROC _glGetActiveUniform;
		PFNGLGETATTACHEDSHADERSPROC _glGetAttachedShaders;
		PFNGLGETATTRIBLOCATIONPROC _glGetAttribLocation;
		PFNGLGETPROGRAMINFOLOGPROC _glGetProgramInfoLog;
		PFNGLGETPROGRAMIVPROC _glGetProgramiv;
		PFNGLGETSHADERINFOLOGPROC _glGetShaderInfoLog;
		PFNGLGETSHADERSOURCEPROC _glGetShaderSource;
		PFNGLGETSHADERIVPROC _glGetShaderiv;
		PFNGLGETUNIFORMLOCATIONPROC _glGetUniformLocation;
		PFNGLGETUNIFORMFVPROC _glGetUniformfv;
		PFNGLGETUNIFORMIVPROC _glGetUniformiv;
		PFNGLGETVERTEXATTRIBPOINTERVPROC _glGetVertexAttribPointerv;
		PFNGLGETVERTEXATTRIBFVPROC _glGetVertexAttribfv;
		PFNGLGETVERTEXATTRIBIVPROC _glGetVertexAttribiv;
		PFNGLISPROGRAMPROC _glIsProgram;
		PFNGLISSHADERPROC _glIsShader;
		PFNGLLINKPROGRAMPROC _glLinkProgram;
		PFNGLSHADERSOURCEPROC _glShaderSource;
		PFNGLSTENCILFUNCSEPARATEPROC _glStencilFuncSeparate;
		PFNGLSTENCILMASKSEPARATEPROC _glStencilMaskSeparate;
		PFNGLSTENCILOPSEPARATEPROC _glStencilOpSeparate;
		PFNGLUNIFORM1FPROC _glUniform1f;
		PFNGLUNIFORM1FVPROC _glUniform1fv;
		PFNGLUNIFORM1IPROC _glUniform1i;
		PFNGLUNIFORM1IVPROC _glUniform1iv;
		PFNGLUNIFORM2FPROC _glUniform2f;
		PFNGLUNIFORM2FVPROC _glUniform2fv;
		PFNGLUNIFORM2IPROC _glUniform2i;
		PFNGLUNIFORM2IVPROC _glUniform2iv;
		PFNGLUNIFORM3FPROC _glUniform3f;
		PFNGLUNIFORM3FVPROC _glUniform3fv;
		PFNGLUNIFORM3IPROC _glUniform3i;
		PFNGLUNIFORM3IVPROC _glUniform3iv;
		PFNGLUNIFORM4FPROC _glUniform4f;
		PFNGLUNIFORM4FVPROC _glUniform4fv;
		PFNGLUNIFORM4IPROC _glUniform4i;
		PFNGLUNIFORM4IVPROC _glUniform4iv;
		PFNGLUNIFORMMATRIX2FVPROC _glUniformMatrix2fv;
		PFNGLUNIFORMMATRIX3FVPROC _glUniformMatrix3fv;
		PFNGLUNIFORMMATRIX4FVPROC _glUniformMatrix4fv;
		PFNGLUSEPROGRAMPROC _glUseProgram;
		PFNGLVALIDATEPROGRAMPROC _glValidateProgram;
		PFNGLVERTEXATTRIB1FPROC _glVertexAttrib1f;
		PFNGLVERTEXATTRIB1FVPROC _glVertexAttrib1fv;
		PFNGLVERTEXATTRIB2FPROC _glVertexAttrib2f;
		PFNGLVERTEXATTRIB2FVPROC _glVertexAttrib2fv;
		PFNGLVERTEXATTRIB3FPROC _glVertexAttrib3f;
		PFNGLVERTEXATTRIB3FVPROC _glVertexAttrib3fv;
		PFNGLVERTEXATTRIB4FPROC _glVertexAttrib4f;
		PFNGLVERTEXATTRIB4FVPROC _glVertexAttrib4fv;
		PFNGLVERTEXATTRIBPOINTERPROC _glVertexAttribPointer;
		PFNGLDELETEFRAMEBUFFERSPROC _glDeleteFramebuffers;
		PFNGLDELETERENDERBUFFERSPROC _glDeleteRenderbuffers;
		PFNGLDEPTHRANGEFPROC _glDepthRangef;
		PFNGLFRAMEBUFFERRENDERBUFFERPROC _glFramebufferRenderbuffer;
		PFNGLFRAMEBUFFERTEXTURE2DPROC _glFramebufferTexture2D;
		PFNGLGENERATEMIPMAPPROC _glGenerateMipmap;
		PFNGLGENFRAMEBUFFERSPROC _glGenFramebuffers;
		PFNGLGENRENDERBUFFERSPROC _glGenRenderbuffers;
		PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC _glGetFramebufferAttachmentParameteriv;
		PFNGLGETRENDERBUFFERPARAMETERIVPROC _glGetRenderbufferParameteriv;
		PFNGLGETSHADERPRECISIONFORMATPROC _glGetShaderPrecisionFormat;
		PFNGLISFRAMEBUFFERPROC _glIsFramebuffer;
		PFNGLISRENDERBUFFERPROC _glIsRenderbuffer;
		PFNGLRENDERBUFFERSTORAGEPROC _glRenderbufferStorage;
		PFNGLRELEASESHADERCOMPILERPROC _glReleaseShaderCompiler;
		PFNGLSAMPLECOVERAGEPROC _glSampleCoverage;
		PFNGLSHADERBINARYPROC _glShaderBinary;
		PFNGLBINDVERTEXARRAYPROC _glBindVertexArray;
		PFNGLDELETEVERTEXARRAYSPROC _glDeleteVertexArrays;
		PFNGLGENVERTEXARRAYSPROC _glGenVertexArrays;
		PFNGLISVERTEXARRAYPROC _glIsVertexArray;
#endif
	};


#if defined Q_USE_RENDER_THREAD
	static const bool s_enableFullCaching = false;	//the caching doesn't work with multithread rendering
#else
	static const bool s_enableFullCaching = true;
#endif

	//static bool s_rebindTextures = false;
	//static bool s_rebindPrograms = false;

#if defined Q_WINDOWS
	__declspec(thread) TL_Interface* tl_interface;
#elif defined Q_ANDROID
	__thread TL_Interface* tl_interface;
#elif defined Q_LINUX
	__thread TL_Interface* tl_interface;
#else
#	error define for your platform
#endif

std::vector<TL_Interface*> s_threadInterfaces;
std::mutex s_threadInterfacesMutex;

void init_for_thread(void* context)
{
	QASSERT(context);

	TL_Interface* interf = tl_interface;
	if (!interf)
	{
		std::lock_guard<std::mutex> sm(s_threadInterfacesMutex);

		interf = new TL_Interface();
		tl_interface = interf;
		interf->internal_load();
		interf->sync();

		s_threadInterfaces.push_back(interf);
	}
}
void shutdown_for_thread(void* context)
{
	QASSERT(context);

	TL_Interface* interf = tl_interface;
	if (!interf)
	{
		std::lock_guard<std::mutex> sm(s_threadInterfacesMutex);
		s_threadInterfaces.erase(std::remove(s_threadInterfaces.begin(), s_threadInterfaces.end(), interf), s_threadInterfaces.end());

		delete interf;
		tl_interface = 0;
	}
}
void set_context(void* context)
{
	init_for_thread(context);
}

Interface::Interface()
{
	//gets the crt pointer for the thread. The sCrtInterface is TLS
	m_interface = tl_interface;
	QASSERT(m_interface);
}

TL_Interface* Interface::get_interface()
{
	return m_interface;
}


void Interface::iglActiveTexture(uint32_t texture)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	state.activeTexture = texture;
}
void Interface::iglAttachShader(uint32_t program, uint32_t shader)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glAttachShader(program, shader));
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_program_rebinding(program);
#endif
}
void Interface::iglBindAttribLocation(uint32_t program, uint32_t index, const char* name)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glBindAttribLocation(program, index, name));
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_program_rebinding(program);
#endif
}
void Interface::iglBindBuffer(uint32_t target, uint32_t buffer)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (state.bindVertexArray == 0)
	{
		//we cache the binding for vertex aray 0 only.
		//for any other array force flush all the time
		if (target == iGL_ARRAY_BUFFER)
		{
			if (state.bindArrayBuffer != buffer)
			{
				state.bindArrayBuffer = buffer;
				state.buffersChanged = true;
			}
		}
		else if (target == iGL_ELEMENT_ARRAY_BUFFER)
		{
			if (state.bindElementBuffer != buffer)
			{
				state.bindElementBuffer = buffer;
				state.buffersChanged = true;
			}
		}
		else
		{
			QASSERT(0);
		}
	}
	else
	{
		interf->flush();
		GL_FUN2(interf, glBindBuffer(target, buffer));
		iassertNoError();
	}
	//interf->_glBindBuffer(target, buffer);
}
void Interface::iglBindFramebuffer(uint32_t target, uint32_t framebuffer)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;

	QASSERT(
		target == iGL_FRAMEBUFFER ||
		target == iGL_READ_FRAMEBUFFER ||
		target == iGL_DRAW_FRAMEBUFFER);

	if (target == iGL_FRAMEBUFFER)
	{
		state.bindFramebuffer = framebuffer;
		state.buffersChanged = true;
	}
	else if (target == iGL_READ_FRAMEBUFFER || target == iGL_DRAW_FRAMEBUFFER)
	{
		interf->flush();
		GL_FUN2(interf, glBindFramebuffer(target, framebuffer));
		iassertNoError();
	}

	if (!s_enableFullCaching)
	{
		interf->flush();
		GL_FUN2(interf, glBindFramebuffer(target, framebuffer));
		iassertNoError();
	}
}
void Interface::iglBindRenderbuffer(uint32_t target, uint32_t renderbuffer)
{
	if (s_enableFullCaching)
	{
		TL_Interface* interf = get_interface();
		TL_Interface::GL_State& state = interf->m_crt_state;
		QASSERT(target == iGL_RENDERBUFFER);
		state.bindRenderbuffer = renderbuffer;
		state.buffersChanged = true;
	}
	else
	{
		TL_Interface* interf = get_interface();
		interf->flush();
		GL_FUN2(interf, glBindRenderbuffer(target, renderbuffer));
		iassertNoError();
	}
}
void Interface::iglBindTexture(uint32_t target, uint32_t texture)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	QASSERT(target == iGL_TEXTURE_2D || target == iGL_TEXTURE_CUBE_MAP);
	uint32_t activeIdx = state.activeTexture - iGL_TEXTURE0;
	QASSERT(activeIdx < state.bindTextures.size());
	state.bindTextures[activeIdx] = std::make_pair(texture, target == iGL_TEXTURE_CUBE_MAP);
	state.bindTexturesLastChangedIdx = std::max<int>(activeIdx, state.bindTexturesLastChangedIdx);

// 	if (!s_enableFullCaching)
// 	{
// 		interf->flush();
// 		::glBindTexture(target, texture);
// 		iassertNoError();
// 	}
}
void Interface::iglBlendColor(float red, float green, float blue, float alpha)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	math::vec4f color(red, green, blue, alpha);
	if (*(math::vec4s32*)&state.blendColor != *(math::vec4s32*)&color)
	{
		state.blendColor = color;
		state.blendChanged = true;
	}
}
void Interface::iglBlendEquation(uint32_t mode)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (state.blendEqRGB != mode || state.blendEqAlpha != mode)
	{
		state.blendEqRGB = mode;
		state.blendEqAlpha = mode;
		state.blendChanged = true;
	}
	state.blendEqSeparate = false;
}
void Interface::iglBlendEquationSeparate(uint32_t modeRGB, uint32_t modeAlpha)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (state.blendEqRGB != modeRGB || state.blendEqAlpha != modeAlpha)
	{
		state.blendEqRGB = modeRGB;
		state.blendEqAlpha = modeAlpha;
		state.blendChanged = true;
	}
	state.blendEqSeparate = modeRGB != modeAlpha;
}
void Interface::iglBlendFunc(uint32_t sfactor, uint32_t dfactor)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (state.blendFuncSrcRGB != sfactor || state.blendFuncSrcAlpha != sfactor ||
		state.blendFuncDstRGB != dfactor || state.blendFuncDstAlpha != dfactor)
	{
		state.blendFuncSrcRGB = sfactor;
		state.blendFuncSrcAlpha = sfactor;
		state.blendFuncDstRGB = dfactor;
		state.blendFuncDstAlpha = dfactor;
		state.blendChanged = true;
	}
	state.blendFuncSeparate = false;
}
void Interface::iglBlendFuncSeparate(uint32_t srcRGB, uint32_t dstRGB, uint32_t srcAlpha, uint32_t dstAlpha)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (state.blendFuncSrcRGB != srcRGB || state.blendFuncSrcAlpha != srcAlpha ||
		state.blendFuncDstRGB != dstRGB || state.blendFuncDstAlpha != dstAlpha)
	{
		state.blendFuncSrcRGB = srcRGB;
		state.blendFuncSrcAlpha = srcAlpha;
		state.blendFuncDstRGB = dstRGB;
		state.blendFuncDstAlpha = dstAlpha;
		state.blendChanged = true;
	}
	state.blendFuncSeparate = srcRGB != srcAlpha || dstRGB != dstAlpha;
}
void Interface::iglBufferData(gles::iGLenum target, iGLsizeiptr size, const void* data, gles::iGLenum usage)
{
	TL_Interface* interf = get_interface();
	//TLInterface::iGLState& state = interf->m_crtState;
	interf->flush();
	GL_FUN2(interf, glBufferData(target, size, data, usage));
#ifdef Q_USE_RENDER_THREAD
	if (target == iGL_ARRAY_BUFFER)
	{
		interf->force_array_buffer_rebinding(state.bindArrayBuffer);
	}
	else if (target == iGL_ELEMENT_ARRAY_BUFFER)
	{
		interf->force_array_buffer_rebinding(state.bindElementBuffer);
	}
#endif
	iassertNoError();
}
void Interface::iglBufferSubData(gles::iGLenum target, iGLintptr offset, iGLsizeiptr size, const void* data)
{
	TL_Interface* interf = get_interface();
	//TLInterface::iGLState& state = interf->m_crtState;
	interf->flush();
	GL_FUN2(interf, glBufferSubData(target, offset, size, data));
#ifdef Q_USE_RENDER_THREAD
	if (target == iGL_ARRAY_BUFFER)
		interf->force_array_buffer_rebinding(state.bindArrayBuffer);
	else if (target == iGL_ELEMENT_ARRAY_BUFFER)
		interf->force_array_buffer_rebinding(state.bindElementBuffer);
#endif
	iassertNoError();
}
uint32_t Interface::iglCheckFramebufferStatus(uint32_t target)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = GL_FUN2(interf, glCheckFramebufferStatus(target));
	iassertNoError();
	return ret;
}
void Interface::iglClear(uint32_t mask)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glClear(mask);
	iassertNoError();
}
void Interface::iglClearColor(float red, float green, float blue, float alpha)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	math::vec4f color(red, green, blue, alpha);
	if (*(math::vec4s32*)&state.clearColor != *(math::vec4s32*)&color)
	{
		state.clearColor = color;
		state.clearChanged = true;
	}
}
void Interface::iglClearDepthf(float depth)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (*(int*)&state.clearDepth != *(int*)&depth)
	{
		state.clearDepth = depth;
		state.clearChanged = true;
	}
}
void Interface::iglClearStencil(int s)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (state.clearStencil != s)
	{
		state.clearStencil = s;
		state.clearChanged = true;
	}
}
void Interface::iglColorMask(gles::iGLboolean red, gles::iGLboolean green, gles::iGLboolean blue, gles::iGLboolean alpha)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	math::vec4<iGLboolean> mask(red, green, blue, alpha);
	if (mask != state.colorMask)
	{
		state.colorMask = mask;
		state.flagsChanged = true;
	}
}
void Interface::iglCompileShader(uint32_t shader)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glCompileShader(shader));
	iassertNoError();
}
void Interface::iglCompressedTexImage2D(uint32_t target, int level, uint32_t internalformat, int width, int height, int border, int imageSize, const void* data)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data));
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_texture_rebinding();
#endif
}
void Interface::iglCompressedTexSubImage2D(uint32_t target, int level, int xoffset, int yoffset, int width, int height, uint32_t format, int imageSize, const void* data)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data));
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_texture_rebinding();
#endif
}
void Interface::iglCopyTexImage2D(uint32_t target, int level, uint32_t internalformat, int x, int y, int width, int height, int border)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_texture_rebinding();
#endif
}
void Interface::iglCopyTexSubImage2D(uint32_t target, int level, int xoffset, int yoffset, int x, int y, int width, int height)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_texture_rebinding();
#endif
}
uint32_t Interface::iglCreateProgram(void)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = GL_FUN2(interf, glCreateProgram());
	iassertNoError();
	return ret;
}
uint32_t Interface::iglCreateShader(uint32_t type)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = GL_FUN2(interf, glCreateShader(type));
	iassertNoError();
	return ret;
}
void Interface::iglCullFace(uint32_t mode)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (mode != state.cullFace)
	{
		state.cullFace = mode;
		state.flagsChanged = true;
	}
}
void Interface::iglDeleteBuffers(int n, const uint32_t* buffers)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glDeleteBuffers(n, buffers));
	iassertNoError();

	for (int i = 0; i < n; i++)
	{
		interf->remove_buffer(buffers[i]);
	}
}
void Interface::iglDeleteFramebuffers(int n, const uint32_t* framebuffers)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glDeleteFramebuffers(n, framebuffers));
	iassertNoError();
	for (int i = 0; i < n; i++)
	{
		interf->remove_framebuffer(framebuffers[i]);
	}
}
void Interface::iglDeleteProgram(uint32_t program)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	interf->flush();
	GL_FUN2(interf, glDeleteProgram(program));
	// clear state
	if (state.program == program)
	{
		state.program = 0;
	}
	iassertNoError();
	interf->remove_program(program);
}
void Interface::iglDeleteRenderbuffers(int n, const uint32_t* renderbuffers)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glDeleteRenderbuffers(n, renderbuffers));
	iassertNoError();
	for (int i = 0; i < n; i++)
	{
		interf->remove_renderbuffer(renderbuffers[i]);
	}
}
void Interface::iglDeleteShader(uint32_t shader)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glDeleteShader(shader));
	iassertNoError();
}

void Interface::iglDeleteTextures(int n, const uint32_t* textures)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glDeleteTextures(n, textures);

	iassertNoError();
	for (int i = 0; i < n; i++)
	{
		interf->remove_texture(textures[i]);
	}
}
void Interface::iglDepthFunc(uint32_t func)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (func != state.depthFunc)
	{
		state.depthFunc = func;
		state.flagsChanged = true;
	}
}
void Interface::iglDepthMask(gles::iGLboolean flag)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (flag != state.depthMask)
	{
		state.depthMask = flag;
		state.flagsChanged = true;
	}
}
void Interface::iglDepthRangef(float zNear, float zFar)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (*(int*)&zNear != *(int*)&state.depthNear || *(int*)&zFar != *(int*)&state.depthFar)
	{
		state.depthNear = zNear;
		state.depthFar = zFar;
		state.flagsChanged = true;
	}
}
void Interface::iglDetachShader(uint32_t program, uint32_t shader)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glDetachShader(program, shader));
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_program_rebinding(program);
#endif
}
void Interface::iglDisable(uint32_t cap)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	switch (cap)
	{
	case iGL_BLEND:
		state.capsChanged |= state.capBlend != iGL_FALSE;
		state.capBlend = iGL_FALSE;
		break;
	case iGL_CULL_FACE:
		state.capsChanged |= state.capCullFace != iGL_FALSE;
		state.capCullFace = iGL_FALSE;
		break;
	case iGL_DEPTH_TEST:
		state.capsChanged |= state.capDepthTest != iGL_FALSE;
		state.capDepthTest = iGL_FALSE;
		break;
	case iGL_DITHER:
		state.capsChanged |= state.capDither != iGL_FALSE;
		state.capDither = iGL_FALSE;
		break;
	case iGL_POLYGON_OFFSET_FILL:
		state.capsChanged |= state.capPolygonOffset != iGL_FALSE;
		state.capPolygonOffset = iGL_FALSE;
		break;
	case iGL_SAMPLE_ALPHA_TO_COVERAGE:
		state.capsChanged |= state.capSampleAlphaToCoverage != iGL_FALSE;
		state.capSampleAlphaToCoverage = iGL_FALSE;
		break;
	case iGL_SAMPLE_COVERAGE:
		state.capsChanged |= state.capSampleCoverage != iGL_FALSE;
		state.capSampleCoverage = iGL_FALSE;
		break;
	case iGL_SCISSOR_TEST:
		state.capsChanged |= state.capScissorTest != iGL_FALSE;
		state.capScissorTest = iGL_FALSE;
		break;
	case iGL_STENCIL_TEST:
		state.capsChanged |= state.capStencilTest != iGL_FALSE;
		state.capStencilTest = iGL_FALSE;
		break;
	default:
		QASSERT(0);
	}
}
void Interface::iglDisableVertexAttribArray(uint32_t index)
{
	//if (s_enableFullCaching)
	{
		TL_Interface* interf = get_interface();
		TL_Interface::GL_State& state = interf->m_crt_state;
		QASSERT(index < state.vertexAttribs.size());
		state.vertexAttribs[index] = 0;
		state.vertexAttribsLastChangedIdx = std::max(state.vertexAttribsLastChangedIdx, (int)index);
	}
	/*else
	{
		TLInterface* interf = GetInterface();
		TLInterface::iGLState& state = interf->m_crtState;
		interf->flush();
#if defined(Q_WINDOWS)
		interf->_glDisableVertexAttribArray(index);
#else
		::glDisableVertexAttribArray(index);
#endif
		iassertNoError();
	}*/
}
// void Interface::iglDiscardFramebuffer(iGLenum target, iGLsizei numAttachments, const iGLenum *attachments)
// {
// 	TLInterface* interf = GetInterface();
//	TLInterface::iGLState& state = interf->m_crtState;
// 	interf->flush();
// 	if (interf->m_funcs->_glDiscardFramebuffer)
// 	{
// 		interf->m_funcs->_glDiscardFramebuffer(target, numAttachments, attachments);
// 	}
// 	iassertNoError();
// #ifdef Q_USE_RENDER_THREAD
// 	interf->forceFramebufferRebinding(state.bindArrayBuffer);
// #endif
// }

void Interface::iglDrawArrays(uint32_t mode, int first, int count)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glDrawArrays(mode, first, count);
	iassertNoError();
}
void Interface::iglDrawElements(uint32_t mode, int count, uint32_t type, const void* indices)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glDrawElements(mode, count, type, indices);
	iassertNoError();
}
void Interface::iglEnable(uint32_t cap)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	switch (cap)
	{
	case iGL_BLEND:
		state.capsChanged |= state.capBlend != iGL_TRUE;
		state.capBlend = iGL_TRUE;
		break;
	case iGL_CULL_FACE:
		state.capsChanged |= state.capCullFace != iGL_TRUE;
		state.capCullFace = iGL_TRUE;
		break;
	case iGL_DEPTH_TEST:
		state.capsChanged |= state.capDepthTest != iGL_TRUE;
		state.capDepthTest = iGL_TRUE;
		break;
	case iGL_DITHER:
		state.capsChanged |= state.capDither != iGL_TRUE;
		state.capDither = iGL_TRUE;
		break;
	case iGL_POLYGON_OFFSET_FILL:
		state.capsChanged |= state.capPolygonOffset != iGL_TRUE;
		state.capPolygonOffset = iGL_TRUE;
		break;
	case iGL_SAMPLE_ALPHA_TO_COVERAGE:
		state.capsChanged |= state.capSampleAlphaToCoverage != iGL_TRUE;
		state.capSampleAlphaToCoverage = iGL_TRUE;
		break;
	case iGL_SAMPLE_COVERAGE:
		state.capsChanged |= state.capSampleCoverage != iGL_TRUE;
		state.capSampleCoverage = iGL_TRUE;
		break;
	case iGL_SCISSOR_TEST:
		state.capsChanged |= state.capScissorTest != iGL_TRUE;
		state.capScissorTest = iGL_TRUE;
		break;
	case iGL_STENCIL_TEST:
		state.capsChanged |= state.capStencilTest != iGL_TRUE;
		state.capStencilTest = iGL_TRUE;
		break;
	default:
		QASSERT(0);
	}
}
void Interface::iglEnableVertexAttribArray(uint32_t index)
{
	//if (s_enableFullCaching)
	{
		TL_Interface* interf = get_interface();
		TL_Interface::GL_State& state = interf->m_crt_state;
		QASSERT(index < state.vertexAttribs.size());
		state.vertexAttribs[index] = 1;
		state.vertexAttribsLastChangedIdx = std::max(state.vertexAttribsLastChangedIdx, (int)index);
	}
	/*else
	{
		TLInterface* interf = GetInterface();
		TLInterface::iGLState& state = interf->m_crtState;
		interf->flush();
#if defined(Q_WINDOWS)
		interf->_glEnableVertexAttribArray(index);
#else
		::glEnableVertexAttribArray(index);
#endif
		iassertNoError();
	}*/
}
void Interface::iglFinish(void)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glFinish();
	iassertNoError();
}
void Interface::iglFlush(void)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glFlush();
	iassertNoError();
}
void Interface::iglFramebufferRenderbuffer(uint32_t target, uint32_t attachment, uint32_t renderbuffertarget, uint32_t renderbuffer)
{
	TL_Interface* interf = get_interface();
	//TLInterface::iGLState& state = interf->m_crtState;
	interf->flush();
	GL_FUN2(interf, glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer));
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_framebuffer_rebinding(state.bindArrayBuffer);
#endif
}
void Interface::iglFramebufferTexture2D(uint32_t target, uint32_t attachment, uint32_t textarget, uint32_t texture, int level)
{
	TL_Interface* interf = get_interface();
	//TLInterface::iGLState& state = interf->m_crtState;
	interf->flush();
	GL_FUN2(interf, glFramebufferTexture2D(target, attachment, textarget, texture, level));
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_framebuffer_rebinding(state.bindArrayBuffer);
#endif
}
void Interface::iglFrontFace(uint32_t mode)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (mode != state.frontFace)
	{
		state.frontFace = mode;
		state.flagsChanged = true;
	}
}
void Interface::iglGenBuffers(int n, uint32_t* buffers)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGenBuffers(n, buffers));
	iassertNoError();
}
void Interface::iglGenerateMipmap(uint32_t target)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGenerateMipmap(target));
	iassertNoError();
}
void Interface::iglGenFramebuffers(int n, uint32_t* framebuffers)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGenFramebuffers(n, framebuffers));
	iassertNoError();
}
void Interface::iglGenRenderbuffers(int n, uint32_t* renderbuffers)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGenRenderbuffers(n, renderbuffers));
	iassertNoError();
}
void Interface::iglGenTextures(int n, uint32_t* textures)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glGenTextures(n, textures);
	iassertNoError();
}
void Interface::iglGetActiveAttrib(uint32_t program, uint32_t index, int bufsize, int* length, int* size, uint32_t* type, char* name)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetActiveAttrib(program, index, bufsize, length, size, type, name));
	iassertNoError();
}
void Interface::iglGetActiveUniform(uint32_t program, uint32_t index, int bufsize, int* length, int* size, uint32_t* type, char* name)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetActiveUniform(program, index, bufsize, length, size, type, name));
	iassertNoError();
}
void Interface::iglGetAttachedShaders(uint32_t program, int maxcount, int* count, uint32_t* shaders)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetAttachedShaders(program, maxcount, count, shaders));
	iassertNoError();
}
int Interface::iglGetAttribLocation(uint32_t program, const char* name)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = GL_FUN2(interf, glGetAttribLocation(program, name));
	iassertNoError();
	return ret;
}
void Interface::iglGetBooleanv(gles::iGLenum pname, gles::iGLboolean* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glGetBooleanv(pname, params);
	iassertNoError();
}
void Interface::iglGetBufferParameteriv(uint32_t target, uint32_t pname, int* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetBufferParameteriv(target, pname, params));
	iassertNoError();
}
uint32_t Interface::iglGetError(void)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	return ::glGetError();
}
void Interface::iglGetFloatv(uint32_t pname, float* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glGetFloatv(pname, params);
	iassertNoError();
}
void Interface::iglGetFramebufferAttachmentParameteriv(uint32_t target, uint32_t attachment, uint32_t pname, int* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetFramebufferAttachmentParameteriv(target, attachment, pname, params));
	iassertNoError();
}
void Interface::iglGetIntegerv(uint32_t pname, int* params)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	switch (pname)
	{
	case iGL_FRAMEBUFFER_BINDING:
		params[0] = state.bindFramebuffer;
		break;
	case iGL_VIEWPORT:
		*(math::vec4s32*)params = state.viewport;
		break;
	case iGL_CURRENT_PROGRAM:
		params[0] = state.program;
		break;
	case iGL_TEXTURE_BINDING_2D:
		{
			uint32_t activeIdx = state.activeTexture - iGL_TEXTURE0;
			bool isCube = state.bindTextures[activeIdx].second;
			params[0] = isCube ? 0 : state.bindTextures[activeIdx].first;
		}
		break;
	case iGL_TEXTURE_BINDING_CUBE_MAP:
		{
			uint32_t activeIdx = state.activeTexture - iGL_TEXTURE0;
			bool isCube = state.bindTextures[activeIdx].second;
			params[0] = isCube ? state.bindTextures[activeIdx].first : 0;
		}
		break;
	case iGL_ACTIVE_TEXTURE:
		params[0] = state.activeTexture;
		break;
	case iGL_UNPACK_ALIGNMENT:
		params[0] = state.pixelStoreUnbpackAlignment;
		break;
	default:
		interf->flush();
		::glGetIntegerv(pname, params);
		iassertNoError();
		break;
	}
}
void Interface::iglGetProgramiv(uint32_t program, uint32_t pname, int* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetProgramiv(program, pname, params));
	iassertNoError();
}
void Interface::iglGetProgramInfoLog(uint32_t program, int bufsize, int* length, char* infolog)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetProgramInfoLog(program, bufsize, length, infolog));
	iassertNoError();
}
void Interface::iglGetRenderbufferParameteriv(uint32_t target, uint32_t pname, int* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetRenderbufferParameteriv(target, pname, params));
	iassertNoError();
}
void Interface::iglGetShaderiv(uint32_t shader, uint32_t pname, int* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetShaderiv(shader, pname, params));
	iassertNoError();
}
void Interface::iglGetShaderInfoLog(uint32_t shader, int bufsize, int* length, char* infolog)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetShaderInfoLog(shader, bufsize, length, infolog));
	iassertNoError();
}
void Interface::iglGetShaderPrecisionFormat(uint32_t shadertype, uint32_t precisiontype, int* range, int* precision)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision));
	iassertNoError();
}
void Interface::iglGetShaderSource(uint32_t shader, int bufsize, int* length, char* source)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetShaderSource(shader, bufsize, length, source));
	iassertNoError();
}
const uint8_t* Interface::iglGetString(uint32_t name)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = ::glGetString(name);
	iassertNoError();
	return ret;
}
void Interface::iglGetTexParameterfv(uint32_t target, uint32_t pname, float* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glGetTexParameterfv(target, pname, params);
	iassertNoError();
}
void Interface::iglGetTexParameteriv(uint32_t target, uint32_t pname, int* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	::glGetTexParameteriv(target, pname, params);
	iassertNoError();
}
void Interface::iglGetUniformfv(uint32_t program, int location, float* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetUniformfv(program, location, params));
	iassertNoError();
}
void Interface::iglGetUniformiv(uint32_t program, int location, int* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetUniformiv(program, location, params));
	iassertNoError();
}
int Interface::iglGetUniformLocation(uint32_t program, const char* name)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = GL_FUN2(interf, glGetUniformLocation(program, name));
	iassertNoError();
	return ret;
}
void Interface::iglGetVertexAttribfv(uint32_t index, uint32_t pname, float* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetVertexAttribfv(index, pname, params));
	iassertNoError();
}
void Interface::iglGetVertexAttribiv(uint32_t index, uint32_t pname, int* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetVertexAttribiv(index, pname, params));
	iassertNoError();
}
void Interface::iglGetVertexAttribPointerv(uint32_t index, uint32_t pname, void** pointer)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glGetVertexAttribPointerv(index, pname, pointer));
	iassertNoError();
}
void Interface::iglHint(uint32_t target, uint32_t mode)
{
	//TLInterface* interf = GetInterface();
	//TLInterface::iGLState& state = interf->m_crtState;
    ::glHint(target, mode);
    iassertNoError();
	/*if (target == iGL_GENERATE_MIPMAP_HINT)
	{
		if (mode != state.hintGenerateMipmap)
		{
			state.hintGenerateMipmap = mode;
			state.flagsChanged = true;
		}
	}
	else
	{
		QASSERT(0);
	}*/
}
gles::iGLboolean Interface::iglIsBuffer(uint32_t buffer)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = GL_FUN2(interf, glIsBuffer(buffer));
	iassertNoError();
	return ret;
}
gles::iGLboolean Interface::iglIsEnabled(uint32_t cap)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = glIsEnabled(cap);
	iassertNoError();
	return ret;
}
gles::iGLboolean Interface::iglIsFramebuffer(uint32_t framebuffer)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = GL_FUN2(interf, glIsFramebuffer(framebuffer));
	iassertNoError();
	return ret;
}
gles::iGLboolean Interface::iglIsProgram(uint32_t program)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = GL_FUN2(interf, glIsProgram(program));
	iassertNoError();
	return ret;
}
gles::iGLboolean Interface::iglIsRenderbuffer(uint32_t renderbuffer)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = GL_FUN2(interf, glIsRenderbuffer(renderbuffer));
	iassertNoError();
	return ret;
}
gles::iGLboolean Interface::iglIsShader(uint32_t shader)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = GL_FUN2(interf, glIsShader(shader));
	iassertNoError();
	return ret;
}
gles::iGLboolean Interface::iglIsTexture(uint32_t texture)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	auto ret = glIsTexture(texture);
	iassertNoError();
	return ret;
}
void Interface::iglLineWidth(float width)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (*(int*)&width != *(int*)&state.lineWidth)
	{
		state.lineWidth = width;
		state.flagsChanged = true;
	}
}
void Interface::iglLinkProgram(uint32_t program)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glLinkProgram(program));
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_program_rebinding(program);
#endif
}
void Interface::iglPixelStorei(uint32_t pname, int param)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	interf->flush();
	glPixelStorei(pname, param);
	iassertNoError();

	if (pname == iGL_UNPACK_ALIGNMENT)
	{
		state.pixelStoreUnbpackAlignment = param;
	}
}
void Interface::iglPolygonOffset(float factor, float units)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (*(int*)&factor != *(int*)&state.polygonOffsetFactor ||
		*(int*)&units != *(int*)&state.polygonOffsetUnits)
	{
		state.polygonOffsetFactor = factor;
		state.polygonOffsetUnits = units;
		state.flagsChanged = true;
	}
}
void Interface::iglReadPixels(int x, int y, int width, int height, uint32_t format, uint32_t type, void* pixels)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	glReadPixels(x, y, width, height, format, type, pixels);
	iassertNoError();
}
void Interface::iglReleaseShaderCompiler(void)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glReleaseShaderCompiler());
	iassertNoError();
}
void Interface::iglRenderbufferStorage(uint32_t target, uint32_t internalformat, int width, int height)
{
	TL_Interface* interf = get_interface();
	//TLInterface::iGLState& state = interf->m_crtState;
	interf->flush();
	GL_FUN2(interf, glRenderbufferStorage(target, internalformat, width, height));
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_renderbuffer_rebinding(state.bindRenderbuffer);
#endif
}

void Interface::iglRenderbufferStorageMultisample(iGLenum target, iGLsizei samples, iGLenum internalformat, iGLsizei width, iGLsizei height)
{
	TL_Interface* interf = get_interface();
	//TLInterface::iGLState& state = interf->m_crtState;
	interf->flush();

#ifdef Q_WINDOWS
	GL_FUN2(interf, glRenderbufferStorageMultisample(target, samples, internalformat, width, height));
	iassertNoError();
#endif
#ifdef Q_IOS
	if (internalformat == iGL_RGBA)
	{
		internalformat = GL_RGBA8_OES;//special format for apple
	}
	glRenderbufferStorageMultisampleAPPLE(target, samples, internalformat, width, height);
	iassertNoError();
#endif

#ifdef Q_USE_RENDER_THREAD
	interf->force_renderbuffer_rebinding(state.bindRenderbuffer);
#endif
}

void Interface::iglBlitFramebuffer(iGLint srcX0, iGLint srcY0, iGLint srcX1, iGLint srcY1, iGLint dstX0, iGLint dstY0, iGLint dstX1, iGLint dstY1, iGLbitfield mask, iGLenum filter)
{
	TL_Interface* interf = get_interface();
	interf->flush();
#ifdef Q_WINDOWS
	GL_FUN2(interf, glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter));
#endif

#ifdef Q_IOS
	glResolveMultisampleFramebufferAPPLE();
	//GLenum attachments[] = { GL_STENCIL_ATTACHMENT, GL_DEPTH_ATTACHMENT };

	// a bug in in ipad3 driver seems to discard other attachments sometimes  (more precisely, the color attachment), so this code is disabled by now
	//GLenum attachments[] = { GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT };
	//glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, attachments);
#endif
	iassertNoError();
}

void Interface::iglSampleCoverage(gles::iGLclampf value, gles::iGLboolean invert)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (*(int*)&value != *(int*)&state.sampleCoverageValue ||
		invert != state.sampleCoverageInvert)
	{
		state.sampleCoverageValue = value;
		state.sampleCoverageInvert = invert;
		state.flagsChanged = true;
	}
}
void Interface::iglScissor(int x, int y, int width, int height)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	math::vec4s32 sc(x, y, width, height);
	if (sc != state.scissorsBox)
	{
		state.scissorsBox = sc;
		state.flagsChanged = true;
	}
}
void Interface::iglShaderBinary(int n, const uint32_t* shaders, uint32_t binaryformat, const void* binary, int length)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glShaderBinary(n, shaders, binaryformat, binary, length));
	iassertNoError();
}
void Interface::iglShaderSource(uint32_t shader, int count, const char** string, const int* length)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glShaderSource(shader, count, string, length));
	iassertNoError();
}
void Interface::iglStencilFunc(uint32_t func, int ref, uint32_t mask)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (state.stencilFuncF != func || state.stencilFuncB != func ||
		state.stencilFuncRefF != ref || state.stencilFuncRefB != ref ||
		state.stencilFuncMaskF != mask || state.stencilFuncMaskB != mask)
	{
		state.stencilFuncF = func;
		state.stencilFuncB = func;
		state.stencilFuncRefF = ref;
		state.stencilFuncRefB = ref;
		state.stencilFuncMaskF = mask;
		state.stencilFuncMaskB = mask;

		state.stencilChanged = true;
	}
	state.stencilFuncSeparate = false;
}
void Interface::iglStencilFuncSeparate(uint32_t face, uint32_t func, int ref, uint32_t mask)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (face == iGL_FRONT)
	{
		if (state.stencilFuncF != func || state.stencilFuncRefF != ref || state.stencilFuncMaskF != mask)
		{
			state.stencilFuncF = func;
			state.stencilFuncRefF = ref;
			state.stencilFuncMaskF = mask;

			state.stencilChanged = true;
		}
		state.stencilFuncSeparate = func != state.stencilFuncB || ref != state.stencilFuncRefB || mask != state.stencilFuncMaskB;
	}
	else if (face == iGL_BACK)
	{
		if (state.stencilFuncB != func || state.stencilFuncRefB != ref || state.stencilFuncMaskB != mask)
		{
			state.stencilFuncB = func;
			state.stencilFuncRefB = ref;
			state.stencilFuncMaskB = mask;

			state.stencilChanged = true;
		}
		state.stencilFuncSeparate = func != state.stencilFuncF || ref != state.stencilFuncRefF || mask != state.stencilFuncMaskF;
	}
	else if (face == iGL_FRONT_AND_BACK)
	{
		if (state.stencilFuncF != func || state.stencilFuncB != func ||
			state.stencilFuncRefF != ref || state.stencilFuncRefB != ref ||
			state.stencilFuncMaskF != mask || state.stencilFuncMaskB != mask)
		{
			state.stencilFuncF = func;
			state.stencilFuncB = func;
			state.stencilFuncRefF = ref;
			state.stencilFuncRefB = ref;
			state.stencilFuncMaskF = mask;
			state.stencilFuncMaskB = mask;

			state.stencilChanged = true;
		}
		state.stencilFuncSeparate = false;
	}
}
void Interface::iglStencilMask(uint32_t mask)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (state.stencilMaskF != mask || state.stencilMaskB != mask)
	{
		state.stencilMaskF = mask;
		state.stencilMaskB = mask;
		state.stencilChanged = true;
	}
	state.stencilMaskSeparate = false;
}
void Interface::iglStencilMaskSeparate(uint32_t face, uint32_t mask)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (face == iGL_FRONT)
	{
		if (state.stencilMaskF != mask)
		{
			state.stencilMaskF = mask;
			state.stencilChanged = true;
		}
		state.stencilMaskSeparate = mask != state.stencilMaskB;
	}
	else if (face == iGL_BACK)
	{
		if (state.stencilMaskB != mask)
		{
			state.stencilMaskB = mask;
			state.stencilChanged = true;
		}
		state.stencilMaskSeparate = mask != state.stencilMaskF;
	}
	else if (face == iGL_FRONT_AND_BACK)
	{
		if (state.stencilMaskF != mask || state.stencilMaskB != mask)
		{
			state.stencilMaskF = mask;
			state.stencilMaskB = mask;

			state.stencilChanged = true;
		}
		state.stencilMaskSeparate = false;
	}
}
void Interface::iglStencilOp(uint32_t fail, uint32_t zfail, uint32_t zpass)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (state.stencilOpFailF != fail || state.stencilOpFailB != fail ||
		state.stencilOpZFailF != zfail || state.stencilOpZFailB != zfail ||
		state.stencilOpZPassF != zpass || state.stencilOpZPassB != zpass)
	{
		state.stencilOpFailF = fail;
		state.stencilOpFailB = fail;
		state.stencilOpZFailF = zfail;
		state.stencilOpZFailB = zfail;
		state.stencilOpZPassF = zpass;
		state.stencilOpZPassB = zpass;

		state.stencilChanged = true;
	}
	state.stencilOpSeparate = false;
}
void Interface::iglStencilOpSeparate(uint32_t face, uint32_t fail, uint32_t zfail, uint32_t zpass)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	if (face == iGL_FRONT)
	{
		if (state.stencilOpFailF != fail || state.stencilOpZFailF != zfail || state.stencilOpZPassF != zpass)
		{
			state.stencilOpFailF = fail;
			state.stencilOpZFailF = zfail;
			state.stencilOpZPassF = zpass;

			state.stencilChanged = true;
		}
		state.stencilOpSeparate = fail != state.stencilOpFailB || zfail != state.stencilOpZFailB || zpass != state.stencilOpZPassB;
	}
	else if (face == iGL_BACK)
	{
		if (state.stencilOpFailB != fail || state.stencilOpZFailB != zfail || state.stencilOpZPassB != zpass)
		{
			state.stencilOpFailB = fail;
			state.stencilOpZFailB = zfail;
			state.stencilOpZPassB = zpass;

			state.stencilChanged = true;
		}
		state.stencilOpSeparate = fail != state.stencilOpFailF || zfail != state.stencilOpZFailF || zpass != state.stencilOpZPassF;
	}
	else if (face == iGL_FRONT_AND_BACK)
	{
		if (state.stencilOpFailF != fail || state.stencilOpFailB != fail ||
			state.stencilOpZFailF != zfail || state.stencilOpZFailB != zfail ||
			state.stencilOpZPassF != zpass || state.stencilOpZPassB != zpass)
		{
			state.stencilOpFailF = fail;
			state.stencilOpFailB = fail;
			state.stencilOpZFailF = zfail;
			state.stencilOpZFailB = zfail;
			state.stencilOpZPassF = zpass;
			state.stencilOpZPassB = zpass;

			state.stencilChanged = true;
		}
		state.stencilOpSeparate = false;
	}
}
void Interface::iglTexImage2D(uint32_t target, int level, int internalformat, int width, int height, int border, uint32_t format, uint32_t type, const void* pixels)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_texture_rebinding();
#endif
}
void Interface::iglTexParameterf(uint32_t target, uint32_t pname, float param)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	glTexParameterf(target, pname, param);
	iassertNoError();
}
void Interface::iglTexParameterfv(uint32_t target, uint32_t pname, const float* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	glTexParameterfv(target, pname, params);
	iassertNoError();
}
void Interface::iglTexParameteri(uint32_t target, uint32_t pname, int param)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	glTexParameteri(target, pname, param);
	iassertNoError();
}
void Interface::iglTexParameteriv(uint32_t target, uint32_t pname, const int* params)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	glTexParameteriv(target, pname, params);
	iassertNoError();
}
void Interface::iglTexSubImage2D(uint32_t target, int level, int xoffset, int yoffset, int width, int height, uint32_t format, uint32_t type, const void* pixels)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
	iassertNoError();
#ifdef Q_USE_RENDER_THREAD
	interf->force_texture_rebinding();
#endif
}
void Interface::iglUniform1f(int location, float x)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform1f(location, x));
	iassertNoError();
}
void Interface::iglUniform1fv(int location, int count, const float* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform1fv(location, count, values));
	iassertNoError();
}
void Interface::iglUniform1i(int location, int x)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform1i(location, x));
	iassertNoError();
}
void Interface::iglUniform1iv(int location, int count, const int* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform1iv(location, count, values));
	iassertNoError();
}
void Interface::iglUniform2f(int location, float x, float y)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform2f(location, x, y));
	iassertNoError();
}
void Interface::iglUniform2fv(int location, int count, const float* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform2fv(location, count, values));
	iassertNoError();
}
void Interface::iglUniform2i(int location, int x, int y)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform2i(location, x, y));
	iassertNoError();
}
void Interface::iglUniform2iv(int location, int count, const int* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform2iv(location, count, values));
	iassertNoError();
}
void Interface::iglUniform3f(int location, float x, float y, float z)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform3f(location, x, y, z));
	iassertNoError();
}
void Interface::iglUniform3fv(int location, int count, const float* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform3fv(location, count, values));
	iassertNoError();
}
void Interface::iglUniform3i(int location, int x, int y, int z)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform3i(location, x, y, z));
	iassertNoError();
}
void Interface::iglUniform3iv(int location, int count, const int* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform3iv(location, count, values));
	iassertNoError();
}
void Interface::iglUniform4f(int location, float x, float y, float z, float w)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform4f(location, x, y, z, w));
	iassertNoError();
}
void Interface::iglUniform4fv(int location, int count, const float* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform4fv(location, count, values));
	iassertNoError();
}
void Interface::iglUniform4i(int location, int x, int y, int z, int w)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform4i(location, x, y, z, w));
	iassertNoError();
}
void Interface::iglUniform4iv(int location, int count, const int* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniform4iv(location, count, values));
	iassertNoError();
}
void Interface::iglUniformMatrix2fv(gles::iGLint location, gles::iGLsizei count, gles::iGLboolean transpose, const gles::iGLfloat* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniformMatrix2fv(location, count, transpose, values));
	iassertNoError();
}
void Interface::iglUniformMatrix3fv(gles::iGLint location, gles::iGLsizei count, gles::iGLboolean transpose, const gles::iGLfloat* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniformMatrix3fv(location, count, transpose, values));
	iassertNoError();
}
void Interface::iglUniformMatrix4fv(gles::iGLint location, gles::iGLsizei count, gles::iGLboolean transpose, const gles::iGLfloat* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glUniformMatrix4fv(location, count, transpose, values));
	iassertNoError();
}
void Interface::iglUseProgram(uint32_t program)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	state.program = program;

// 	if (!s_enableFullCaching)
// 	{
// 		TLInterface* interf = GetInterface();
// 		interf->flush();
// #if defined(Q_WINDOWS)
// 		interf->_glUseProgram(program);
// #else
// 		::glUseProgram(program);
// #endif
// 		iassertNoError();
// 	}
}
void Interface::iglValidateProgram(uint32_t program)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glValidateProgram(program));
	iassertNoError();
}
void Interface::iglVertexAttrib1f(uint32_t index, float x)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glVertexAttrib1f(index, x));
	iassertNoError();
}
void Interface::iglVertexAttrib1fv(uint32_t index, const float* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glVertexAttrib1fv(index, values));
	iassertNoError();
}
void Interface::iglVertexAttrib2f(uint32_t index, float x, float y)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glVertexAttrib2f(index, x, y));
	iassertNoError();
}
void Interface::iglVertexAttrib2fv(uint32_t index, const float* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glVertexAttrib2fv(index, values));
	iassertNoError();
}
void Interface::iglVertexAttrib3f(uint32_t index, float x, float y, float z)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glVertexAttrib3f(index, x, y, z));
	iassertNoError();
}
void Interface::iglVertexAttrib3fv(uint32_t index, const float* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glVertexAttrib3fv(index, values));
	iassertNoError();
}
void Interface::iglVertexAttrib4f(uint32_t index, float x, float y, float z, float w)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glVertexAttrib4f(index, x, y, z, w));
	iassertNoError();
}
void Interface::iglVertexAttrib4fv(uint32_t index, const float* values)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glVertexAttrib4fv(index, values));
	iassertNoError();
}
void Interface::iglVertexAttribPointer(gles::iGLuint index, gles::iGLint size, gles::iGLenum type, gles::iGLboolean normalized, gles::iGLsizei stride, const void* ptr)
{
	TL_Interface* interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glVertexAttribPointer(index, size, type, normalized, stride, ptr));
	iassertNoError();
}
void Interface::iglViewport(int x, int y, int width, int height)
{
	TL_Interface* interf = get_interface();
	TL_Interface::GL_State& state = interf->m_crt_state;
	math::vec4s32 vp(x, y, width, height);
	if (vp != state.viewport)
	{
		state.viewport = vp;
		state.flagsChanged = true;
	}
}

iGLvoid* Interface::iglMapBuffer(iGLenum target, iGLenum access)
{
	TL_Interface* interf = get_interface();
	interf->flush();
#if defined(Q_WINDOWS)
	auto ret = interf->m_funcs->_glMapBuffer(target, access);
#else
	auto ret = ::glMapBufferOES(target, access);
#endif
	iassertNoError();
	return ret;
}
iGLboolean Interface::iglUnmapBuffer(iGLenum target)
{
	TL_Interface* interf = get_interface();
	//TLInterface::iGLState& state = interf->m_crtState;
	interf->flush();

#ifdef Q_USE_RENDER_THREAD
	if (target == iGL_ARRAY_BUFFER)
		interf->force_array_buffer_rebinding(state.bindArrayBuffer);
	else if (target == iGL_ELEMENT_ARRAY_BUFFER)
		interf->force_array_buffer_rebinding(state.bindElementBuffer);
#endif

#if defined(Q_WINDOWS)
	auto ret = interf->m_funcs->_glUnmapBuffer(target);
#else
	auto ret = ::glUnmapBufferOES(target);
#endif
	iassertNoError();
	return ret;
}
/*void Interface::iglBindVertexArray(iGLuint array)
{
	TLInterface* interf = GetInterface();
	TLInterface::iGLState& state = interf->m_crtState;
	interf->flush();

	if (state.bindVertexArray != array)
	{
		state.bindVertexArray = array;
#if defined(Q_WINDOWS)
		GL_FUN2(interf, glBindVertexArray(array));
#elif defined(Q_ANDROID)
		GL_FUN2_ANDROID(interf, glBindVertexArray(array));
#else
		glBindVertexArrayOES(array);
#endif
		iassertNoError();
	}
}
void Interface::iglDeleteVertexArrays(iGLsizei n, const iGLuint* arrays)
{
	TLInterface* interf = GetInterface();
	interf->flush();
#if defined(Q_WINDOWS)
	GL_FUN2(interf, glDeleteVertexArrays(n, arrays));
#elif defined(Q_ANDROID)
	GL_FUN2_ANDROID(interf, glDeleteVertexArrays(n, arrays));
#else
	glDeleteVertexArraysOES(n, arrays);
#endif
	iassertNoError();
}
void Interface::iglGenVertexArrays(iGLsizei n, iGLuint* arrays)
{
	TLInterface* interf = GetInterface();
	interf->flush();
#if defined(Q_WINDOWS)
	GL_FUN2(interf, glGenVertexArrays(n, arrays));
#elif defined(Q_ANDROID)
	GL_FUN2_ANDROID(interf, glGenVertexArrays(n, arrays));
#else
	glGenVertexArraysOES(n, arrays);
#endif
	iassertNoError();
}
iGLboolean Interface::iglIsVertexArray(iGLuint array)
{
	TLInterface* interf = GetInterface();
	interf->flush();
#if defined(Q_WINDOWS)
	iGLboolean res = GL_FUN2(interf, glIsVertexArray(array));
#elif defined(Q_ANDROID)
	iGLboolean res = GL_FUN2_ANDROID(interf, glIsVertexArray(array));
#else
	iGLboolean res = glIsVertexArrayOES(array);
#endif
	iassertNoError();
	return res;
}
*/

//////////////////////////////////////////////////////////////////////////
// New helper functions

const char* Interface::iglGetErrorString()
{
	iGLenum errCode;
	const iGLubyte *errString = 0;
	if ((errCode = ::glGetError()) != iGL_NO_ERROR)
	{
#if defined(Q_WINDOWS)
		errString = gluErrorString(errCode);
#else
		static char stringBuffer[128];
		sprintf(stringBuffer, "\nGeneric OpenGL Error code %d.", errCode);
		errString = (const iGLubyte*)stringBuffer;
#endif
	}
	return (const char*)errString;
}

bool Interface::icheckError()
{
#ifdef _DEBUG
	TL_Interface* interf = get_interface();
	interf->flush();

	GLenum errCode = glGetError();
	if (errCode == GL_NO_ERROR)
	{
		return false;
	}

	return true;
#else
	return false;
#endif
}

bool Interface::ihasExtension(const char* extension)
{
	QASSERT(extension);
	if (!extension)
	{
		return false;
	}

	TL_Interface* interf = get_interface();
	QASSERT(interf);
	if (!interf)
	{
		return false;
	}

	const char* all = interf->get_extensions();
	if (!all)
	{
		return false;
	}

	const size_t extlen = strlen(extension);

	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = all; ; p++)
	{
		// Advance p Up To The Next Possible Match
		p = q::util::stristr(p, extension);
		if (!p)
		{
			return false;  // No Match
		}

		// Make Sure That Match Is At The Start Of The String Or That
		// The Previous Char Is A Space, Or Else We Could Accidentally
		// Match "wglFunkywglExtension" With "wglExtension"
		// Also, Make Sure That The Following Character Is Space Or NULL
		// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p == all || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' '))
		{
			return true; // Match
		}
	}
	return false;
}
bool Interface::ihasExtensionPart(const char* extensionPart)
{
	QASSERT(extensionPart);
	if (!extensionPart)
	{
		return false;
	}

	TL_Interface* interf = get_interface();
	QASSERT(interf);
	if (!interf)
	{
		return false;
	}

	const char* all = interf->get_extensions();
	if (!all)
	{
		return false;
	}

	return q::util::stristr(all, extensionPart) != 0;
}

void Interface::iflush()
{
	TL_Interface* interf = get_interface();
	QASSERT(interf);
	if (!interf)
	{
		return;
	}

    interf->flush();
}


#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <windows.h>
#  define GET_PROC_ADDRESS(name) wglGetProcAddress((LPCSTR)name)
#else
#  if defined(__APPLE__)
#    define GET_PROC_ADDRESS(name) NSGLGetProcAddress(name)
#  elif defined(__ANDROID__)
#    include <EGL/egl.h>
//#    define GET_PROC_ADDRESS(name) (*eglGetProcAddress)(name)
#    define GET_PROC_ADDRESS(name) 0
#  else
#    define GET_PROC_ADDRESS(name) (*glXGetProcAddressARB)(name)
#  endif
#endif

#define STRINGIFY(x)  #x
#define GET_PROC(t, p) m_funcs->_##p = (t)GET_PROC_ADDRESS(STRINGIFY(p))

static void assertNoError()
{
#if defined(_DEBUG)

	iGLenum errCode;
	const char* errString = 0;
	if ((errCode = ::glGetError()) != iGL_NO_ERROR)
	{
#	if defined(Q_WINDOWS)
		errString = (const char*)gluErrorString(errCode);
#	else
		static char stringBuffer[128];
		sprintf(stringBuffer, "\nGeneric OpenGL Error code %d.", errCode);
		errString = (const char*)stringBuffer;
#	endif
	}

	QASSERT_MSG(!errString, errString);
#endif
}

TL_Interface::TL_Interface()
{
}
TL_Interface::~TL_Interface()
{
}

const char* TL_Interface::get_extensions() const
{
	return m_extensions.c_str();
}

void TL_Interface::internal_load()
{
	if (m_extensions.empty())
	{
		const char* ext = (const char*)glGetString(GL_EXTENSIONS);
		assertNoError();
		QASSERT(ext);
		if (ext)
		{
			m_extensions = ext;
		}
	}
	//PDBG("\nExtensions = {}", extensions);

	m_funcs = new GL_Ext_Functions;

#if defined(_WIN32)
	GET_PROC(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC, glRenderbufferStorageMultisample);
	GET_PROC(PFNGLBLITFRAMEBUFFERPROC, glBlitFramebuffer);
	GET_PROC(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus);
	GET_PROC(PFNGLCLEARDEPTHFPROC, glClearDepthf);
	GET_PROC(PFNGLCOMPRESSEDTEXIMAGE2DPROC, glCompressedTexImage2D);
	GET_PROC(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC, glCompressedTexSubImage2D);
	GET_PROC(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
	GET_PROC(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer);
	GET_PROC(PFNGLACTIVETEXTUREPROC, glActiveTexture);
	GET_PROC(PFNGLBLENDCOLORPROC, glBlendColor);
	GET_PROC(PFNGLBLENDEQUATIONPROC, glBlendEquation);
	GET_PROC(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate);
	GET_PROC(PFNGLBINDBUFFERPROC, glBindBuffer);
	GET_PROC(PFNGLBUFFERDATAPROC, glBufferData);
	GET_PROC(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
	GET_PROC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
	GET_PROC(PFNGLGENBUFFERSPROC, glGenBuffers);
	GET_PROC(PFNGLGETBUFFERPARAMETERIVPROC, glGetBufferParameteriv);
	GET_PROC(PFNGLGETBUFFERPOINTERVPROC, glGetBufferPointerv);
	GET_PROC(PFNGLGETBUFFERSUBDATAPROC, glGetBufferSubData);
	GET_PROC(PFNGLISBUFFERPROC, glIsBuffer);
	GET_PROC(PFNGLMAPBUFFERPROC, glMapBuffer);
	GET_PROC(PFNGLUNMAPBUFFERPROC, glUnmapBuffer);
	GET_PROC(PFNGLATTACHSHADERPROC, glAttachShader);
	GET_PROC(PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation);
	GET_PROC(PFNGLBLENDEQUATIONSEPARATEPROC, glBlendEquationSeparate);
	GET_PROC(PFNGLCOMPILESHADERPROC, glCompileShader);
	GET_PROC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
	GET_PROC(PFNGLCREATESHADERPROC, glCreateShader);
	GET_PROC(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
	GET_PROC(PFNGLDELETESHADERPROC, glDeleteShader);
	GET_PROC(PFNGLDETACHSHADERPROC, glDetachShader);
	GET_PROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
	GET_PROC(PFNGLDRAWBUFFERSPROC, glDrawBuffers);
	GET_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
	GET_PROC(PFNGLGETACTIVEATTRIBPROC, glGetActiveAttrib);
	GET_PROC(PFNGLGETACTIVEUNIFORMPROC, glGetActiveUniform);
	GET_PROC(PFNGLGETATTACHEDSHADERSPROC, glGetAttachedShaders);
	GET_PROC(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
	GET_PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
	GET_PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
	GET_PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
	GET_PROC(PFNGLGETSHADERSOURCEPROC, glGetShaderSource);
	GET_PROC(PFNGLGETSHADERIVPROC, glGetShaderiv);
	GET_PROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
	GET_PROC(PFNGLGETUNIFORMFVPROC, glGetUniformfv);
	GET_PROC(PFNGLGETUNIFORMIVPROC, glGetUniformiv);
	GET_PROC(PFNGLGETVERTEXATTRIBPOINTERVPROC, glGetVertexAttribPointerv);
	GET_PROC(PFNGLGETVERTEXATTRIBFVPROC, glGetVertexAttribfv);
	GET_PROC(PFNGLISPROGRAMPROC, glIsProgram);
	GET_PROC(PFNGLISSHADERPROC, glIsShader);
	GET_PROC(PFNGLLINKPROGRAMPROC, glLinkProgram);
	GET_PROC(PFNGLSHADERSOURCEPROC, glShaderSource);
	GET_PROC(PFNGLSTENCILFUNCSEPARATEPROC, glStencilFuncSeparate);
	GET_PROC(PFNGLSTENCILMASKSEPARATEPROC, glStencilMaskSeparate);
	GET_PROC(PFNGLSTENCILOPSEPARATEPROC, glStencilOpSeparate);
	GET_PROC(PFNGLUNIFORM1FPROC, glUniform1f);
	GET_PROC(PFNGLUNIFORM1FVPROC, glUniform1fv);
	GET_PROC(PFNGLUNIFORM1IPROC, glUniform1i);
	GET_PROC(PFNGLUNIFORM1IVPROC, glUniform1iv);
	GET_PROC(PFNGLUNIFORM2FPROC, glUniform2f);
	GET_PROC(PFNGLUNIFORM2FVPROC, glUniform2fv);
	GET_PROC(PFNGLUNIFORM2IPROC, glUniform2i);
	GET_PROC(PFNGLUNIFORM2IVPROC, glUniform2iv);
	GET_PROC(PFNGLUNIFORM3FPROC, glUniform3f);
	GET_PROC(PFNGLUNIFORM3FVPROC, glUniform3fv);
	GET_PROC(PFNGLUNIFORM3IPROC, glUniform3i);
	GET_PROC(PFNGLUNIFORM3IVPROC, glUniform3iv);
	GET_PROC(PFNGLUNIFORM4FPROC, glUniform4f);
	GET_PROC(PFNGLUNIFORM4FVPROC, glUniform4fv);
	GET_PROC(PFNGLUNIFORM4IPROC, glUniform4i);
	GET_PROC(PFNGLUNIFORM4IVPROC, glUniform4iv);
	GET_PROC(PFNGLUNIFORMMATRIX2FVPROC, glUniformMatrix2fv);
	GET_PROC(PFNGLUNIFORMMATRIX3FVPROC, glUniformMatrix3fv);
	GET_PROC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
	GET_PROC(PFNGLUSEPROGRAMPROC, glUseProgram);
	GET_PROC(PFNGLVALIDATEPROGRAMPROC, glValidateProgram);
	GET_PROC(PFNGLVERTEXATTRIB1FPROC, glVertexAttrib1f);
	GET_PROC(PFNGLVERTEXATTRIB1FVPROC, glVertexAttrib1fv);
	GET_PROC(PFNGLVERTEXATTRIB2FPROC, glVertexAttrib2f);
	GET_PROC(PFNGLVERTEXATTRIB2FVPROC, glVertexAttrib2fv);
	GET_PROC(PFNGLVERTEXATTRIB3FPROC, glVertexAttrib3f);
	GET_PROC(PFNGLVERTEXATTRIB3FVPROC, glVertexAttrib3fv);
	GET_PROC(PFNGLVERTEXATTRIB4FPROC, glVertexAttrib4f);
	GET_PROC(PFNGLVERTEXATTRIB4FVPROC, glVertexAttrib4fv);
	GET_PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
	GET_PROC(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers);
	GET_PROC(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers);
	GET_PROC(PFNGLDEPTHRANGEFPROC, glDepthRangef);
	GET_PROC(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer);
	GET_PROC(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
	GET_PROC(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap);
	GET_PROC(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
	GET_PROC(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers);
	GET_PROC(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC, glGetFramebufferAttachmentParameteriv);
	GET_PROC(PFNGLGETRENDERBUFFERPARAMETERIVPROC, glGetRenderbufferParameteriv);
	GET_PROC(PFNGLGETSHADERPRECISIONFORMATPROC, glGetShaderPrecisionFormat);
	GET_PROC(PFNGLGETUNIFORMFVPROC, glGetUniformfv);
	GET_PROC(PFNGLGETUNIFORMIVPROC, glGetUniformiv);
	GET_PROC(PFNGLGETVERTEXATTRIBIVPROC, glGetVertexAttribiv);
	GET_PROC(PFNGLISFRAMEBUFFERPROC, glIsFramebuffer);
	GET_PROC(PFNGLISRENDERBUFFERPROC, glIsRenderbuffer);
	GET_PROC(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage);
	GET_PROC(PFNGLRELEASESHADERCOMPILERPROC, glReleaseShaderCompiler);
	GET_PROC(PFNGLSAMPLECOVERAGEPROC, glSampleCoverage);
	GET_PROC(PFNGLSHADERBINARYPROC, glShaderBinary);
    GET_PROC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
	GET_PROC(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
	GET_PROC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
	GET_PROC(PFNGLISVERTEXARRAYPROC, glIsVertexArray);
	//GET_PROC(PFNGLDISCARDFRAMEBUFFEREXTPROC, glDiscardFramebuffer);
#endif
}

void TL_Interface::sync()
{
	Interface interf;

	//void	glActiveTexture(uint32_t texture);
	glGetIntegerv(iGL_ACTIVE_TEXTURE, reinterpret_cast<GLint*>(&m_crt_state.activeTexture));
	assertNoError();

	//void	glBindBuffer(uint32_t target, uint32_t buffer);
	glGetIntegerv(iGL_ARRAY_BUFFER_BINDING, reinterpret_cast<GLint*>(&m_crt_state.bindArrayBuffer));
	assertNoError();
	glGetIntegerv(iGL_ELEMENT_ARRAY_BUFFER_BINDING, reinterpret_cast<GLint*>(&m_crt_state.bindElementBuffer));
	assertNoError();

	//if (interf.ihasExtensionPart("vertex_array_object"))//don't do it because it calls flush
	if (m_extensions.find("OES_vertex_array_object ") != String::npos)
	{
		glGetIntegerv(iGL_VERTEX_ARRAY_BINDING, reinterpret_cast<GLint*>(&m_crt_state.bindVertexArray));
		assertNoError();
	}

	//void	glBindFramebuffer(uint32_t target, uint32_t framebuffer);
	glGetIntegerv(iGL_FRAMEBUFFER_BINDING, reinterpret_cast<GLint*>(&m_crt_state.bindFramebuffer));
	assertNoError();

	//void	glBindRenderbuffer(uint32_t target, uint32_t renderbuffer);
	glGetIntegerv(iGL_RENDERBUFFER_BINDING, reinterpret_cast<GLint*>(&m_crt_state.bindRenderbuffer));
	assertNoError();

	//void	glBindTexture(uint32_t target, uint32_t texture);
	int maxTextures = 8;
	glGetIntegerv(iGL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextures);
	assertNoError();
	m_crt_state.bindTextures.resize(maxTextures);

	maxTextures = std::min(32, maxTextures);//160 in a GTS 450
	m_crt_state.bindTextures.resize(maxTextures);
	QASSERT(maxTextures <= 32);
	for (int i = 0; i < maxTextures; i++)
	{
		GL_FUN(glActiveTexture(iGL_TEXTURE0 + i));
		assertNoError();
		int bind2d = 0;
		int bindCubeMap = 0;
		glGetIntegerv(iGL_TEXTURE_BINDING_2D, &bind2d);
		assertNoError();
		glGetIntegerv(iGL_TEXTURE_BINDING_CUBE_MAP, &bindCubeMap);
		assertNoError();
		if (bindCubeMap > 0 && bind2d <= 0)
		{
			m_crt_state.bindTextures[i] = std::make_pair(bindCubeMap, true);
		}
		else
		{
			m_crt_state.bindTextures[i] = std::make_pair(bind2d, false);
		}
	}
	GL_FUN(glActiveTexture(m_crt_state.activeTexture));
	assertNoError();
	m_crt_state.bindTexturesLastChangedIdx = -1;

	//void	glBlendColor(float red, float green, float blue, float alpha);
	glGetFloatv(iGL_BLEND_COLOR, reinterpret_cast<GLfloat*>(&m_crt_state.blendColor));
	assertNoError();

	//void	glBlendEquationSeparate(uint32_t modeRGB, uint32_t modeAlpha);
	glGetIntegerv(iGL_BLEND_EQUATION_RGB, reinterpret_cast<GLint*>(&m_crt_state.blendEqRGB));
	assertNoError();
	glGetIntegerv(iGL_BLEND_EQUATION_ALPHA, reinterpret_cast<GLint*>(&m_crt_state.blendEqAlpha));
	assertNoError();
	m_crt_state.blendEqSeparate = true;

	//void	glBlendFunc(uint32_t sfactor, uint32_t dfactor);
	//void	glBlendFuncSeparate(uint32_t srcRGB, uint32_t dstRGB, uint32_t srcAlpha, uint32_t dstAlpha);
	glGetIntegerv(iGL_BLEND_SRC_RGB, reinterpret_cast<GLint*>(&m_crt_state.blendFuncSrcRGB));
	assertNoError();
	glGetIntegerv(iGL_BLEND_DST_RGB, reinterpret_cast<GLint*>(&m_crt_state.blendFuncDstRGB));
	assertNoError();
	glGetIntegerv(iGL_BLEND_SRC_ALPHA, reinterpret_cast<GLint*>(&m_crt_state.blendFuncSrcAlpha));
	assertNoError();
	glGetIntegerv(iGL_BLEND_DST_ALPHA, reinterpret_cast<GLint*>(&m_crt_state.blendFuncDstAlpha));
	assertNoError();
	m_crt_state.blendFuncSeparate = true;

	//void	glClearColor(float red, float green, float blue, float alpha);
	glGetFloatv(iGL_COLOR_CLEAR_VALUE, reinterpret_cast<GLfloat*>(&m_crt_state.clearColor));
	assertNoError();

	//void	glClearDepthf(float depth);
	glGetFloatv(iGL_DEPTH_CLEAR_VALUE, &m_crt_state.clearDepth);
	assertNoError();

	//void	glClearStencil(int s);
	glGetIntegerv(iGL_STENCIL_CLEAR_VALUE, &m_crt_state.clearStencil);
	assertNoError();

	//void	glColorMask(glbool red, glbool green, glbool blue, glbool alpha);
	glGetBooleanv(iGL_COLOR_WRITEMASK, reinterpret_cast<GLboolean*>(&m_crt_state.colorMask));
	assertNoError();

	//void	glCullFace(uint32_t mode);
	glGetIntegerv(iGL_CULL_FACE_MODE, reinterpret_cast<GLint*>(&m_crt_state.cullFace));
	assertNoError();

	//void	glDepthFunc(uint32_t func);
	glGetIntegerv(iGL_DEPTH_FUNC, reinterpret_cast<GLint*>(&m_crt_state.depthFunc));
	assertNoError();

	//void	glDepthMask(glbool flag);
	glGetBooleanv(iGL_DEPTH_WRITEMASK, &m_crt_state.depthMask);
	assertNoError();

	//void	glDepthRangef(float zNear, float zFar);
	float v[2];
	glGetFloatv(iGL_DEPTH_RANGE, v);
	assertNoError();
	m_crt_state.depthNear = v[0];
	m_crt_state.depthFar = v[1];

	//void	glDisable(uint32_t cap);
	//void	glEnable(uint32_t cap);
	glGetBooleanv(iGL_BLEND, &m_crt_state.capBlend);
	assertNoError();
	glGetBooleanv(iGL_CULL_FACE, &m_crt_state.capCullFace);
	assertNoError();
	glGetBooleanv(iGL_DEPTH_TEST, &m_crt_state.capDepthTest);
	assertNoError();
	glGetBooleanv(iGL_DITHER, &m_crt_state.capDither);
	assertNoError();
	glGetBooleanv(iGL_POLYGON_OFFSET_FILL, &m_crt_state.capPolygonOffset);
	assertNoError();
	glGetBooleanv(iGL_SAMPLE_ALPHA_TO_COVERAGE, &m_crt_state.capSampleAlphaToCoverage);
	assertNoError();
	glGetBooleanv(iGL_SAMPLE_COVERAGE, &m_crt_state.capSampleCoverage);
	assertNoError();
	glGetBooleanv(iGL_SCISSOR_TEST, &m_crt_state.capScissorTest);
	assertNoError();
	glGetBooleanv(iGL_STENCIL_TEST, &m_crt_state.capStencilTest);
	assertNoError();
	m_crt_state.capsChanged = false;

	//void	glFrontFace(uint32_t mode);
	glGetIntegerv(iGL_FRONT_FACE, reinterpret_cast<GLint*>(&m_crt_state.frontFace));
	assertNoError();

	//void	glHint(uint32_t target, uint32_t mode);
	//glGetIntegerv(iGL_GENERATE_MIPMAP_HINT, reinterpret_cast<GLint*>(&m_crtState.hintGenerateMipmap));
	//assertNoError();

	//void	glLineWidth(float width);
	glGetFloatv(iGL_LINE_WIDTH, &m_crt_state.lineWidth);
	assertNoError();

	//void	glPolygonOffset(float factor, float units);
	glGetFloatv(iGL_POLYGON_OFFSET_FACTOR, &m_crt_state.polygonOffsetFactor);
	assertNoError();
	glGetFloatv(iGL_POLYGON_OFFSET_UNITS, &m_crt_state.polygonOffsetUnits);
	assertNoError();

	//void	glSampleCoverage(float value, glbool invert);
	glGetFloatv(iGL_SAMPLE_COVERAGE_VALUE, &m_crt_state.sampleCoverageValue);
	assertNoError();
	glGetBooleanv(iGL_SAMPLE_COVERAGE_INVERT, &m_crt_state.sampleCoverageInvert);
	assertNoError();

	//void	glScissor(int x, int y, int width, int height);
	glGetIntegerv(iGL_SCISSOR_BOX, reinterpret_cast<GLint*>(&m_crt_state.scissorsBox));
	assertNoError();

	//void	glStencilFuncSeparate(uint32_t face, uint32_t func, int ref, uint32_t mask);
	glGetIntegerv(iGL_STENCIL_FUNC, reinterpret_cast<GLint*>(&m_crt_state.stencilFuncF));
	assertNoError();
	glGetIntegerv(iGL_STENCIL_REF, &m_crt_state.stencilFuncRefF);
	assertNoError();
	glGetIntegerv(iGL_STENCIL_VALUE_MASK, reinterpret_cast<GLint*>(&m_crt_state.stencilFuncMaskF));
	assertNoError();
	glGetIntegerv(iGL_STENCIL_BACK_FUNC, reinterpret_cast<GLint*>(&m_crt_state.stencilFuncB));
	assertNoError();
	glGetIntegerv(iGL_STENCIL_BACK_REF, &m_crt_state.stencilFuncRefB);
	assertNoError();
	glGetIntegerv(iGL_STENCIL_BACK_VALUE_MASK, reinterpret_cast<GLint*>(&m_crt_state.stencilFuncMaskB));
	assertNoError();
	m_crt_state.stencilFuncSeparate = true;

	//void	glStencilMaskSeparate(uint32_t face, uint32_t mask);
	glGetIntegerv(iGL_STENCIL_WRITEMASK, reinterpret_cast<GLint*>(&m_crt_state.stencilMaskF));
	assertNoError();
	glGetIntegerv(iGL_STENCIL_BACK_WRITEMASK, reinterpret_cast<GLint*>(&m_crt_state.stencilMaskB));
	assertNoError();
	m_crt_state.stencilMaskSeparate = true;

	//void	glStencilOpSeparate(uint32_t face, uint32_t fail, uint32_t zfail, uint32_t zpass);
	glGetIntegerv(iGL_STENCIL_FAIL, reinterpret_cast<GLint*>(&m_crt_state.stencilOpFailF));
	assertNoError();
	glGetIntegerv(iGL_STENCIL_PASS_DEPTH_FAIL, reinterpret_cast<GLint*>(&m_crt_state.stencilOpZFailF));
	assertNoError();
	glGetIntegerv(iGL_STENCIL_PASS_DEPTH_PASS, reinterpret_cast<GLint*>(&m_crt_state.stencilOpZPassF));
	assertNoError();
	glGetIntegerv(iGL_STENCIL_BACK_FAIL, reinterpret_cast<GLint*>(&m_crt_state.stencilOpFailB));
	assertNoError();
	glGetIntegerv(iGL_STENCIL_BACK_PASS_DEPTH_FAIL, reinterpret_cast<GLint*>(&m_crt_state.stencilOpZFailB));
	assertNoError();
	glGetIntegerv(iGL_STENCIL_BACK_PASS_DEPTH_PASS, reinterpret_cast<GLint*>(&m_crt_state.stencilOpZPassB));
	assertNoError();
	m_crt_state.stencilOpSeparate = true;

	//void	glUseProgram(uint32_t program);
	glGetIntegerv(iGL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&m_crt_state.program));
	assertNoError();

	//void	glViewport(int x, int y, int width, int height);
	glGetIntegerv(iGL_VIEWPORT, reinterpret_cast<GLint*>(&m_crt_state.viewport));
	assertNoError();

	glGetIntegerv(iGL_UNPACK_ALIGNMENT, &m_crt_state.pixelStoreUnbpackAlignment);
	assertNoError();

	int maxAttribs = 8;
	glGetIntegerv(iGL_MAX_VERTEX_ATTRIBS, &maxAttribs);
	assertNoError();
	m_crt_state.vertexAttribs.resize(maxAttribs);
	for (int i = 0; i < maxAttribs; i++)
	{
		int enabled;
		GL_FUN(glGetVertexAttribiv(i, iGL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled));
		assertNoError();
		m_crt_state.vertexAttribs[i] = enabled != 0 ? 1 : 0;
	}
	m_crt_state.vertexAttribsLastChangedIdx = -1;

	m_old_state = m_crt_state;

	m_crt_state.buffersChanged = true;
	m_crt_state.blendChanged = true;
	m_crt_state.clearChanged = true;
	m_crt_state.flagsChanged = true;
	m_crt_state.stencilChanged = true;
}

#define CRT(x) m_crt_state.x
#define OLD(x) m_old_state.x
#define EQ(x) (m_crt_state.x == m_old_state.x)
#define ASSIGN(x) { m_old_state.x = m_crt_state.x; }
#define EQ2(x, y) (m_crt_state.x == m_old_state.x && m_crt_state.y == m_old_state.y)
#define ASSIGN2(x, y) { m_old_state.x = m_crt_state.x; m_old_state.y = m_crt_state.y; }

#define CHECK_STATE(x, body)	\
if (!EQ(x))	\
{								\
	ASSIGN(x);	\
	body;						\
}
#define CHECK_STATE2(x, y, body)	\
if (!EQ2(x, y))	\
{								\
	ASSIGN2(x, y);	\
	body;						\
}

void TL_Interface::flush()
{
	assertNoError();

	if (m_crt_state.buffersChanged)
	{
		if (m_crt_state.bindVertexArray == 0)
		{
			if (m_crt_state.bindArrayBuffer != m_old_state.bindArrayBuffer)
			{
				GL_FUN(glBindBuffer(iGL_ARRAY_BUFFER, CRT(bindArrayBuffer)));
				assertNoError();
				ASSIGN(bindArrayBuffer);
			}
			if (m_crt_state.bindElementBuffer != m_old_state.bindElementBuffer)
			{
				GL_FUN(glBindBuffer(iGL_ELEMENT_ARRAY_BUFFER, CRT(bindElementBuffer)));
				assertNoError();
				ASSIGN(bindElementBuffer);
			}
		}

		//void	glBindBuffer(uint32_t target, uint32_t buffer);
		if (s_enableFullCaching)
		{

			//void	glBindFramebuffer(uint32_t target, uint32_t framebuffer);
			if (m_crt_state.bindFramebuffer != m_old_state.bindFramebuffer)
			{
				GL_FUN(glBindFramebuffer(iGL_FRAMEBUFFER, CRT(bindFramebuffer)));
				assertNoError();
				ASSIGN(bindFramebuffer);
			}

			//void	glBindRenderbuffer(uint32_t target, uint32_t renderbuffer);
			if (m_crt_state.bindRenderbuffer != m_old_state.bindRenderbuffer)
			{
				GL_FUN(glBindRenderbuffer(iGL_RENDERBUFFER, CRT(bindRenderbuffer)));
				assertNoError();
				ASSIGN(bindRenderbuffer);
			}
		}
	}

	//if (!s_enableFullCaching)
	{
		//don't break this block
		{
			//void	glBindTexture(uint32_t target, uint32_t texture);
			if (m_crt_state.bindTexturesLastChangedIdx >= 0)
			{
				for (uint32_t i = 0; i <= (uint32_t)m_crt_state.bindTexturesLastChangedIdx; i++)
				{
					std::pair<uint32_t, bool> const& bt = m_crt_state.bindTextures[i];
					if (bt != m_old_state.bindTextures[i])
					{
						uint32_t activeTexture = iGL_TEXTURE0 + i;
						if (m_old_state.activeTexture != activeTexture)
						{
							GL_FUN(glActiveTexture(activeTexture));
							assertNoError();
							m_old_state.activeTexture = activeTexture;
						}
						if (bt.second)
						{
							::glBindTexture(iGL_TEXTURE_CUBE_MAP, bt.first);
							assertNoError();
						}
						else
						{
							::glBindTexture(iGL_TEXTURE_2D, bt.first);
							assertNoError();
						}
						m_old_state.bindTextures[i] = bt;
					}
				}
				m_crt_state.bindTexturesLastChangedIdx = -1;
			}
			assertNoError();

			//put back the active texture - it was modified above
			CHECK_STATE(activeTexture, GL_FUN(glActiveTexture(CRT(activeTexture))));
			assertNoError();
		}
	}
 	//else
 	{
 	//	CHECK_STATE(activeTexture, GL_FUN(glActiveTexture(CRT(activeTexture))));
 	//	assertNoError();
 	}

	if (m_crt_state.blendChanged)
	{
		m_crt_state.blendChanged = false;
		//void	glBlendColor(float red, float green, float blue, float alpha);
		if (*(math::vec4s32*)&m_crt_state.blendColor != *(math::vec4s32*)&m_old_state.blendColor)
		{
			GL_FUN(glBlendColor(CRT(blendColor.x), CRT(blendColor.y), CRT(blendColor.z), CRT(blendColor.w)));
			assertNoError();
			m_old_state.blendColor = m_crt_state.blendColor;
		}

		//void	glBlendEquationSeparate(uint32_t modeRGB, uint32_t modeAlpha);
		if (CRT(blendEqSeparate))
		{
			if (!EQ(blendEqRGB) || !EQ(blendEqAlpha))
			{
				ASSIGN(blendEqRGB);
				ASSIGN(blendEqAlpha);
				GL_FUN(glBlendEquationSeparate(CRT(blendEqRGB), CRT(blendEqAlpha)));
				assertNoError();
			}
		}
		else
		{
			if (!EQ(blendEqRGB))
			{
				ASSIGN(blendEqRGB);
				GL_FUN(glBlendEquation(CRT(blendEqRGB)));
				assertNoError();
			}
		}
		//void	glBlendFunc(uint32_t sfactor, uint32_t dfactor);
		//void	glBlendFuncSeparate(uint32_t srcRGB, uint32_t dstRGB, uint32_t srcAlpha, uint32_t dstAlpha);
		if (CRT(blendFuncSeparate))
		{
			if (!EQ(blendFuncSrcRGB) || !EQ(blendFuncSrcAlpha) || !EQ(blendFuncDstRGB) || !EQ(blendFuncDstAlpha))
			{
				ASSIGN(blendFuncSrcRGB);
				ASSIGN(blendFuncSrcAlpha);
				ASSIGN(blendFuncDstRGB);
				ASSIGN(blendFuncDstAlpha);
				GL_FUN(glBlendFuncSeparate(CRT(blendFuncSrcRGB), CRT(blendFuncDstRGB), CRT(blendFuncSrcAlpha), CRT(blendFuncDstAlpha)));
				assertNoError();
			}
		}
		else
		{
			if (!EQ(blendFuncSrcRGB) || !EQ(blendFuncDstRGB))
			{
				ASSIGN(blendFuncSrcRGB);
				ASSIGN(blendFuncDstRGB);
				::glBlendFunc(CRT(blendFuncSrcRGB), CRT(blendFuncDstRGB));
				assertNoError();
			}
		}
	}

	if (m_crt_state.clearChanged)
	{
		m_crt_state.clearChanged = false;
		//void	glClearColor(float red, float green, float blue, float alpha);
		if (*(math::vec4s32*)&m_crt_state.clearColor != *(math::vec4s32*)&m_old_state.clearColor)
		{
			::glClearColor(CRT(clearColor.x), CRT(clearColor.y), CRT(clearColor.z), CRT(clearColor.w));
			assertNoError();
			m_old_state.clearColor = m_crt_state.clearColor;
		}

		//void	glClearDepthf(float depth);
		CHECK_STATE(clearDepth, GL_FUN(glClearDepthf(CRT(clearDepth))));
		assertNoError();

		//void	glClearStencil(int s);
		CHECK_STATE(clearStencil, ::glClearStencil(CRT(clearStencil)));
		assertNoError();
	}

	if (m_crt_state.flagsChanged)
	{
		m_crt_state.flagsChanged = false;
		//void	glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
		CHECK_STATE(colorMask, ::glColorMask(CRT(colorMask.x), CRT(colorMask.y), CRT(colorMask.z), CRT(colorMask.w)));
		assertNoError();

		//void	glCullFace(uint32_t mode);
		CHECK_STATE(cullFace, ::glCullFace(CRT(cullFace)));
		assertNoError();

		//void	glDepthFunc(uint32_t func);
		CHECK_STATE(depthFunc, ::glDepthFunc(CRT(depthFunc)));
		assertNoError();

		//void	glDepthMask(GLboolean flag);
		CHECK_STATE(depthMask, ::glDepthMask(CRT(depthMask)));
		assertNoError();

		//void	glDepthRangef(float zNear, float zFar);
		CHECK_STATE2(depthNear, depthFar, GL_FUN(glDepthRangef(CRT(depthNear), CRT(depthFar))));
		assertNoError();

		//void	glFrontFace(uint32_t mode);
		CHECK_STATE(frontFace, ::glFrontFace(CRT(frontFace)));
		assertNoError();

		//void	glHint(uint32_t target, uint32_t mode);
		//CHECK_STATE(hintGenerateMipmap, ::glHint(iGL_GENERATE_MIPMAP_HINT, CRT(hintGenerateMipmap)));
		//assertNoError();

		//void	glLineWidth(float width);
		CHECK_STATE(lineWidth, ::glLineWidth(CRT(lineWidth)));
		assertNoError();

		//void	glPolygonOffset(float factor, float units);
		CHECK_STATE2(polygonOffsetFactor, polygonOffsetUnits, glPolygonOffset(CRT(polygonOffsetFactor), CRT(polygonOffsetUnits)));
		assertNoError();

		//void	glSampleCoverage(float value, GLboolean invert);
		CHECK_STATE2(sampleCoverageValue, sampleCoverageInvert, GL_FUN(glSampleCoverage(CRT(sampleCoverageValue), CRT(sampleCoverageInvert))));
		assertNoError();

		//void	glSciior(int x, int y, int width, int height);
		CHECK_STATE(scissorsBox, ::glScissor(CRT(scissorsBox.x), CRT(scissorsBox.y), CRT(scissorsBox.z), CRT(scissorsBox.w)));
		assertNoError();

		//void	glViewport(int x, int y, int width, int height);
		CHECK_STATE(viewport, ::glViewport(CRT(viewport.x), CRT(viewport.y), CRT(viewport.z), CRT(viewport.w)));
		assertNoError();
	}
	else
	{
		//int a = 0;
	}

	//void	glDisable(uint32_t cap);
	//void	glEnable(uint32_t cap);
#define FLUSH_CAP(x, c)	\
if (!EQ(x))				\
{						\
	ASSIGN(x);			\
	if (CRT(x))			\
		glEnable(c);	\
	else				\
		glDisable(c);	\
}

	if (m_crt_state.capsChanged)
	{
		m_crt_state.capsChanged = false;
		FLUSH_CAP(capBlend, iGL_BLEND);
		FLUSH_CAP(capCullFace, iGL_CULL_FACE);
		FLUSH_CAP(capDepthTest, iGL_DEPTH_TEST);
		FLUSH_CAP(capDither, iGL_DITHER);
		FLUSH_CAP(capPolygonOffset, iGL_POLYGON_OFFSET_FILL);
		FLUSH_CAP(capSampleAlphaToCoverage, iGL_SAMPLE_ALPHA_TO_COVERAGE);
		FLUSH_CAP(capSampleCoverage, iGL_SAMPLE_COVERAGE);
		FLUSH_CAP(capScissorTest, iGL_SCISSOR_TEST);
		FLUSH_CAP(capStencilTest, iGL_STENCIL_TEST);
	}

	if (m_crt_state.stencilChanged)
	{
		m_crt_state.stencilChanged = false;
		//void	glStencilFuncSeparate(uint32_t face, uint32_t func, int ref, uint32_t mask);
		if (CRT(stencilFuncSeparate))
		{
			if (!EQ(stencilFuncF) || !EQ(stencilFuncRefF) || !EQ(stencilFuncMaskF))
			{
				ASSIGN(stencilFuncF);
				ASSIGN(stencilFuncRefF);
				ASSIGN(stencilFuncMaskF);
				GL_FUN(glStencilFuncSeparate(GL_FRONT, CRT(stencilFuncF), CRT(stencilFuncRefF), CRT(stencilFuncMaskF)));
				assertNoError();
			}
			if (!EQ(stencilFuncB) || !EQ(stencilFuncRefB) || !EQ(stencilFuncMaskB))
			{
				ASSIGN(stencilFuncB);
				ASSIGN(stencilFuncRefB);
				ASSIGN(stencilFuncMaskB);
				GL_FUN(glStencilFuncSeparate(GL_BACK, CRT(stencilFuncB), CRT(stencilFuncRefB), CRT(stencilFuncMaskB)));
				assertNoError();
			}
		}
		else
		{
			// when no separate, use variables for front
			if (!EQ(stencilFuncF) || !EQ(stencilFuncRefF) || !EQ(stencilFuncMaskF))
			{
				ASSIGN(stencilFuncF);
				ASSIGN(stencilFuncRefF);
				ASSIGN(stencilFuncMaskF);
				::glStencilFunc(CRT(stencilFuncF), CRT(stencilFuncRefF), CRT(stencilFuncMaskF));
				assertNoError();
			}
		}

		//void	glStencilMaskSeparate(uint32_t face, uint32_t mask);
		if (CRT(stencilMaskSeparate))
		{
			if (!EQ(stencilMaskF))
			{
				ASSIGN(stencilMaskF);
				GL_FUN(glStencilMaskSeparate(GL_FRONT, CRT(stencilMaskF)));
				assertNoError();
			}
			if (!EQ(stencilMaskB))
			{
				ASSIGN(stencilMaskB);
				GL_FUN(glStencilMaskSeparate(GL_BACK, CRT(stencilMaskB)));
				assertNoError();
			}
		}
		else
		{
			// when no separate, use variables for front
			if (!EQ(stencilMaskF))
			{
				ASSIGN(stencilMaskF);
				::glStencilMask(CRT(stencilMaskB));
				assertNoError();
			}
		}

		//void	glStencilOpSeparate(uint32_t face, uint32_t fail, uint32_t zfail, uint32_t zpass);
		if (CRT(stencilOpSeparate))
		{
			if (!EQ(stencilOpFailF) || !EQ(stencilOpZFailF) || !EQ(stencilOpZPassF))
			{
				ASSIGN(stencilOpFailF);
				ASSIGN(stencilOpZFailF);
				ASSIGN(stencilOpZPassF);
				GL_FUN(glStencilOpSeparate(GL_FRONT, CRT(stencilOpFailF), CRT(stencilOpZFailF), CRT(stencilOpZPassF)));
				assertNoError();
			}
			if (!EQ(stencilOpFailB) || !EQ(stencilOpZFailB) || !EQ(stencilOpZPassB))
			{
				ASSIGN(stencilOpFailB);
				ASSIGN(stencilOpZFailB);
				ASSIGN(stencilOpZPassB);
				GL_FUN(glStencilOpSeparate(GL_BACK, CRT(stencilOpFailB), CRT(stencilOpZFailB), CRT(stencilOpZPassB)));
				assertNoError();
			}
		}
		else
		{
			// when no separate, use variables for front
			if (!EQ(stencilOpFailF) || !EQ(stencilOpZFailF) || !EQ(stencilOpZPassF))
			{
				ASSIGN(stencilOpFailF);
				ASSIGN(stencilOpZFailF);
				ASSIGN(stencilOpZPassF);
				::glStencilOp(CRT(stencilOpFailF), CRT(stencilOpZFailF), CRT(stencilOpZPassF));
				assertNoError();
			}
		}
	}

	//void	glUseProgram(uint32_t program);
	//if (s_enableFullCaching)
	{
		if (m_crt_state.program != m_old_state.program)
		{
			GL_FUN(glUseProgram(CRT(program)));
			assertNoError();
			ASSIGN(program);
		}
	}

	//void	glDisableVertexAttribArray(uint32_t index);
	//void	glEnableVertexAttribArray(uint32_t index);
	//m_crtState.vertexAttribsLastChangedIdx = m_crtState.vertexAttribs.size() - 1;
	//if (s_enableFullCaching)
	{
		if (m_crt_state.vertexAttribsLastChangedIdx >= 0)
		{
			for (uint32_t i = 0; i <= (uint32_t)m_crt_state.vertexAttribsLastChangedIdx; i++)
			{
				uint8_t a = m_crt_state.vertexAttribs[i];
				if (a != m_old_state.vertexAttribs[i])
				{
					if (a)
					{
						GL_FUN(glEnableVertexAttribArray(i));
						assertNoError();
					}
					else
					{
						GL_FUN(glDisableVertexAttribArray(i));
						assertNoError();
					}
					m_old_state.vertexAttribs[i] = a;
				}
			}
			m_crt_state.vertexAttribsLastChangedIdx = -1;
		}
		assertNoError();
	}
//
// 	m_crtState.buffersChanged = false;
 	//m_crtState.blendChanged = false;
 	//m_crtState.clearChanged = false;
 	//m_crtState.flagsChanged = false;
 	//m_crtState.stencilChanged = false;
}

#undef CHECK_STATE
#undef CHECK_STATE2
#undef CRT
#undef OLD
#undef EQ
#undef ASSIGN
#undef EQ2
#undef ASSIGN2


void TL_Interface::force_array_buffer_rebinding(iGLuint buffer)
{
	for (uint32_t i = 0; i < s_threadInterfaces.size(); i++)
	{
		TL_Interface* interf = s_threadInterfaces[i];
		if (interf && interf != this)
		{
			if (interf->m_crt_state.bindArrayBuffer == buffer)
				interf->m_old_state.bindArrayBuffer = 0;
		}
	}
}
void TL_Interface::force_element_buffer_rebinding(iGLuint buffer)
{
	for (uint32_t i = 0; i < s_threadInterfaces.size(); i++)
	{
		TL_Interface* interf = s_threadInterfaces[i];
		if (interf && interf != this)
		{
			if (interf->m_crt_state.bindElementBuffer == buffer)
				interf->m_old_state.bindElementBuffer = 0;
		}
	}
}
void TL_Interface::force_framebuffer_rebinding(iGLuint buffer)
{
	for (uint32_t i = 0; i < s_threadInterfaces.size(); i++)
	{
		TL_Interface* interf = s_threadInterfaces[i];
		if (interf && interf != this)
		{
			if (interf->m_crt_state.bindFramebuffer == buffer)
				interf->m_old_state.bindFramebuffer = 0;
		}
	}
}
void TL_Interface::force_renderbuffer_rebinding(iGLuint buffer)
{
	for (uint32_t i = 0; i < s_threadInterfaces.size(); i++)
	{
		TL_Interface* interf = s_threadInterfaces[i];
		if (interf && interf != this)
		{
			if (interf->m_crt_state.bindRenderbuffer == buffer)
				interf->m_old_state.bindRenderbuffer = 0;
		}
	}
}
void TL_Interface::force_texture_rebinding()
{
	for (uint32_t i = 0; i < s_threadInterfaces.size(); i++)
	{
		TL_Interface* interf = s_threadInterfaces[i];
		if (interf && interf != this)
		{
			uint32_t size = interf->m_old_state.bindTextures.size();
			interf->m_old_state.bindTexturesLastChangedIdx = size - 1;
			for (uint32_t j = 0; j < size; j++)
				interf->m_old_state.bindTextures[j] = std::make_pair(0, false);
		}
	}
}
void TL_Interface::force_program_rebinding(iGLuint program)
{
	for (uint32_t i = 0; i < s_threadInterfaces.size(); i++)
	{
		TL_Interface* interf = s_threadInterfaces[i];
		if (interf && interf != this)
		{
			if (interf->m_crt_state.program == program)
				interf->m_old_state.program = 0;
		}
	}
}

void TL_Interface::remove_buffer(uint32_t buffer)
{
	for (uint32_t i = 0; i < s_threadInterfaces.size(); i++)
	{
		TL_Interface* interf = s_threadInterfaces[i];
		if (interf)
		{
			if (interf->m_crt_state.bindArrayBuffer == buffer)
				interf->m_crt_state.bindArrayBuffer = 0;
			if (interf->m_crt_state.bindElementBuffer == buffer)
				interf->m_crt_state.bindElementBuffer = 0;
		}
	}
}
void TL_Interface::remove_framebuffer(uint32_t buffer)
{
	for (uint32_t i = 0; i < s_threadInterfaces.size(); i++)
	{
		TL_Interface* interf = s_threadInterfaces[i];
		if (interf)
		{
			if (interf->m_crt_state.bindFramebuffer == buffer)
				interf->m_crt_state.bindFramebuffer = 0;
		}
	}
}
void TL_Interface::remove_program(uint32_t program)
{
	for (uint32_t i = 0; i < s_threadInterfaces.size(); i++)
	{
		TL_Interface* interf = s_threadInterfaces[i];
		if (interf)
		{
			if (interf->m_crt_state.program == program)
				interf->m_crt_state.program = 0;
		}
	}
}
void TL_Interface::remove_renderbuffer(uint32_t buffer)
{
	for (uint32_t i = 0; i < s_threadInterfaces.size(); i++)
	{
		TL_Interface* interf = s_threadInterfaces[i];
		if (interf)
		{
			if (interf->m_crt_state.bindRenderbuffer == buffer)
				interf->m_crt_state.bindRenderbuffer = 0;
		}
	}
}
void TL_Interface::remove_texture(uint32_t texture)
{
	for (uint32_t i = 0; i < s_threadInterfaces.size(); i++)
	{
		TL_Interface* interf = s_threadInterfaces[i];
		if (interf)
		{
			for (uint32_t t = 0; t < interf->m_crt_state.bindTextures.size(); t++)
			{
				if (interf->m_crt_state.bindTextures[t].first == texture)
				{
					interf->m_crt_state.bindTextures[t] = std::make_pair(0, false);
					interf->m_crt_state.bindTexturesLastChangedIdx = t;
				}
			}
		}
	}
}


}
}
}

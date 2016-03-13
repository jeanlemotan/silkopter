#include "QStdAfx.h"
#include "video/GLES/GLES_Interface.h"

#if defined(Q_WINDOWS)
#	define NOMINMAX
#	define NOWINRES
#	define NOMCX
#	include <windows.h>
#	include <GL/gl.h>
#	include <GL/glu.h>
#	include "video/GLES/std_glext.h"
#	pragma comment(lib, "OpenGL32.lib")
#	pragma comment(lib, "GLu32.lib")
#elif defined (Q_RASPBERRY_PI)
#   define GL_GLEXT_PROTOTYPES
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#elif defined(Q_LINUX)
#	define GL_GLEXT_PROTOTYPES
#	include <GL/gl.h>
#	include <GL/glext.h>
#elif defined(Q_ANDROID)
#	define GL_GLEXT_PROTOTYPES
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#elif defined(Q_IOS)
#	include <OpenGLES/ES3/gl.h>
#	include <OpenGLES/ES3/glext.h>
#endif

#include "GL_Ext_Functions.h"


namespace q
{
namespace video
{
namespace gles
{

#ifdef Q_IOS
void gles30RenderbufferStorageMultisample(iGLenum target, iGLsizei samples, iGLenum internalformat, iGLsizei width, iGLsizei height)
{
	glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
}
void gles30BlitFramebuffer(iGLint srcX0, iGLint srcY0, iGLint srcX1, iGLint srcY1, iGLint dstX0, iGLint dstY0, iGLint dstX1, iGLint dstY1, iGLbitfield mask, iGLenum filter)
{
	glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}
iGLboolean gles30UnmapBuffer(iGLenum target)
{
	return glUnmapBuffer(target);
}
void gles30BindVertexArray(iGLuint array)
{
	glBindVertexArray(array);
}
void gles30DeleteVertexArrays(iGLsizei n, const iGLuint* arrays)
{
	glDeleteVertexArrays(n, arrays);
}
void gles30GenVertexArrays(iGLsizei n, iGLuint* arrays)
{
	glGenVertexArrays(n, arrays);
}
iGLboolean gles30IsVertexArray(iGLuint array)
{
	return glIsVertexArray(array);
}
#endif

#if !defined Q_RASPBERRY_PI

void Interface::iglDrawBuffers(iGLsizei n, const iGLenum* bufs)
{
	QASSERT(n > 0 && n <= 4);
	auto* interf = get_interface();	
	auto& state = interf->m_crt_state;
	interf->flush();

	// caching is done here because this is GLES30 only (and void TL_Interface::flush() is GLES20 only)
    if (static_cast<uint32_t>(n) == interf->m_crt_state.attachmentCount)
	{
		int res = memcmp(interf->m_crt_state.attachments.data(), bufs, n * sizeof(iGLenum));
		bool doNotUpdate = res == 0;
		if (doNotUpdate)
		{
			return;
		}
	}
	
	interf->m_old_state.attachmentCount = interf->m_crt_state.attachmentCount;
	std::copy(state.attachments.begin(), state.attachments.end(), interf->m_old_state.attachments.begin());
	interf->m_crt_state.attachmentCount = n;
	std::copy(bufs, bufs + n, state.attachments.begin());
	
    GL_FUN2(interf, glDrawBuffers(n, bufs));
	iassertNoError();
}

iGLvoid* Interface::iglMapBufferRange(iGLenum target, iGLintptr offset, iGLsizeiptr length, iGLbitfield access)
{
	auto interf = get_interface();
	interf->flush();
	auto res = GL_FUN2(interf, glMapBufferRange(target, offset, length, access));
	iassertNoError();
	return res;
}

iGLvoid Interface::iglFlushMappedBufferRange(iGLenum target, iGLintptr offset, iGLsizeiptr length)
{
	auto interf = get_interface();
	interf->flush();
	GL_FUN2(interf, glFlushMappedBufferRange(target, offset, length));
	iassertNoError();
}

#endif

}
}
}

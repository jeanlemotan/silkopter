#include "QStdAfx.h"
#include "video/GLES/GLES_Fbo_Wrapper.h"
#include "video/GLES/GLES_Texture.h"
#include "video/GLES/GLES_Interface.h"
#include "video/GLES/GLES_Command_Stream.h"

using namespace q;
using namespace video;


GLES_Fbo_Wrapper::GLES_Fbo_Wrapper()
	: m_color_format(Color_Format::NONE)
	, m_depth_format(Depth_Format::NONE)
	, m_stencil_format(Stencil_Format::NONE)
	, m_aa_format(AA_Format::NONE)
	, m_gl_id(0)
	, m_depth_clear_value(1)
	, m_last_frame_idx(~0u)
{
}
GLES_Fbo_Wrapper::~GLES_Fbo_Wrapper()
{

}

bool GLES_Fbo_Wrapper::allocate(math::vec2u32 const& size, Color_Format color, Depth_Format depth, Stencil_Format stencil, AA_Format aa, bool /*prefer_npot*/)
{
	QASSERT(size.x != 0 && size.y != 0);
	m_size = size;
	m_color_format = color;
	m_depth_format = depth;
	m_stencil_format = stencil;
	m_aa_format = aa;
	return true;
}

void GLES_Fbo_Wrapper::clear(gles::Interface& interf, math::vec4f const& color, float depth, uint32_t mask)
{
	interf.iglDepthMask(gles::iGL_TRUE);
	interf.iglColorMask(gles::iGL_TRUE, gles::iGL_TRUE, gles::iGL_TRUE, gles::iGL_TRUE);

	interf.iglClearColor(color.x, color.y, color.z, color.w);
	interf.iglClearDepthf(depth);

	interf.iglClear(mask);
}

void GLES_Fbo_Wrapper::clear(bool color, bool depth, bool stencil)
{
	Command_Stream& stream = std::static_pointer_cast<GLES_Renderer>(System::inst().get_renderer())->lock_command_stream();
	Command_Stream::Render_Target_Handle rh = stream.add_render_target(std::static_pointer_cast<GLES_Fbo_Wrapper>(shared_from_this()));
	stream.do_clear_fbo(rh, color, depth, stencil);
}

void GLES_Fbo_Wrapper::clear_all()
{
	clear(true, true, true);
}

bool GLES_Fbo_Wrapper::check_complete_fbo(uint32_t fbo)
{
	gles::Interface interf;
	interf.iglBindFramebuffer(gles::iGL_FRAMEBUFFER, fbo);
	gles::iGLenum res = interf.iglCheckFramebufferStatus(gles::iGL_FRAMEBUFFER);
	if (res == gles::iGL_FRAMEBUFFER_COMPLETE)
	{
		//QLOGI("Render target success");
	}
	else if (res == gles::iGL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
	{
        QLOGE("Render target failed: Incomplete attachment");
	}
	else if (res == gles::iGL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS)
	{
        QLOGE("Render target failed: Incomplete dimensions");
	}
	else if (res == gles::iGL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
	{
        QLOGE("Render target failed: Missing attachment");
	}
	else if (res == gles::iGL_FRAMEBUFFER_UNSUPPORTED)
	{
        QLOGE("Render target failed: unsupported");
	}
	else
	{
        QLOGE("Render target failed: unknown error");
	}

	return res == gles::iGL_FRAMEBUFFER_COMPLETE;
}

void GLES_Fbo_Wrapper::bind(gles::Interface& interf, math::vec4f const& color, float depth)
{
	interf.iglBindFramebuffer(gles::iGL_FRAMEBUFFER, m_gl_id);

	auto* renderer = static_cast<GLES_Renderer*>(System::inst().get_renderer().get());
	if (m_last_frame_idx != renderer->get_frame_idx())
	{
		check_complete_fbo(m_gl_id);

		m_last_frame_idx = renderer->get_frame_idx();
		if (renderer->needs_full_clear_every_frame())
		{
			clear(interf, color, depth, gles::iGL_COLOR_BUFFER_BIT | gles::iGL_DEPTH_BUFFER_BIT | gles::iGL_STENCIL_BUFFER_BIT);
		}
		else
		{
			//if a clear is not needed every frame, only clear the depth+stencil (as they are usually much faster than clearing with a draw call)
			clear(interf, color, depth, gles::iGL_DEPTH_BUFFER_BIT | gles::iGL_STENCIL_BUFFER_BIT);
		}
	}
}

auto GLES_Fbo_Wrapper::get_allocated_size() const -> math::vec2u32 const&
{
	return m_size;
}
auto GLES_Fbo_Wrapper::get_color_texture() const -> Texture_cptr
{
	return Texture_ptr();
}
auto GLES_Fbo_Wrapper::get_depth_texture() const -> Texture_cptr
{
	return Texture_ptr();
}
void GLES_Fbo_Wrapper::set_color_clear_value(math::vec4f const& color)
{
	m_color_clear_value = color;
}
auto GLES_Fbo_Wrapper::get_color_clear_value() const -> math::vec4f const&
{
	return m_color_clear_value;
}
void GLES_Fbo_Wrapper::set_depth_clear_value(float depth)
{
	m_depth_clear_value = depth;
}
auto GLES_Fbo_Wrapper::get_depth_clear_value() const -> float
{
	return m_depth_clear_value;
}
void GLES_Fbo_Wrapper::set_gl_id(uint32_t id)
{
	m_gl_id = id;
}
auto GLES_Fbo_Wrapper::get_gl_id() const -> uint32_t
{
	return m_gl_id;
}

auto GLES_Fbo_Wrapper::get_color_format() const -> Color_Format
{
	return m_color_format;
}
auto GLES_Fbo_Wrapper::get_depth_format() const -> Depth_Format
{
	return m_depth_format;
}
auto GLES_Fbo_Wrapper::get_stencil_format() const -> Stencil_Format
{
	return m_stencil_format;
}
auto GLES_Fbo_Wrapper::get_aa_format() const -> AA_Format
{
	return m_aa_format;
}

auto GLES_Fbo_Wrapper::get_size() const -> math::vec2u32 const&
{
	return m_size;
}

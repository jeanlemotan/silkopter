#include "QStdAfx.h"
#include "video/GLES/GLES_Render_Target.h"
#include "video/GLES/GLES_Texture.h"
#include "video/GLES/GLES_Interface.h"
#include "system/System.h"
#include "video/Capabilities.h"
#include "video/Renderer.h"

using namespace q;
using namespace video;


GLES_Render_Target::~GLES_Render_Target()
{
    QLOGI("Deleting render target {}", get_gl_id());
	//TODO - check the leaks
}

bool GLES_Render_Target::allocate(math::vec2u32 const& size, Color_Format color, Depth_Format depth, Stencil_Format stencil, AA_Format aa, bool prefer_npot)
{
	if (m_color_buffer ||
		m_depth_buffer ||
		m_depth_buffer_gl_id)
	{
		return false;
	}

    //preferNpot = true;

	Renderer_ptr const& r = System::inst().get_renderer();
	QASSERT(r);

	Capabilities caps = r->get_capabilities();

    QLOGI("Creating new render target size {}", size);

	gles::Interface interf;

	if (aa != AA_Format::NONE)
	{
		uint32_t req_samples = 0;
		switch (aa)
		{
		case AA_Format::A2X: req_samples = 2; break;
		case AA_Format::A4X: req_samples = 4; break;
		case AA_Format::A8X: req_samples = 8; break;
		case AA_Format::A16X: req_samples = 16; break;
		default: req_samples = 2; break;
		}

		//check if msaa is available
		if (caps.max_aa_samples > 0)
		{
            uint32_t samples = math::min<uint32_t>(req_samples, caps.max_aa_samples);
			switch (samples)
			{
			case 0: aa = AA_Format::NONE; break;
			case 2: aa = AA_Format::A2X; break;
			case 4: aa = AA_Format::A4X; break;
			case 8: aa = AA_Format::A8X; break;
			case 16: aa = AA_Format::A16X; break;
			default: QASSERT(0); break;
			}
            QLOGI("Requested {} samples, got {}", req_samples, samples);
		}
		else
		{
            QLOGI("Requested {} samples but AA is not available", req_samples);
			aa = AA_Format::NONE;
		}
	}

	if (color != Color_Format::NONE)
	{

	}

	math::vec2u32 const& vp = r->get_viewport_size();

	math::vec2u32 s = size;
	s.x = (s.x == 0) ? vp.x : s.x;
	s.y = (s.y == 0) ? vp.y : s.y;

	int tex_width = prefer_npot ? math::get_min_pot(s.x) : s.x;
	int tex_height = prefer_npot ? math::get_min_pot(s.y) : s.y;
	math::vec2u32 tex_size(tex_width, tex_height);

    QLOGI("Requested size {}, real size {}", size, tex_size);

	m_allocated_size = tex_size;

	// get original fbo
	int original_fbo = 0;
	interf.iglGetIntegerv(gles::iGL_FRAMEBUFFER_BINDING, &original_fbo);
	int original_rb = 0;
	interf.iglGetIntegerv(gles::iGL_RENDERBUFFER_BINDING, &original_rb);

	uint32_t fbo_id = 0;
	interf.iglGenFramebuffers(1, &fbo_id);

	set_gl_id(fbo_id);
	bool res = GLES_Fbo_Wrapper::allocate(size, color, depth, stencil, aa, prefer_npot);
	QASSERT(res);

    // bind fbo
	interf.iglBindFramebuffer(gles::iGL_FRAMEBUFFER, fbo_id);

	bool error = false;
	if (color != Color_Format::NONE)
	{
		Texture::Format tex_format = Texture::Format::RGB_565;
		switch (color)
		{
		case Color_Format::RGBA_4	 : tex_format = Texture::Format::RGBA_4;	break;
		case Color_Format::RGB_565	 : tex_format = Texture::Format::RGB_565;	break;
		case Color_Format::RGBA_5551 : tex_format = Texture::Format::RGBA_5551;	break;
		case Color_Format::RGB_8	 : tex_format = Texture::Format::RGB_8;		break;
		case Color_Format::RGBA_8	 : tex_format = Texture::Format::RGBA_8;	break;
		case Color_Format::I_8		 : tex_format = Texture::Format::I_8;		break;
		case Color_Format::A_8		 : tex_format = Texture::Format::A_8;		break;
		case Color_Format::AI_8		 : tex_format = Texture::Format::AI_8;		break;
		case Color_Format::RGBA_16	 : tex_format = Texture::Format::RGBA_16;	break;
		case Color_Format::RGB_16	 : tex_format = Texture::Format::RGB_16;	break;
		case Color_Format::I_16		 : tex_format = Texture::Format::I_16;		break;
		case Color_Format::A_16		 : tex_format = Texture::Format::A_16;		break;
		case Color_Format::AI_16	 : tex_format = Texture::Format::AI_16;		break;
		case Color_Format::RGBA_32	 : tex_format = Texture::Format::RGBA_32;	break;
		case Color_Format::RGB_32	 : tex_format = Texture::Format::RGB_32;	break;
		case Color_Format::I_32		 : tex_format = Texture::Format::I_32;		break;
		case Color_Format::A_32		 : tex_format = Texture::Format::A_32;		break;
		case Color_Format::AI_32	 : tex_format = Texture::Format::AI_32;		break;
		default:
			{
				error = true;
				int format = (int)color;
                QLOGE("Unknown color format {}", format);
			}
		}

		if (!error)
		{
			int format = (int)color;
            QLOGI("Creating color texture format {}", format);
			m_color_buffer = Texture::create(Path());
			bool res = m_color_buffer->allocate(tex_format, tex_size);
			if (res)
			{
				GLES_Texture_ptr ctex = std::static_pointer_cast<GLES_Texture>(m_color_buffer);
				//gles::iglBindTexture(gles::iGL_TEXTURE_2D, ctex->getGLId());
				interf.iglFramebufferTexture2D(gles::iGL_FRAMEBUFFER, gles::iGL_COLOR_ATTACHMENT0, gles::iGL_TEXTURE_2D, ctex->get_gl_id(), 0);
			}
			else
			{
				error = true;
				int format = (int)color;
                QLOGE("Color format unsupported {}", format);
			}
		}
	}
    error = false;
	if (depth != Depth_Format::NONE)
	{
		m_depth_buffer = Texture::create(Path());

		Texture::Format tex_format;
		if (depth == Depth_Format::FULL)
		{
			tex_format = Texture::Format::DEPTH_FULL;
		}
		else if (depth == Depth_Format::HALF)
		{
			tex_format = Texture::Format::DEPTH_HALF;
		}
		else
		{
			QASSERT(0);
			error = true;
			int format = (int)depth;
            QLOGE("Unknown depth format {}", format);
		}

		if (caps.depth_texture_support)
		{
			int format = (int)depth;
            QLOGI("Allocating depth texture format {}", format);
			m_depth_buffer->allocate(tex_format, tex_size);
			GLES_Texture_ptr dtex = std::static_pointer_cast<GLES_Texture>(m_depth_buffer);
			// Now we have our depth buffer attach it to our frame buffer object.

			interf.iglFramebufferTexture2D(gles::iGL_FRAMEBUFFER, gles::iGL_DEPTH_ATTACHMENT, gles::iGL_TEXTURE_2D, dtex->get_gl_id(), 0);
		}
		else
		{
			int format = (int)depth;
            QLOGI("Allocating depth buffer format {}", format);
			// Generate and bind the handle for the render buffer (which will become our depth buffer)
			interf.iglGenRenderbuffers(1, &m_depth_buffer_gl_id);
			interf.iglBindRenderbuffer(gles::iGL_RENDERBUFFER, m_depth_buffer_gl_id);

			// allocate depth buffer
			if (depth == Depth_Format::HALF)
			{
				interf.iglRenderbufferStorage(gles::iGL_RENDERBUFFER, gles::iGL_DEPTH_COMPONENT16, tex_width, tex_height);
			}
			else if (depth == Depth_Format::FULL)
			{
				interf.iglRenderbufferStorage(gles::iGL_RENDERBUFFER, gles::iGL_DEPTH_COMPONENT, tex_width, tex_height);
			}

			// Now we have our depth buffer attach it to our frame buffer object.
			interf.iglFramebufferRenderbuffer(gles::iGL_FRAMEBUFFER, gles::iGL_DEPTH_ATTACHMENT, gles::iGL_RENDERBUFFER, m_depth_buffer_gl_id);
		}
	}

	check_complete_fbo(fbo_id);

    if (original_rb >= 0)
	{
		interf.iglBindRenderbuffer(gles::iGL_RENDERBUFFER, original_rb);
	}
	if (original_fbo >= 0)
	{
		interf.iglBindFramebuffer(gles::iGL_FRAMEBUFFER, original_fbo);
	}

	return true;
}

Texture_cptr GLES_Render_Target::get_color_texture() const
{
	return m_color_buffer;
}

Texture_cptr GLES_Render_Target::get_depth_texture() const
{
	return m_depth_buffer;
}

math::vec2u32 const& GLES_Render_Target::get_allocated_size() const
{
	return m_allocated_size;
}

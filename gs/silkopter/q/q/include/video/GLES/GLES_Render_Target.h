#pragma once

#include "GLES_Fbo_Wrapper.h"

namespace q
{
namespace video
{
	class Command_Stream;

	class GLES_Render_Target : public GLES_Fbo_Wrapper
	{
		friend class Command_Stream;
	public:
		GLES_Render_Target();
		~GLES_Render_Target();

		bool allocate(math::vec2u32 const& size,
			Color_Format color,
			Depth_Format depth,
			Stencil_Format stencil,
			AA_Format aa,
			bool prefer_npot);

		auto get_allocated_size() const->math::vec2u32 const&;

		auto get_color_texture() const -> Texture_cptr;
		auto get_depth_texture() const -> Texture_cptr;

	protected:

	private:
		math::vec2u32			m_allocated_size;

		Texture_ptr			m_color_buffer;
		Texture_ptr			m_depth_buffer;

		uint32_t			m_frame_buffer_gl_id;
		uint32_t			m_frame_buffer_aa_gl_id;
		uint32_t			m_color_buffer_aa_gl_id;
		bool				m_needs_to_resolve;

		uint32_t			m_depth_buffer_gl_id;
	};

	DECLARE_CLASS_PTR(GLES_Render_Target);
}
}

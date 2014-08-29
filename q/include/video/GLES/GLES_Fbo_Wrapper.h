#pragma once

namespace q
{
namespace video
{
	class Command_Stream;

	class GLES_Fbo_Wrapper : public Render_Target
	{
		friend class Command_Stream;
	public:
		GLES_Fbo_Wrapper();
		virtual ~GLES_Fbo_Wrapper();

		bool allocate(math::vec2u32 const& size, Color_Format color, Depth_Format depth, Stencil_Format stencil, AA_Format aa, bool prefer_npot);

		auto get_color_format() const -> Color_Format;
		auto get_depth_format() const -> Depth_Format;
		auto get_stencil_format() const -> Stencil_Format;
		auto get_aa_format() const -> AA_Format;

		auto get_size() const->math::vec2u32 const&;
		auto get_allocated_size() const->math::vec2u32 const&;

		auto get_color_texture() const -> Texture_cptr;
		auto get_depth_texture() const -> Texture_cptr;

		void set_color_clear_value(math::vec4f const& color);
		auto get_color_clear_value() const -> math::vec4f const&;

		void set_depth_clear_value(float depth);
		auto get_depth_clear_value() const -> float;

		void clear(bool color, bool depth, bool stencil);
		void clear_all();


		void set_gl_id(uint32_t id);
		auto get_gl_id() const -> uint32_t;

	protected:
		auto check_complete_fbo(uint32_t fbo) -> bool;

		void clear(gles::Interface& interf, math::vec4f const& color, float depth, uint32_t mask);
		void bind(gles::Interface& interf, math::vec4f const& color, float depth);

	private:
		Color_Format m_color_format;
		Depth_Format m_depth_format;
		Stencil_Format m_stencil_format;
		AA_Format m_aa_format;

		math::vec2u32 m_size;

		uint32_t m_gl_id;

		math::vec4f m_color_clear_value;
		float m_depth_clear_value;
		size_t m_last_frame_idx;
	};

	DECLARE_CLASS_PTR(GLES_Fbo_Wrapper);
}
}

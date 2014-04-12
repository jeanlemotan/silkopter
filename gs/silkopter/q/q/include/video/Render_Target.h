#pragma once

namespace q
{
namespace video
{

	class Render_Target : public std::enable_shared_from_this<Render_Target>, public util::Noncopyable
	{
	public:
        virtual ~Render_Target() {}

		static auto	create() -> Render_Target_ptr;

		enum class Color_Format : uint8_t
		{
			NONE,
			RGBA_4,
			RGB_565,
			RGBA_5551,
			RGB_8,
			RGBA_8,
			I_8,
			A_8,
			AI_8,

			//float formats
			RGBA_16,
			RGB_16,
			I_16,
			A_16,
			AI_16,

			RGBA_32,
			RGB_32,
			I_32,
			A_32,
			AI_32,
		};

		enum class Depth_Format : uint8_t
		{
			NONE,
			HALF,
			FULL
		};

		enum class Stencil_Format : uint8_t
		{
			NONE,
			FULL,
		};

		enum class AA_Format : uint8_t
		{
			NONE,
			A2X,
			A4X,
			A8X,
			A16X
		};

		virtual bool allocate(math::vec2u32 const& size,
			Color_Format color,
			Depth_Format depth,
			Stencil_Format stencil,
			AA_Format aa,
			bool prefer_npot) = 0;

		virtual Color_Format		get_color_format() const = 0;
		virtual Depth_Format		get_depth_format() const = 0;
		virtual Stencil_Format		get_stencil_format() const = 0;
		virtual AA_Format			get_aa_format() const = 0;

		virtual math::vec2u32 const&	get_size() const = 0;
		virtual math::vec2u32 const&	get_allocated_size() const = 0;

		virtual Texture_cptr		get_color_texture() const = 0;
		virtual Texture_cptr		get_depth_texture() const = 0;

		virtual void				set_color_clear_value(math::vec3f const& color) = 0;
		virtual math::vec3f const&	get_color_clear_value() const = 0;

		virtual void				set_depth_clear_value(float depth) = 0;
		virtual float				get_depth_clear_value() const = 0;

		virtual void				clear(bool color, bool depth, bool stencil) = 0;
		virtual void				clear_all() = 0;
	};

}
}

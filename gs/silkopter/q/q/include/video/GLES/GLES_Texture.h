#pragma once

namespace q
{
namespace video
{

	class GLES_Texture : public Texture
	{
		friend class GLES_Renderer;
		friend class GLES_Shader;
		friend class GLES_Render_Target;
	public:
		GLES_Texture(Path const& path);

		virtual void		unload();
		virtual bool		is_valid() const;

		virtual math::vec2u32 const&	get_size() const;
		virtual bool		is_npot() const;

		virtual Format		get_format() const;
		virtual Type		get_type() const;
		virtual bool		has_mipmaps() const;

		virtual bool		supports_format(Format format) const;
		virtual bool		allocate(Format format, math::vec2u32 const& size);
		virtual void		upload_data(uint32_t mipmap, uint8_t const* data);
		virtual void		generate_mipmaps();

	protected:
		void				bind(Sampler const& sampler, uint32_t slot_idx) const;

		uint32_t			create_gl_object();
		void				destroy_gl_object();
		uint32_t			get_gl_id() const;

	private:
		String				m_name;

		Format				m_format;
		math::vec2u32		m_size;
		bool				m_is_npot;
		bool				m_has_mipmaps;
		bool				m_is_valid;


		struct Texture_Deleter
		{
			typedef Handle pointer;
			void operator()(Handle p);
		};

		std::unique_ptr<Handle, Texture_Deleter> m_gl_id;

		mutable int			m_old_filtering;
		mutable int			m_old_use_mipmapping;
		mutable int			m_old_wrapping_u;
		mutable int			m_old_wrapping_v;
	};


	inline unsigned int GLES_Texture::get_gl_id() const
	{
		return m_gl_id.get().value;
	}

	DECLARE_CLASS_PTR(GLES_Texture);

}
}

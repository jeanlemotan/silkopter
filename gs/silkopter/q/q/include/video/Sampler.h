#pragma once

namespace q
{
namespace video
{

	class Sampler
	{
	public:
		enum class Type : uint8_t
		{
			TEXTURE_2D,
			TEXTURE_CUBE
		};

		enum class Filter : uint8_t
		{
			NEAREST,
			BILINEAR,
			TRILINEAR
		};

		enum class Wrap : uint8_t
		{
			WRAP,
			CLAMP
		};

		Sampler();
		Sampler(Sampler const& other);
		Sampler(Sampler&& other);
		~Sampler();

		Sampler&		operator=(Sampler const& other);
		bool			operator==(Sampler const& other) const;
		DEFINE_EQ_OPERATORS(Sampler);

		Type			get_type() const;
		void			set_type(Type type);

		bool			get_mipmapping() const;
		void			set_mipmapping(bool yes);

		Filter			get_filtering() const;
		void			set_filtering(Filter filtering);

		Wrap			get_wrapping_u() const;
		Wrap			get_wrapping_v() const;
		void			set_wrapping(Wrap u, Wrap v);

		Texture_cptr	get_texture() const;
		void			set_texture(Texture_cptr const& ptr);

	private:
		Filter			m_filtering;
		Wrap			m_wrapping_u;
		Wrap			m_wrapping_v;
		Type			m_type;
		bool			m_use_mipmap;
		Texture_cptr	m_texture;
	};

	inline Sampler::Sampler()
		: m_filtering(Filter::BILINEAR)
		, m_wrapping_u(Wrap::WRAP)
		, m_wrapping_v(Wrap::WRAP)
		, m_type(Type::TEXTURE_2D)
		, m_use_mipmap(true)
	{
	}
	inline Sampler::Sampler(Sampler const& other)
		: m_filtering(other.m_filtering)
		, m_wrapping_u(other.m_wrapping_u)
		, m_wrapping_v(other.m_wrapping_v)
		, m_type(other.m_type)
		, m_use_mipmap(other.m_use_mipmap)
		, m_texture(other.m_texture)
	{
	}

	inline Sampler::Sampler(Sampler&& other)
		: m_filtering(std::move(other.m_filtering))
		, m_wrapping_u(std::move(other.m_wrapping_u))
		, m_wrapping_v(std::move(other.m_wrapping_v))
		, m_type(std::move(other.m_type))
		, m_use_mipmap(std::move(other.m_use_mipmap))
		, m_texture(std::move(other.m_texture))
	{
	}

	inline Sampler::~Sampler()
	{
	}

	inline Sampler& Sampler::operator=(Sampler const& other)
	{
		m_filtering = other.m_filtering;
		m_wrapping_u = other.m_wrapping_u;
		m_wrapping_v = other.m_wrapping_v;
		m_use_mipmap = other.m_use_mipmap;
		m_type = other.m_type;
		m_texture = other.m_texture;
		return *this;
	}
	inline bool Sampler::operator==(Sampler const& other) const
	{
		return m_filtering == other.m_filtering 
			&& m_wrapping_u == other.m_wrapping_u
			&& m_wrapping_v == other.m_wrapping_v
			&& m_type == other.m_type
			&& m_use_mipmap == other.m_use_mipmap
			&& m_texture == other.m_texture;
	}

	inline bool Sampler::get_mipmapping() const
	{
		return m_use_mipmap;
	}
	inline void Sampler::set_mipmapping(bool yes)
	{
		m_use_mipmap = yes;
	}

	inline Sampler::Filter Sampler::get_filtering() const
	{
		return m_filtering;
	}
	inline void Sampler::set_filtering(Filter filtering)
	{
		m_filtering = filtering;
	}

	inline Sampler::Wrap Sampler::get_wrapping_u() const
	{
		return m_wrapping_u;
	}
	inline Sampler::Wrap Sampler::get_wrapping_v() const
	{
		return m_wrapping_v;
	}
	inline void Sampler::set_wrapping(Wrap u, Wrap v)
	{
		m_wrapping_u = u;
		m_wrapping_v = v;
	}

	inline Texture_cptr Sampler::get_texture() const
	{
		return m_texture;
	}
	inline void Sampler::set_texture(Texture_cptr const& ptr)
	{
		m_texture = ptr;
	}
	inline Sampler::Type Sampler::get_type() const
	{
		return m_type;
	}
	inline void Sampler::set_type(Type type)
	{
		m_type = type;
	}



}
}
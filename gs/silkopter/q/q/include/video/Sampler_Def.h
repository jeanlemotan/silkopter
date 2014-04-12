#pragma once

#include "Sampler.h"

namespace q
{
namespace video
{

	class Sampler_Def
	{
	public:
		//defines the purpose of this sampler
		enum class Semantic : uint8_t
		{
			DIFFUSE,
			NORMAL,
			LIGHTMAP,
			USER
		};

	public:
		Sampler_Def();
		Sampler_Def(Sampler_Def&& other);
		Sampler_Def(String const& name, Sampler const& def);
		Sampler_Def(String const& name, String const& human_name, Sampler const& def);

#if !defined(_WIN32)
		Sampler_Def(Sampler_Def const& other) = default;
		Sampler_Def& operator=(Sampler_Def const& other) = default;
#endif

		String	get_human_name() const; //a pretty name to be displayed to the user, in an editor for example
		String	get_name() const; //the name to be used in the shader
		void			set_name(String const& name);
		Sampler const&	get_default_value() const;

		typedef std::function<Sampler const& (void*)> Getter;

		void			set_getter(Getter func);
		Sampler const&	get_value(void* data) const;
		bool			has_getter() const;

		void			set_semantic(Semantic s);
		Semantic		get_semantic() const;

		void			set_enabled(bool yes);
		bool			is_enabled() const;

	private:
		String			m_name;
		String			m_human_name;
		Sampler			m_default;
		Semantic		m_semantic;

		Getter			m_getter;
		bool			m_is_enabled;
	};

	inline Sampler_Def::Sampler_Def()
		: m_semantic(Semantic::USER)
		, m_is_enabled(false)
	{
		;
	}
	inline Sampler_Def::Sampler_Def(Sampler_Def&& other)
		: m_name(std::move(other.m_name))
		, m_human_name(std::move(other.m_human_name))
		, m_default(std::move(other.m_default))
		, m_semantic(other.m_semantic)
		, m_getter(std::move(other.m_getter))
		, m_is_enabled(other.m_is_enabled)
	{
	}
	inline Sampler_Def::Sampler_Def(String const& name, Sampler const& def)
		: m_name(name)
		, m_human_name(name)
		, m_default(def)
		, m_is_enabled(false)
	{
		;
	}
	inline Sampler_Def::Sampler_Def(String const& name, String const& human_name, Sampler const& def)
		: m_name(name)
		, m_human_name(human_name)
		, m_default(def)
		, m_is_enabled(false)
	{
		;
	}
	inline String Sampler_Def::get_name() const
	{
		return m_name;
	}
	inline void Sampler_Def::set_name(String const& name)
	{
		m_name = name;
	}
	inline String Sampler_Def::get_human_name() const
	{
		return m_human_name;
	}
	inline Sampler const& Sampler_Def::get_default_value() const
	{
		return m_default;
	}
	inline void Sampler_Def::set_getter(Getter func)
	{
		m_getter = func;
	}
	inline bool Sampler_Def::has_getter() const
	{
		return m_getter != 0;
	}
	inline Sampler const& Sampler_Def::get_value(void* data) const
	{
		return m_getter ? m_getter(data) : m_default;
	}
	inline void Sampler_Def::set_semantic(Semantic s)
	{
		m_semantic = s;
	}
	inline Sampler_Def::Semantic Sampler_Def::get_semantic() const
	{
		return m_semantic;
	}
	inline void Sampler_Def::set_enabled(bool yes)
	{
		m_is_enabled = yes;
	}
	inline bool Sampler_Def::is_enabled() const
	{
		return m_is_enabled;
	}

}
}

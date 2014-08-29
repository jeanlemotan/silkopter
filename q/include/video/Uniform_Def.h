#pragma once

#include "Uniform.h"

namespace q
{
namespace video
{

	class Uniform_Def
	{
	public:
		enum class Precision : uint8_t
		{
			LOW,
			MEDIUM,
			HIGH
		};

        Uniform_Def() = default;
        Uniform_Def(Uniform_Def const&) = default;
        Uniform_Def(Uniform_Def&&) = default;
		Uniform_Def(String const& name, 
				const Uniform& default_value, const Uniform& min_value, const Uniform& max_value);
		Uniform_Def(String const& name, String const& human_name, 
				const Uniform& default_value, const Uniform& min_value, const Uniform& max_value);
		Uniform_Def(String const& name, String const& human_name);

		Uniform_Def& operator=(Uniform_Def const& other);

		String	get_human_name() const;
		String	get_name() const;
		void			set_name(String const& name);
		Uniform const&	get_default_value() const;
		Uniform const&	get_max_value() const;
		Uniform const&	get_min_value() const;

		Path const&		get_link() const;
		void			set_link(Path const& link);

		typedef std::function<Uniform const& (void*)> Getter;

		void			set_getter(Getter func);
		Uniform const&	get_value(void* data) const;
		bool			has_getter() const;

		void			set_enabled(bool yes);
		bool			is_enabled() const;

		void			set_precision(Precision precision);
		Precision		get_precision() const;

		void			set_semantic(String const& semantic);
		String	get_semantic() const;

	private:
		String			m_name;
		String			m_human_name;
		Uniform			m_default;
		Uniform			m_min;
		Uniform			m_max;
		Path			m_link;

		Getter			m_getter;
        bool			m_is_enabled = false;
        Precision		m_precision = Precision::HIGH;

		String			m_semantic;
	};

	inline Uniform_Def::Uniform_Def(String const& name, const Uniform& default_value, const Uniform& min_value, const Uniform& max_value)
		: m_name(name)
		, m_human_name(name)
	{
		QASSERT(default_value.get_type() == min_value.get_type() && min_value.get_type() == max_value.get_type());

		m_default = default_value;
		m_min = min_value;
		m_max = max_value;
	}
	inline Uniform_Def::Uniform_Def(String const& name, String const& human_name, const Uniform& default_value, const Uniform& min_value, const Uniform& max_value)
		: m_name(name)
		, m_human_name(human_name)
	{
		QASSERT(default_value.get_type() == min_value.get_type() && min_value.get_type() == max_value.get_type());

		m_default = default_value;
		m_min = min_value;
		m_max = max_value;
	}
	inline Uniform_Def::Uniform_Def(String const& name, String const& human_name)
		: m_name(name)
		, m_human_name(human_name)
	{
		;
	}

	inline Uniform_Def& Uniform_Def::operator=(Uniform_Def const& other)
	{
		if (this != &other)
		{
			m_name = other.m_name;
			m_human_name = other.m_human_name;
			m_default = other.m_default;
			m_min = other.m_min;
			m_max = other.m_max;
			m_link = other.m_link;
			m_getter = other.m_getter;
			m_is_enabled = other.m_is_enabled;
			m_precision = other.m_precision;
		}
		return *this;
	}

	inline Path const& Uniform_Def::get_link() const
	{
		return m_link;
	}
	inline void Uniform_Def::set_link(Path const& link)
	{
		m_link = link;
	}

	inline String Uniform_Def::get_human_name() const
	{
		return m_human_name;
	}
	inline String Uniform_Def::get_name() const
	{
		return m_name;
	}
	inline void Uniform_Def::set_name(String const& name)
	{
		m_name = name;
	}
	inline const Uniform& Uniform_Def::get_default_value() const
	{
		return m_default;
	}
	inline const Uniform& Uniform_Def::get_max_value() const
	{
		return m_max;
	}
	inline const Uniform& Uniform_Def::get_min_value() const
	{
		return m_min;
	}
	inline void Uniform_Def::set_getter(Getter func)
	{
		m_getter = func;
		if (func)
		{
            m_default = func(nullptr);
		}
	}
	inline bool Uniform_Def::has_getter() const
	{
		return m_getter != 0;
	}
	inline Uniform const& Uniform_Def::get_value(void* data) const
	{
		return m_getter ? m_getter(data) : m_default;
	}
	inline void Uniform_Def::set_enabled(bool yes)
	{
		m_is_enabled = yes;
	}
	inline bool Uniform_Def::is_enabled() const
	{
		return m_is_enabled;
	}

	inline void Uniform_Def::set_precision(Precision precision)
	{
		m_precision = precision;
	}
	inline Uniform_Def::Precision Uniform_Def::get_precision() const
	{
		return m_precision;
	}

	inline void Uniform_Def::set_semantic(String const& semantic)
	{
		m_semantic = semantic;
	}
	inline String Uniform_Def::get_semantic() const
	{
		return m_semantic;
	}
}
}

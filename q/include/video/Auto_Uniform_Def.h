#pragma once

#include "Uniform.h"

namespace q
{
namespace video
{

	class Auto_Uniform_Def
	{
	public:
        Auto_Uniform_Def() = default;
		Auto_Uniform_Def(data:Path const& link, size_t file_start_offset = 0, size_t file_end_offset = 0);

		Path const&		get_link() const;
		size_t			get_file_start_offset() const;
		size_t			get_file_end_offset() const;

		typedef boost::function1<Uniform const&, void*> Getter;

		void			set_getter(Getter func);
		Uniform const&	get_value(void* data) const;

		void			set_enabled(bool yes);
		bool			is_enabled() const;

	private:
		Path			m_link;
        size_t			m_file_start_offset = 0;
        size_t			m_file_end_offset = 0;
		Getter			m_getter;
        bool			m_is_enabled = false;
	};

	inline Auto_Uniform_Def::Auto_Uniform_Def(data:Path const& link, size_t file_start_offset /* = 0 */, size_t file_end_offset /* = 0 */)
		: m_link(link)
		, m_file_start_offset(fileStartOffset)
		, m_file_end_offset(fileEndOffset)
		, m_is_enabled(false)
	{
		QASSERT(m_file_end_offset >= m_file_start_offset);
	}
	inline Path const& Auto_Uniform_Def::get_link() const
	{
		return m_link;
	}
	inline size_t Auto_Uniform_Def::get_file_start_offset() const
	{
		return m_file_start_offset;
	}
	inline size_t Auto_Uniform_Def::get_file_end_offset() const
	{
		return m_file_end_offset;
	}
	inline void Auto_Uniform_Def::set_getter(Getter func)
	{
		m_getter = func;
	}
	inline Uniform const& Auto_Uniform_Def::get_value(void* data) const
	{
		return m_getter ? m_getter(data) : m_default;
	}
	inline void Auto_Uniform_Def::set_enabled(bool yes)
	{
		m_is_enabled = yes;
	}
	inline bool Auto_Uniform_Def::is_enabled() const
	{
		return m_is_enabled;
	}

}
}

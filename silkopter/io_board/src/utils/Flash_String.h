#pragma once

#ifdef __AVR__
#	include <avr/pgmspace.h>
#endif

#include "QBase.h"

namespace util
{
	
#ifdef __AVR__
#	define F_STR(x) ::util::Flash_String(PSTR(x))
#else
#	define F_STR(x) (x)
#	define PGM_P char const*
#	define pgm_read_byte_near(x) (*(x))
#	define strlen_P(x) strlen(x)
#endif

class Flash_String
{
public:
	struct Iterator
	{
		explicit Iterator(PGM_P ptr) : m_ptr(ptr) {}
		char operator*() const { return pgm_read_byte_near(m_ptr); }
		Iterator& operator++() { m_ptr += 1; return *this; }
		const Iterator operator++(int) { Iterator temp(*this); ++m_ptr; return temp; }
	private:
		PGM_P m_ptr;
	};

	explicit Flash_String(PGM_P src) : m_str(src) {}

	typedef char value_type;
	typedef Iterator const_iterator;
	
	auto size() const -> size_t { return strlen_P(m_str); }
	auto data() const -> PGM_P { return m_str; }
	auto begin() const -> const_iterator { return Iterator(m_str); }
	auto end() const -> const_iterator { return Iterator(m_str + size()); }
		
private:
	PGM_P m_str;
};

}

namespace q
{
	namespace util
	{
		namespace formatting
		{
            template<> struct Format_String_Adapter<::util::Flash_String>
			{
				public:
				typedef ::util::Flash_String::value_type value_type;
				typedef ::util::Flash_String::const_iterator iterator;
				Format_String_Adapter(::util::Flash_String const& fmt) : m_it(fmt.begin()) {}
				auto is_done() const -> bool { return *m_it == 0; }
				auto get() -> value_type { return *m_it; }
				auto get_and_advance() -> value_type { auto c = *m_it; m_it++; return c; }
				private:
				::util::Flash_String::const_iterator m_it;
			};

			template<class Dst_Adapter, class Placeholder>
			void format_string(Dst_Adapter& dst, Placeholder const&, ::util::Flash_String const& p)
			{
				for (auto it = p.begin(); *it != 0; ++it)
				{
					dst.append(*it);
				}
            }
		}
	}
}


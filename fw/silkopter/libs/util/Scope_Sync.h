#pragma once


#include "util/Noncopyable.h"
#ifdef __AVR__
#	include <avr/interrupt.h>
#endif

namespace util
{


class Scope_Sync : Noncopyable
{
public: 
	Scope_Sync()
	{
#ifdef __AVR__
		m_old_sreg = SREG;
		cli();
#endif
	}
	~Scope_Sync()
	{
#ifdef __AVR__
		SREG = m_old_sreg;
#endif
	}

private:
#ifdef __AVR__
	uint8_t m_old_sreg;
#endif
};


}
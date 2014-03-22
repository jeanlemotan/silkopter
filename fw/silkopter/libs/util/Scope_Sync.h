#pragma once


#include "util/Noncopyable.h"
#include <avr/interrupt.h>

namespace util
{


class Scope_Sync : Noncopyable
{
public: 
	Scope_Sync()
	{
		m_old_sreg = SREG;
		cli();
	}
	~Scope_Sync()
	{
		SREG = m_old_sreg;
	}

private:
	uint8_t m_old_sreg;
};


}
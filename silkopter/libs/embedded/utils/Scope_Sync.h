#pragma once


#include "qbase.h"
#ifdef __AVR__
#	include <avr/interrupt.h>
#endif

#ifdef Q_WINDOWS
extern std::atomic<uint8_t> SREG;
extern void cli();
#endif

namespace util
{

class Scope_Sync : q::util::Noncopyable
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
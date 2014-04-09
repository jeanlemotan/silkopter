#pragma once

#include "board/Sonar.h"

namespace board
{

class Sonar_SR04 : public Sonar
{
public:
	Sonar_SR04();
	
	bool get_data(Data& data) const;
	
	//called by the ISR
	static void trigger();
private:
	static void measure(void* ptr);
	
	chrono::time_us m_last_trigger;

	volatile uint8_t m_state = 0;
	chrono::time_us m_start_time;

	volatile chrono::micros m_echo_delay;
	volatile float m_distance = 0;
};

}
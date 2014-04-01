#pragma once

namespace board
{

void _sonar_trigger(void* ptr);

class Sonar_sr04 : public Sonar
{
	friend void _sonar_trigger(void* ptr);
public:
	Sonar_sr04();
	
	bool get_data(Data& data) const;
	
private:
	chrono::time_us m_last_trigger;

	volatile uint8_t m_state;
	chrono::time_us m_start_time;

	volatile chrono::micros m_echo_delay;
	volatile float m_distance;
};

}
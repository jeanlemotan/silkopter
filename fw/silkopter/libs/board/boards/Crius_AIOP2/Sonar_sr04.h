#pragma once

namespace board
{

void _sonar_trigger(void* ptr);

class Sonar
{
	friend void _sonar_trigger(void* ptr);
public:
	Sonar();
	
	struct Data
	{
		bool is_valid;
		float distance;
	};
		
	void get_data(Data& data) const;
	
private:
	chrono::time_us s_last_trigger;

	volatile uint8_t s_state;
	volatile chrono::time_us s_start_time;
	volatile chrono::micros s_echo_delay;
	volatile float s_altitude;
};

}
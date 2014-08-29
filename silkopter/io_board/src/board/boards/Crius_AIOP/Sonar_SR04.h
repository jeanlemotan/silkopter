#pragma once

#include "board/Sonar.h"
#include "utils/Double_Buffer.h"

namespace board
{

class Sonar_SR04 : public Sonar
{
public:
    Sonar_SR04();

    void start();
    bool is_started() const;

    Data_Config get_data_config() const { return m_data_config; }
    bool get_data(Data& data) const;
	
	//called by the ISR
	static void trigger();
private:
	static void measure(void* ptr);

    bool m_is_started = false;
	
	chrono::time_us m_last_trigger;

	volatile uint8_t m_state = 0;
	chrono::time_us m_start_time;

	volatile chrono::micros m_echo_delay;

    struct Buffer
    {
        uint16_t delay_us = 0;
        uint8_t sample_count = 0;
    };
    mutable util::Double_Buffer<Buffer> m_buffer;

    Data_Config m_data_config;
    mutable Data m_out_data;
};

}

#pragma once

#include "board/Compass.h"

namespace board
{
	
class Compass_HMC5843_5883L_i2c : public Compass
{
public:
	Compass_HMC5843_5883L_i2c();
	bool get_data(Data& data) const;
		
	void init();
		
private:
    math::vec3f			m_calibration;
    bool                _initialised;
    uint8_t             _base_config;
    uint32_t            _retry_time; // when unhealthy the millis() value to retry at

    int16_t			    _mag_x;
    int16_t			    _mag_y;
    int16_t			    _mag_z;
    int16_t             _mag_x_accum;
    int16_t             _mag_y_accum;
    int16_t             _mag_z_accum;
    uint8_t			    _accum_count;
    uint32_t            _last_accum_time;

	//////////////////////////////////////////////////////////////////////////
	
	static void			poll_data(void*);
	void	poll_data_locked();
	bool	init_hardware();

	struct Buffer
	{
		volatile math::vec3s32 compass_sum;
		volatile uint8_t sample_count = 0;
	};
	mutable volatile Buffer m_buffers[2];
	mutable volatile uint8_t m_buffer_idx = 0;
	
	mutable Data m_out_data;
	
	bool m_is_initialized = false;
	
	enum class Compass_Model : uint8_t
	{
		HMC5843,
		HMC5883L
	};
	Compass_Model m_model = Compass_Model::HMC5883L;
};
	
}

#pragma once

#include "board/Compass.h"
#include "utils/Double_Buffer.h"

namespace board
{
	
class Compass_HMC5843_5883L_i2c : public Compass
{
public:
	Compass_HMC5843_5883L_i2c();
    Data_Config get_data_config() const { return m_data_config; }
    bool get_data(Data& data) const;
	
	void init();
		
private:
    math::vec3s32			m_calibration_scale;

	//////////////////////////////////////////////////////////////////////////
	
	static void			poll_data(void*);
	void	poll_data_locked(bool unlock);
	bool	init_hardware();

	struct Buffer
	{
        /*volatile*/ math::vec3s32 sum;
		volatile uint8_t sample_count = 0;
	};
	mutable util::Double_Buffer<Buffer> m_buffer;
	
    Data_Config m_data_config;
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

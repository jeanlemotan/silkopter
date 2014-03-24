#include "UAV.h"
#include "Motor_Mixer.h"
#include "debug/debug.h"
#include "board/clock.h"
#include "physics/constants.h"


using namespace silk;

UAV::UAV(Motor_Mixer::Type type, uint8_t motor_count, float radius)
	: m_radius(radius)
	, m_motor_mixer(type, motor_count)
	, m_control_frame_reference(Control_Reference_Frame::UAV)
	, m_control_mode(Control_Mode::SILKY)
	, m_dts(0)
{
	if (radius < 0.01f)
	{
		PANIC();
	}
}

void UAV::set_assists(Assists const& assists)
{
	m_assists = assists;
}
void UAV::set_failsafes(Failsafes const& failsafes)
{
	m_failsafes = failsafes;
}

void UAV::set_control_reference_frame(Control_Reference_Frame frame)
{
	m_control_frame_reference = frame;
}
void UAV::set_control_mode(Control_Mode mode)
{
	m_control_mode = mode;
}

const UAV::Status& UAV::get_status()
{
	return m_status;
}

void UAV::process()
{
	auto now = board::clock::now_us();
	if (now - m_last_time < chrono::micros(2000))
	{
		return;
	}
	m_dt = now - m_last_time;
	m_dts = float(m_dt.count) * 0.000001f;
	m_last_time = now;
	
	read_imu_data();
	read_sonar_data();
	read_gps_data();
	read_baro_data();
	read_compass_data();
	
	compute_rotation();
	compute_linear_motion();
}

void UAV::read_imu_data()
{
	board::imu::get_data(m_imu_data);
}
void UAV::read_sonar_data()
{
	board::sonar::get_data(m_sonar_data);
}
void UAV::read_gps_data()
{
//	board::gps::get_data(m_gps_data);
}
void UAV::read_baro_data()
{
	board::baro::get_data(m_baro_data);
}
void UAV::read_compass_data()
{
//	board::compass::get_data(m_compass_data);
}

void UAV::compute_rotation()
{
	auto& gyro = m_imu_data.gyroscope.value;
	gyro.z = 0; //TODO - use compass
	m_status.attitude.yaw = gyro.z;
	m_status.attitude.pitch = gyro.x;
	m_status.attitude.roll = gyro.y;
	
	//TODO - figure out why there's a minus in the euler!!!
	math::quatf rot(math::quatf::eulerXYZ, -gyro);
	rot.get_as_mat3(m_status.attitude.rotation);
	//PRINT("\nATT: {0} / {1} / {2} ::: {3}", m_status.attitude.front, m_status.attitude.right, m_status.attitude.up, gyro);
}

void UAV::compute_linear_motion()
{
	m_status.acceleration = m_imu_data.accelerometer.value + m_status.attitude.up * physics::constants::g;
	m_status.velocity += m_status.acceleration * m_dts;
	m_status.position += m_status.velocity * m_dts;
}

	

#pragma once

#include <boost/optional.hpp>

class SFull_Protocol : boost::noncopyable
{
public:
	SFull_Protocol();
	~SFull_Protocol();

	void listen_for_connection(std::string const& com_port, uint32_t baud);
	void stop();
	bool is_listening() const;
	bool is_connected() const;

	void process();

	bool set_board_gyroscope_bias(math::vec3f const& bias);
	bool set_board_accelerometer_bias_scale(math::vec3f const& bias, math::vec3f const& scale);

	template<class Payload>
	struct Data
	{
		Data() : is_valid(false), value() {}
		bool is_valid;
		Payload value;
	};

	Data<uint8_t> data_board_cpu_usage;
	Data<uint32_t> data_board_time;
	Data<std::vector<int16_t>> data_board_rc_in;
	Data<std::vector<int16_t>> data_board_pwm_out;
	Data<math::vec3f> data_board_gyroscope;
	Data<math::vec3f> data_board_accelerometer;
	Data<float> data_board_temperature;
	Data<float> data_board_baro_pressure;
	Data<float> data_board_sonar_distance;
	Data<float> data_board_gps_altitude;
	Data<math::vec3f> data_uav_acceleration;
	Data<math::vec3f> data_uav_velocity;
	Data<math::vec3f> data_uav_position;
	Data<math::vec3f> data_uav_attitude;

private:
	bool m_is_listening;
	bool m_is_connected;
	uint8_t m_protocol_version;
	boost::asio::io_service m_io;
	boost::asio::serial_port m_port;
	std::thread m_io_thread;
	bool m_stop_thread;
	std::array<uint8_t, 8> m_serial_buffer;

	std::mutex m_rx_buffer_mutex;
	std::vector<uint8_t> m_rx_buffer;

	std::mutex m_tx_buffer_mutex;
	std::vector<uint8_t> m_tx_buffer;

	enum class RX_Message : uint8_t
	{
		HELLO_WORLD = 253,
		ACKNOLEDGE = 254,

		//////////////////////////////////////////////////////////////////////////
		//BOARD

		BOARD_CPU_USAGE = 0,
		BOARD_TIME = 1,
		BOARD_GYROSCOPE = 10,
		BOARD_ACCELEROMETER = 11,
		BOARD_TEMPERATURE = 12,
		BOARD_BARO_PRESSURE = 13,
		BOARD_SONAR_DISTANCE = 14,
		BOARD_GPS_ALTITUDE = 15,
		BOARD_RC_IN = 16,
		BOARD_PWM_OUT = 17,

		//////////////////////////////////////////////////////////////////////////
		//UAV

		UAV_ACCELERATION = 30,
		UAV_VELOCITY = 31,
		UAV_POSITION = 32,
		UAV_ATTITUDE = 33,
		UAV_PHASE = 34,
		UAV_CONTROL_MODE = 35,
		UAV_CONTROL_REFERENCE_FRAME = 36,
	};

	//each has a corresponding receive method
	enum class TX_Message
	{
		//control messages
		SET_RX_MESSAGE_ENABLED,
		SET_ALL_RX_MESSAGES_ENABLED,

		//calibration
		SET_BOARD_ACCELEROMETER_BIAS_SCALE,
		SET_BOARD_GYROSCOPE_BIAS,
	};

	struct Header
	{
		RX_Message msg;
		uint8_t size;
		uint16_t crc;
	};

	void start_tx_message(TX_Message msg);
	uint16_t flush_tx_message();

	boost::optional<Header> decode_header(bool& needs_more_data);
	bool wait_for_response(uint16_t expected_crc, std::chrono::high_resolution_clock::duration timeout);

	void process_rx_message(Header const& header);
	uint16_t compute_crc(uint8_t const* data, size_t size) const;

	std::string m_uav_message;
	uint16_t m_uav_version;
};

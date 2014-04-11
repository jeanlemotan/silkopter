#pragma once

#include <boost/optional.hpp>

class SFull_Protocol : boost::noncopyable
{
public:
	SFull_Protocol();
	~SFull_Protocol();

	enum class RX_Message : uint8_t
	{
		HELLO_WORLD = 253,
		ACKNOWLEDGE = 254,
		PRINT = 255,

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


	void listen_for_connection(std::string const& com_port, uint32_t baud);
	void stop();
	bool is_listening() const;
	bool is_connected() const;

	void process();

	bool set_board_gyroscope_bias(math::vec3f const& bias);
	bool set_board_accelerometer_bias_scale(math::vec3f const& bias, math::vec3f const& scale);
	bool set_stream_all_messages(bool enabled);
	bool set_stream_message(RX_Message message, bool enabled);
	bool set_send_all_message_once();
	bool set_send_message_once(RX_Message message);
	bool reset_uav_inertial_frame();

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
	std::array<uint8_t, 2560> m_serial_buffer;

	std::mutex m_rx_buffer_mutex;
	std::vector<uint8_t> m_rx_buffer;

	std::mutex m_tx_buffer_mutex;
	std::vector<uint8_t> m_tx_buffer;

	//each has a corresponding receive method
	enum class TX_Message : uint8_t
	{
		NONE = 0,

		//control messages
		STREAM_MESSAGE = 10, //the message will be streamed continuously
		STREAM_ALL_MESSAGES = 11, //all messages will be streamed continuously
		SEND_MESSAGE_ONCE = 12, //the message will be sent once
		SEND_ALL_MESSAGES_ONCE = 13, //all message will be sent once

		//calibration
		SET_BOARD_ACCELEROMETER_BIAS_SCALE = 20,
		SET_BOARD_GYROSCOPE_BIAS = 21,

		//UAV
		RESET_UAV_INERTIAL_FRAME = 30,
	};

	struct Message
	{
		RX_Message msg;
		uint16_t crc;
		std::vector<uint8_t> payload;
	};

	void handle_read(const boost::system::error_code& e, size_t bytes_transferred);

	void start_tx_message(TX_Message msg);
	uint16_t flush_tx_message();

	boost::optional<Message> decode_message(bool& needs_more_data);
	bool wait_for_response(uint16_t expected_crc, std::chrono::high_resolution_clock::duration timeout);

	void process_rx_message(Message const& message);

	std::string m_uav_message;
	uint16_t m_uav_version;
};

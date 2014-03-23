#pragma once

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

	template<class Payload>
	struct Data
	{
		Data() : received_frame_idx(0), is_valid(false), value() {}
		uint32_t received_frame_idx;
		bool is_valid;
		Payload value;
	};

	Data<uint8_t> data_board_cpu_usage;
	Data<uint32_t> data_board_time_ms;
	Data<std::vector<int16_t>> data_board_rc_in;
	Data<std::vector<int16_t>> data_board_pwm_out;
	Data<math::vec3f> data_board_gyroscope;
	Data<math::vec3f> data_board_accelerometer;
	Data<float> data_board_temperature;
	Data<float> data_board_baro_pressure;
	Data<float> data_board_sonar_altitude;
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
	std::array<uint8_t, 128> m_serial_buffer;

	std::mutex m_buffer_mutex;
	std::vector<uint8_t> m_buffer;

	void serial_callback(const boost::system::error_code& error, size_t bytes_transferred);
	void read_async();
	//void process_hello_world();
	bool process_message();
	uint16_t compute_crc(uint8_t const* data, size_t size) const;

	bool m_is_frame_started;

	std::string m_uav_message;
	uint16_t m_uav_version;

	uint32_t m_last_frame_idx;
};

#pragma once

class Video_Client
{
public:
    Video_Client(boost::asio::io_service& io_service, boost::asio::ip::address address, uint16_t port);
	~Video_Client();

public:
	void process();

    bool get_frame(uint32_t& frame_idx, std::vector<uint8_t>& data) const;

	void set_max_bitrate(uint32_t max_bitrate);
	void set_shutter_speed(std::chrono::microseconds speed);
	void set_quantization(uint32_t quantization);
	void set_intra_period(uint32_t period);
	void set_iso(uint32_t iso);
	void set_mtu(size_t size);

private:
    void start_receiving();
    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred);
	void connect();
	void disconnect();

    uint16_t m_port = 0;
	boost::asio::io_service& m_io_service;
	boost::asio::ip::address m_server_address;

    boost::asio::ip::udp::endpoint m_server_endpoint;
    boost::asio::ip::udp::endpoint m_receive_endpoint;
    std::shared_ptr<boost::asio::ip::udp::socket> m_socket;

	bool m_is_connected = false;

	//mutable Frame m_frame;

	uint32_t m_last_frame_idx = 0;

    std::vector<uint8_t> m_rx_buffer;

	struct Frame_Header
	{
        enum
        {
            FLAG_KEYFRAME = 1 << 0,
        };

        uint32_t frame_idx = 0;
        uint32_t frame_size = 0;
        uint8_t flags = 0;
        uint8_t packet_count = 0;
    };

	struct Frame_Packet
	{
		std::vector<uint8_t> data;
	};

	struct Fragmented_Frame_Data
	{
		Frame_Header header;
		bool has_received_header = false;
		size_t received_packet_count = 0;
		std::vector<Frame_Packet> packets;
	};

	struct Frames
	{
		mutable std::mutex mutex;
		mutable std::map<uint32_t, Fragmented_Frame_Data> fragmented_frame_datas;
		mutable uint32_t last_served_idx = 0;
		mutable q::Clock::time_point last_served_frame_time;
		mutable q::Clock::time_point last_valid_data_time;
	} m_frames;
};



#pragma once

namespace silk
{
	//////////////////////////////////////////////////////////////////////////

	class Video_Server : q::util::Noncopyable
	{
	public:
        Video_Server(boost::asio::io_service& io_service);

        enum class Result
        {
            OK,
            FAILED
        };

        auto start(boost::asio::ip::address const& address, uint16_t port) -> Result;
        void stop();
        auto is_started() const -> bool;

		void process();

        enum class Flag : uint8_t
        {
            FLAG_KEYFRAME = 1 << 0,
        };
        typedef q::util::Flag_Set<Flag, uint8_t> Flags;

        //sends a video frame.
        //The data needs to be alive only for the duration of this call.
        auto send_frame(Flags flags, uint8_t const* data, size_t size) -> Result;

	private:
        struct Frame_Header
        {
            uint32_t frame_idx = 0;
            uint32_t frame_size = 0;
            uint8_t flags = 0;
            uint8_t packet_count = 0;
        };

		boost::asio::io_service& m_io_service;
        uint16_t m_port = 0;

        std::shared_ptr<boost::asio::ip::udp::socket> m_socket;
        boost::asio::ip::udp::endpoint m_endpoint;

		struct Frame_Packet
		{
			std::vector<uint8_t> data;
		};
		std::vector<std::shared_ptr<Frame_Packet>> m_frame_packets;
		size_t m_mtu = 1024;
		std::mutex m_frame_packet_mutex;

        uint32_t m_last_encoded_frame_idx = 0;

		void handle_frame_packet_send(std::shared_ptr<Frame_Packet> frame_packet, const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/);
	};

}


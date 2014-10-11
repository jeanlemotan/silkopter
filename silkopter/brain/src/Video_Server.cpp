#include "BrainStdAfx.h"
#include "Video_Server.h"

using namespace silk;
using namespace boost::asio;

//////////////////////////////////////////////////////////////////////////

static const size_t FRAME_PACKET_POLL_SIZE = 100;


Video_Server::Video_Server(boost::asio::io_service& io_service)
	: m_io_service(io_service)
{
	for (size_t i = 0; i < FRAME_PACKET_POLL_SIZE; i++)
	{
		m_frame_packets.push_back(std::make_shared<Frame_Packet>());
    }

    //setup_capture();

    SILK_INFO("Video server created");
}

auto Video_Server::start(boost::asio::ip::address const& address, uint16_t port) -> bool
{
    stop();
    try
    {
        m_port = port;
        m_socket = std::make_shared<ip::udp::socket>(m_io_service, ip::udp::endpoint(ip::udp::v4(), m_port));
        m_endpoint = ip::udp::endpoint(address, m_port);
    }
    catch(...)
    {
        SILK_WARNING("Cannot connect to {}:{}", address.to_string(), port);
        return false;
    }

    SILK_INFO("Connected to {}:{}", m_endpoint.address().to_string(), m_port);
    return true;
}

void Video_Server::stop()
{
    m_socket.reset();
}

auto Video_Server::is_started() const -> bool
{
    return m_socket != nullptr;
}

template<class T>
static void set_value(std::vector<uint8_t>& t, T const& val, size_t& off)
{
	if (off + sizeof(T) > t.size())
	{
		t.resize(off + sizeof(T));
	}
	*reinterpret_cast<T*>(t.data() + off) = val;
	off += sizeof(T);
}

auto Video_Server::send_frame(Flags flags, uint8_t const* data, size_t data_size) -> bool
{
    if (data_size == 0)
    {
        return true;
    }
    if (!m_socket)
    {
        SILK_WARNING("Cannot send frame as the streamer is not connected");
        return false;
    }

    uint32_t frame_idx = ++m_last_encoded_frame_idx;

    static const size_t max_packet_buffer_size = 255;
    std::shared_ptr<Frame_Packet> local_packet_buffer[max_packet_buffer_size];

	Frame_Header header;
	header.frame_idx = frame_idx;
    header.packet_count = 0; //we'll write later
	header.frame_size = static_cast<uint32_t>(data_size);
    header.flags = flags.value();

    bool failed = false;
	uint8_t packet_idx = 0;
    size_t header_off = 0;
	while (data_size > 0)
	{
        if (packet_idx >= max_packet_buffer_size)
        {
            failed = true;
            break;
        }

        std::shared_ptr<Frame_Packet> frame_packet;
        {
            std::lock_guard<std::mutex> lg(m_frame_packet_mutex);
            if (m_frame_packets.empty())
            {
                failed = true;
                break;
            }
            frame_packet = std::move(m_frame_packets.back());
            m_frame_packets.pop_back();
            local_packet_buffer[packet_idx] = frame_packet;
        }
        QASSERT(frame_packet);

		frame_packet->data.resize(m_mtu);

		size_t off = 0;
		set_value(frame_packet->data, frame_idx, off);
		set_value(frame_packet->data, packet_idx, off);
		auto payload_size_off = off;
		set_value(frame_packet->data, uint16_t(0), off); //payload size
		if (packet_idx == 0)
		{
            header_off = off; //we'll write here later
			set_value(frame_packet->data, header, off);
		}
        uint16_t payload_size = static_cast<uint16_t>(std::min<size_t>(frame_packet->data.size() - off, data_size));

		set_value(frame_packet->data, payload_size, payload_size_off); //payload size

		std::copy(data, data + payload_size, frame_packet->data.begin() + off);
		data += payload_size;
		data_size -= payload_size;

		frame_packet->data.resize(off + payload_size);

		packet_idx++;
	}

    if (failed)
    {
        std::lock_guard<std::mutex> lg(m_frame_packet_mutex);
        for (size_t i = 0; i < packet_idx; i++)
        {
            if (local_packet_buffer[i])
            {
                m_frame_packets.push_back(std::move(local_packet_buffer[i]));
            }
        }
        SILK_WARNING("skipping frame {} due to inssuficient packets ({})", frame_idx, packet_idx);
        return false;
    }
    else
    {
        QASSERT(packet_idx > 0);
        QASSERT(local_packet_buffer[0]);

        header.packet_count = packet_idx;
        set_value(local_packet_buffer[0]->data, header, header_off);
        //send
        for (size_t i = 0; i < packet_idx; i++)
        {
            m_socket->async_send_to(buffer(local_packet_buffer[i]->data), m_endpoint,
                                           boost::bind(&Video_Server::handle_frame_packet_send, this,
                                           local_packet_buffer[i],
                                           placeholders::error,
                                           placeholders::bytes_transferred));
        }
    }

    return true;
}

void Video_Server::handle_frame_packet_send(std::shared_ptr<Frame_Packet> frame_packet, const boost::system::error_code& error, std::size_t /*bytes_transferred*/)
{
	if (error)
	{
        q::quick_logf("send error: {}", error.message());
	}
	else
	{
		//SILK_INFO("Done sending frame {x}", frame_data.get());
	}

	{
		std::lock_guard<std::mutex> lg(m_frame_packet_mutex);
		m_frame_packets.push_back(frame_packet);
	}
}

void Video_Server::process()
{
}

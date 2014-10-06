#include "stdafx.h"
#include "Video_Client.h"

using namespace boost::asio;

//////////////////////////////////////////////////////////////////////////

Video_Client::Video_Client(io_service& io_service, ip::address address, uint16_t port)
    : m_port(port)
    , m_io_service(io_service)
    , m_server_address(address)
{
    m_rx_buffer.resize(100 * 1024);
}

Video_Client::~Video_Client()
{
    m_socket.reset();
}

void Video_Client::connect()
{
    try
    {
        m_socket.reset(new ip::udp::socket(m_io_service, ip::udp::endpoint(ip::udp::v4(), m_port)));
        m_socket->set_option(socket_base::receive_buffer_size(8192));
        m_server_endpoint = ip::udp::endpoint(m_server_address, m_port);
    }
    catch(std::exception const& e)
    {
        SILK_WARNING("Cannot connect to {}:{} : {}", m_server_address.to_string(), m_port, e.what());
        return;
    }

    SILK_INFO("Connected to {}:{}", m_server_address.to_string(), m_port);

    m_is_connected = true;

    start_receiving();
}

void Video_Client::disconnect()
{
    m_socket.reset();
	m_is_connected = false;

	SILK_INFO("Disonnected from {}", m_server_address.to_string());
}

void Video_Client::start_receiving()
{
    m_socket->async_receive_from(
        buffer(m_rx_buffer), m_receive_endpoint,
        boost::bind(&Video_Client::handle_receive, this,
		placeholders::error,
		placeholders::bytes_transferred));
}

template<class T>
static T get_value(const std::vector<uint8_t>& t, size_t& off)
{
	T val = *reinterpret_cast<const T*>(t.data() + off);
	off += sizeof(T);
	return val;
}


void Video_Client::handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	auto now = q::Clock::now();

	if (!error)
	{
		const size_t header_size = sizeof(uint32_t)+sizeof(uint8_t);

		if (bytes_transferred < header_size + 1)
		{
			SILK_WARNING("Bad video packet received. Size is too small: {} bytes", bytes_transferred);
		}
		else
		{
			size_t off = 0;
            auto frame_idx = get_value<uint32_t>(m_rx_buffer, off);
            auto packet_idx = get_value<uint8_t>(m_rx_buffer, off);
            auto payload_size = get_value<uint16_t>(m_rx_buffer, off);

			if (frame_idx < m_frames.last_served_idx)
			{
				SILK_WARNING("skipping past packet from: frame {} packet {}", frame_idx, packet_idx);
			}
			else if (bytes_transferred < payload_size)
			{
				SILK_WARNING("skipping corrupted packet from: frame {} packet {}", frame_idx, packet_idx);
			}
			else
			{
				std::lock_guard<std::mutex> lg(m_frames.mutex);

				//SILK_INFO("received packet: frame {} packet {}", frame_idx, packet_idx);

				auto& frame_data = m_frames.fragmented_frame_datas[frame_idx];
				auto& packets = frame_data.packets;
				packets.resize(math::max<size_t>(packets.size(), packet_idx + 1));

				if (!packets[packet_idx].data.empty())
				{
					SILK_WARNING("skipping duplicate packet from: frame {} packet {}", frame_idx, packet_idx);
				}
				else
				{
					if (packet_idx == 0) //the frame header
					{
						frame_data.has_received_header = true;
                        frame_data.header = get_value<Frame_Header>(m_rx_buffer, off);
						QASSERT(frame_data.header.frame_idx == frame_idx);
						if (frame_data.header.frame_size < bytes_transferred - off)
						{
                            //int a = 0;
						}
					}

					frame_data.received_packet_count++;

					packets[packet_idx].data.resize(payload_size);
                    std::copy(m_rx_buffer.begin() + off, m_rx_buffer.begin() + off + payload_size, packets[packet_idx].data.begin());

					m_frames.last_valid_data_time = now;
				}
			}

			auto ellapsed = now - m_frames.last_valid_data_time;
			if (ellapsed > std::chrono::milliseconds(1000))
			{
				m_frames.last_served_idx = 0;
			}
		}

        start_receiving();
	}
    else
    {
        SILK_ERR("error receiving packet: {}", error.message());
    }
}

void Video_Client::process()
{
	if (!m_is_connected)
	{
		connect();
	}
}

static size_t k_max_allowed_queued_frames(3);

bool Video_Client::get_frame(uint32_t& o_frame_idx, std::vector<uint8_t>& o_data) const
{
	auto now = q::Clock::now();

	std::lock_guard<std::mutex> lg(m_frames.mutex);
    if (m_frames.fragmented_frame_datas.size() < 1)
	{
		return false;
	}

	//SILK_INFO("pending frames {}", m_frames.fragmented_frame_datas.size());
	//for (auto it = m_frames.fragmented_frame_datas.rbegin(); it != m_frames.fragmented_frame_datas.rend(); ++it)
	for (auto it = m_frames.fragmented_frame_datas.begin(); it != m_frames.fragmented_frame_datas.end(); ++it)
	{
        auto frame_idx = it->first;
        auto& frame_data = it->second;
        bool is_frame_ready = true;

		if (!frame_data.has_received_header)
		{
            is_frame_ready = false;
		}
		if (frame_data.received_packet_count != frame_data.header.packet_count)
		{
            is_frame_ready = false;
		}

        //if the frame is not ready and we still have some time left, wait a bit more
        if (!is_frame_ready)
        {
            if (m_frames.fragmented_frame_datas.rbegin()->first - frame_idx < k_max_allowed_queued_frames)
            {
                return false;
            }
            else
            {
                continue;
            }
        }


        o_frame_idx = frame_idx;

        o_data.resize(0);
        o_data.reserve(frame_data.header.frame_size);
		for (size_t i = 0; i < frame_data.header.packet_count; i++)
		{
            std::copy(frame_data.packets[i].data.begin(), frame_data.packets[i].data.end(), std::back_inserter(o_data));
		}

		while (!m_frames.fragmented_frame_datas.empty() && m_frames.fragmented_frame_datas.begin()->first <= frame_idx)
		{
			//SILK_INFO("deleting incomplete frames");
			m_frames.fragmented_frame_datas.erase(m_frames.fragmented_frame_datas.begin());
		}

		m_frames.last_served_idx = frame_idx;
		m_frames.last_served_frame_time = now;
		return true;
	}

	return false;
}





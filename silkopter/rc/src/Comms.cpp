#include "Comms.h"

#include "common/node/INode.h"
#include "common/node/IPilot.h"

#include "common/Comm_Data.h"

#include "utils/comms/UDP_Socket.h"
#include "utils/comms/RFMON_Socket.h"
#include "utils/comms/RF4463F30_Socket.h"

#include "settings.def.h"

namespace silk
{

//constexpr char const* VIDEO_INTERFACE1 = "wlan2";
//constexpr char const* VIDEO_INTERFACE2 = "wlan3";

//constexpr uint8_t RC_SDN_GPIO = 6;
//constexpr uint8_t RC_NIRQ_GPIO = 26;
//constexpr char const* RC_SPI_DEVICE = "/dev/spidev0.0";
//constexpr size_t RC_SPEED = 16000000;

extern settings::Settings s_settings;

Comms::Comms()
    : m_rc(true)
    , m_video_streamer()
{
}

auto Comms::start() -> bool
{
    disconnect();

    settings::Settings::Comms const& comms = s_settings.get_comms();

    util::comms::Video_Streamer::RX_Descriptor rx_descriptor;
    rx_descriptor.interfaces = comms.get_video_interfaces();
    rx_descriptor.coding_k = comms.get_video_coding_k();
    rx_descriptor.coding_n = comms.get_video_coding_n();
    rx_descriptor.max_latency = std::chrono::milliseconds(comms.get_video_max_latency_ms());
    rx_descriptor.reset_duration = std::chrono::milliseconds(comms.get_video_reset_duration_ms());

    try
    {
        m_is_connected = m_rc.init(comms.get_rc_spi_device(), comms.get_rc_spi_speed(), comms.get_rc_sdn_gpio(), comms.get_rc_nirq_gpio())
                && m_video_streamer.init_rx(rx_descriptor);
    }
    catch(std::exception e)
    {
        m_is_connected = false;
    }

    if (!m_is_connected)
    {
        QLOGW("Cannot start comms");
        return false;
    }

    m_video_streamer.on_data_received = std::bind(&Comms::handle_video, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    QLOGI("Started receiving video");

    return true;
}

void Comms::disconnect()
{
    reset();
    m_is_connected = false;
}

auto Comms::is_connected() const -> bool
{
    return m_is_connected;//m_rcp != nullptr && m_rcp->is_connected();
}


void Comms::reset()
{
    m_last_req_id = 0;
}

void Comms::handle_multirotor_state()
{
//    auto& channel = m_pilot_channel;

//    stream::IMultirotor_State::Sample sample;
//    if (!channel.unpack_param(sample))
//    {
//        QLOGE("Error in unpacking multirotor state");
//        return;
//    }

//    std::lock_guard<std::mutex> lg(m_samples_mutex);
//    m_multirotor_state_samples.push_back(sample);
}
void Comms::handle_video(void const* _data, size_t size, math::vec2u16 const& resolution)
{
    std::lock_guard<std::mutex> lg(m_samples_mutex);

    if (resolution != m_video_resolution)
    {
        m_video_resolution = resolution;
        m_video_data.clear();
    }

    if (size > 0)
    {
        uint8_t const* data = reinterpret_cast<uint8_t const*>(_data);
        size_t offset = m_video_data.size();
        m_video_data.resize(offset + size);
        memcpy(m_video_data.data() + offset, data, size);

        //keep the buffer from growing too much
        constexpr size_t MAX_VIDEO_DATA_SIZE = 1024*1024;
        if (m_video_data.size() > MAX_VIDEO_DATA_SIZE)
        {
            size_t del = m_video_data.size() - MAX_VIDEO_DATA_SIZE;
            m_video_data.erase(m_video_data.begin(), m_video_data.begin() + del);
        }
    }

//    FILE* f = fopen("a.h264", "a+");
//    fwrite(_data, size, 1, f);
//    fclose(f);
}

int8_t Comms::get_rx_dBm() const
{
    return m_rc_data.rx_dBm;
}
int8_t Comms::get_tx_dBm() const
{
    return m_rc_data.tx_dBm;
}

q::Clock::time_point Comms::get_last_rx_tp() const
{
    return m_rc_data.rx_timepoint;
}
q::Clock::time_point Comms::get_last_tx_tp() const
{
    return m_rc_data.tx_timepoint;
}


void Comms::get_video_data(std::vector<uint8_t>& dst, math::vec2u16& resolution)
{
    std::lock_guard<std::mutex> lg(m_samples_mutex);
    size_t offset = dst.size();
    size_t size = m_video_data.size();
    if (size > 0)
    {
        dst.resize(offset + size);
        memcpy(dst.data() + offset, m_video_data.data(), size);
        m_video_data.clear();
    }
    resolution = m_video_resolution;
}
stream::IMultirotor_State::Value Comms::get_multirotor_state() const
{
    std::lock_guard<std::mutex> lg(m_samples_mutex);
    return m_multirotor_state;
}
void Comms::send_multirotor_commands_value(stream::IMultirotor_Commands::Value const& value)
{
    size_t off = 0;
    util::serialization::serialize(m_serialization_buffer, value, off);

    m_rc.set_tx_data(m_serialization_buffer.data(), m_serialization_buffer.size());

    //m_pilot_channel.pack_all(silk::rc_comms::Pilot_Message::MULTIROTOR_COMMANDS, value);
    //m_pilot_channel.try_sending(*m_rcp);
}

void Comms::process()
{
    m_video_streamer.process();

    m_rc.get_rx_data(m_rc_data);
    if (!m_rc_data.rx_data.empty())
    {
        size_t off = 0;
        stream::IMultirotor_State::Value value;
        if (util::serialization::deserialize(m_rc_data.rx_data, value, off))
        {
            std::lock_guard<std::mutex> lg(m_samples_mutex);
            m_multirotor_state = value;
        }
        else
        {
            QLOGW("Cannot deserialize incoming multirotor state value");
        }
    }
}

}

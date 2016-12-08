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
auto Comms::get_multirotor_state_samples() -> std::vector<stream::IMultirotor_State::Sample>
{
    std::lock_guard<std::mutex> lg(m_samples_mutex);
    std::vector<stream::IMultirotor_State::Sample> samples = std::move(m_multirotor_state_samples);
    m_multirotor_state_samples.clear();
    return samples;
}
void Comms::send_multirotor_commands_value(stream::IMultirotor_Commands::Value const& value)
{
    size_t off = 0;
    util::serialization::serialize(m_rc_data, value, off);

    m_rc.set_tx_data(m_rc_data.data(), m_rc_data.size());

    //m_pilot_channel.pack_all(silk::rc_comms::Pilot_Message::MULTIROTOR_COMMANDS, value);
    //m_pilot_channel.try_sending(*m_rcp);
}

void Comms::process()
{
    m_video_streamer.process();
//    {
//        std::lock_guard<std::mutex> lg(m_samples_mutex);
//        m_multirotor_state_samples.clear();
//        m_video_samples.clear();
//    }

//    static q::Clock::time_point s_tp = q::Clock::now();
//    if (q::Clock::now() - s_tp >= std::chrono::milliseconds(50))
//    {
//        s_tp = q::Clock::now();

//        struct Data
//        {
//            int8_t throttle = 0;
//            int8_t yaw = 0;
//            int8_t pitch = 0;
//            int8_t roll = 0;
//        };

//        static Data tx_data;
//        tx_data.throttle++;

//        m_rc.set_tx_data(&tx_data, sizeof(tx_data));

//        Data rx_data;
//        util::comms::RC::Data rc_rx_data;
//        m_rc.get_rx_data(rc_rx_data);
//        if (rc_rx_data.rx_data.size() == sizeof(Data))
//        {
//            rx_data = *reinterpret_cast<Data const*>(rc_rx_data.rx_data.data());
//        }

//        QLOGI("^{}dBm, v{}dBm, yaw: {}", rc_rx_data.tx_dBm, rc_rx_data.tx_dBm, rx_data.yaw);
//    }

//    while (m_pilot_channel.get_next_message(*m_rcp))
//    {
//        rc_comms::Pilot_Message msg;
//        if (!m_pilot_channel.begin_unpack() || !m_pilot_channel.unpack_param(msg))
//        {
//            QASSERT(0);
//            m_pilot_channel.end_unpack();
//            break;
//        }

//        switch (msg)
//        {
//        case rc_comms::Pilot_Message::MULTIROTOR_STATE : handle_multirotor_state(); break;
//        default: break;
//        }

//        m_pilot_channel.end_unpack();
//    }
//    while (m_video_channel.get_next_message(*m_rcp))
//    {
//        rc_comms::Video_Message msg;
//        if (!m_video_channel.begin_unpack() || !m_video_channel.unpack_param(msg))
//        {
//            QASSERT(0);
//            m_video_channel.end_unpack();
//            break;
//        }

//        switch (msg)
//        {
//        case rc_comms::Video_Message::FRAME_DATA : handle_video(); break;
//        default: break;
//        }
//    }

//    auto start = q::Clock::now();
//    while (m_telemetry_channel.get_next_message(*m_rcp))
//    {
//        //process only the first 10 ms worh of data and discard the rest
//        if (q::Clock::now() - start < std::chrono::milliseconds(100))
//        {
////            switch (msg.get())
////            {
//////            case comms::Telemetry_Message::STREAM_DATA : handle_stream_data(); break;
////            default: break;
////            }
//        }
//    }

//    m_telemetry_channel.try_sending(*m_rcp);
}

}

#pragma once

#include "common/Comm_Data.h"

#include "common/stream/IAcceleration.h"
#include "common/stream/IADC.h"
#include "common/stream/IFloat.h"
#include "common/stream/IBool.h"
#include "common/stream/IAngular_Velocity.h"
#include "common/stream/IBattery_State.h"
#include "common/stream/ICurrent.h"
#include "common/stream/IDistance.h"
#include "common/stream/IForce.h"
#include "common/stream/IFrame.h"
#include "common/stream/IGPS_Info.h"
#include "common/stream/ILinear_Acceleration.h"
#include "common/stream/IPosition.h"
#include "common/stream/IMagnetic_Field.h"
#include "common/stream/IPressure.h"
#include "common/stream/IPWM.h"
#include "common/stream/ITemperature.h"
#include "common/stream/IThrottle.h"
#include "common/stream/ITorque.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IVideo.h"
#include "common/stream/IVoltage.h"
#include "common/stream/IProximity.h"
#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"

#include "utils/RCP.h"
#include "utils/Channel.h"
#include "common/Manual_Clock.h"

#include "common/config/UAV_Config.h"

#include "common/node/INode.h"
#include "common/stream/IVideo.h"
#include "common/stream/IMultirotor_State.h"

#include <boost/asio.hpp>



namespace silk
{

class Comms : q::util::Noncopyable
{
public:
    Comms();

    auto start_udp(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> bool;
    auto start_rfmon(std::string const& interface, uint8_t id) -> bool;

    void disconnect();
    auto is_connected() const -> bool;

    //----------------------------------------------------------------------

    typedef util::Channel<comms::Setup_Message, uint32_t> Setup_Channel;
    typedef util::Channel<comms::Pilot_Message, uint32_t> Pilot_Channel;
    typedef util::Channel<comms::Video_Message, uint32_t> Video_Channel;
    typedef util::Channel<comms::Telemetry_Message, uint32_t> Telemetry_Channel;

    auto get_setup_channel() -> Setup_Channel&;

    auto get_video_samples() const -> std::vector<stream::IVideo::Sample> const&;
    auto get_multirotor_state_samples() const -> std::vector<stream::IMultirotor_State::Sample> const&;
    void send_multirotor_commands_value(stream::IMultirotor_Commands::Value const& value);

    //----------------------------------------------------------------------


    void add_node(std::string const& def_name, std::string const& name, rapidjson::Document const& init_params);
    void remove_node(std::string const& name);
    void set_node_input_stream_path(std::string const& name, std::string const& input_name, q::Path const& stream_path);
    void set_node_config(std::string const& name, rapidjson::Document const& config);
    void set_stream_telemetry_active(q::Path const& stream_path, bool active);
    void send_node_message(std::string const& name, rapidjson::Document const& json);

    //----------------------------------------------------------------------


    q::util::Signal<void()> sig_reset;
    q::util::Signal<void(Manual_Clock::time_point)> sig_clock_received;


    void request_all_data();

    struct Node_Def
    {
        std::string name;
        node::Type type;
        std::string default_init_params_json;
        struct Input
        {
            stream::Type type;
            std::string name;
            uint32_t rate = 0;
        };
        std::vector<Input> inputs;
        struct Output
        {
            stream::Type type;
            std::string name;
            uint32_t rate = 0;
        };
        std::vector<Output> outputs;
    };

    q::util::Signal<void()> sig_node_defs_reset;
    q::util::Signal<void(std::vector<Node_Def> const&)> sig_node_defs_added;

    struct Node
    {
        std::string name;
        node::Type type;
        std::string init_params_json;
        std::string config_json;

        struct Input
        {
            q::Path stream_path;
            stream::Type type;
            std::string name;
            uint32_t rate = 0;
        };
        std::vector<Input> inputs;
        struct Output
        {
            stream::Type type;
            std::string name;
            uint32_t rate = 0;
        };
        std::vector<Output> outputs;
    };

    q::util::Signal<void()> sig_nodes_reset;
    q::util::Signal<void(std::vector<Node> const&)> sig_nodes_added;
    q::util::Signal<void(Node const&)> sig_node_changed;
    q::util::Signal<void(std::string const& name)> sig_node_removed;
    q::util::Signal<void(std::string const& name, rapidjson::Value const& message)> sig_node_message_received;
    q::util::Signal<void(std::string const& name, rapidjson::Value const& json)> sig_node_config_received;

    struct IStream_Data
    {
        stream::Semantic type;
        uint32_t rate = 0;
        virtual void unpack(Comms::Telemetry_Channel& channel, uint32_t sample_count) = 0;
    };

    template<class Stream_T>
    struct Stream_Data : public IStream_Data
    {
        static constexpr stream::Semantic TYPE = Stream_T::TYPE;
        typedef typename Stream_T::Value Value;
        typedef typename Stream_T::Sample Sample;
        typedef std::vector<Sample> Samples;

        Samples samples;
    private:
        void unpack(Comms::Telemetry_Channel& channel, uint32_t sample_count) override;
    };

    q::util::Signal<void(IStream_Data const&)> sig_stream_data_received;


    void request_node_config(std::string const& name);

    void request_uav_config();
    q::util::Signal<void(boost::optional<silk::UAV_Config&> config)> sig_uav_config_received;
    void send_uav_config(boost::optional<silk::UAV_Config&> config);

    void process_rcp();
    void process();

private:
    void configure_channels();

    std::map<stream::Type, std::unique_ptr<IStream_Data>> m_streams;


    void reset();
    bool m_did_request_data = false;

    std::shared_ptr<util::RCP_Socket> m_socket;
    std::shared_ptr<util::RCP> m_rcp;

    mutable Setup_Channel m_setup_channel;
    mutable Pilot_Channel m_pilot_channel;
    mutable Video_Channel m_video_channel;
    mutable Telemetry_Channel m_telemetry_channel;

    std::vector<stream::IVideo::Sample> m_video_samples;
    std::vector<stream::IMultirotor_State::Sample> m_multirotor_state_samples;

    auto unpack_node_data(Comms::Setup_Channel& channel, Node& node) -> bool;
    auto publish_streams(Node const& node) -> bool;
    auto unpublish_streams(Node const& node) -> bool;

    void handle_stream_data();
    void handle_frame_data();

    void handle_uav_config();
    void handle_multirotor_state();
    void handle_video();

    void handle_clock();
    void handle_enumerate_nodes();
    void handle_enumerate_node_defs();
    void handle_get_node_data();

    void handle_streams_telemetry_active();

    void handle_add_node();
    void handle_remove_node();
    void handle_node_message();
    void handle_node_config();
    void handle_node_input_stream_path();
};

}

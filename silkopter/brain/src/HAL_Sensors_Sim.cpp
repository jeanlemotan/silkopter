#include "BrainStdAfx.h"
#include "HAL_Sensors_Sim.h"
#include "utils/Json_Helpers.h"

#ifndef RASPBERRY_PI

using namespace silk;
using namespace boost::asio;

IO_Board_Sim::IO_Board_Sim(io_service& io_service)
    : m_io_service(io_service)
    , m_socket(io_service)
    , m_channel(m_socket)
{
    load_settings();
}

auto IO_Board_Sim::load_settings() -> bool
{
    q::data::File_Source fs(q::Path("io_board.cfg"));
    if (!fs.is_open())
    {
        return false;
    }

    auto size = fs.get_size();

    std::vector<char> data(size + 1);
    auto result = fs.read(reinterpret_cast<uint8_t*>(data.data()), size);
    if (result != size)
    {
        SILK_WARNING("Failed to read settings.");
        return false;
    }

    data[size] = 0;
    if (m_settings.document.Parse<0>(data.data()).HasParseError())
    {
        SILK_WARNING("Failed to parse settings: {}:{}", m_settings.document.GetErrorOffset(), m_settings.document.GetParseError());
        return false;
    }

    auto finder = jsonutil::Member_Finder(m_settings.document, "calibration_config");
    if (!finder.is_valid())
    {
        SILK_WARNING("Cannot find root");
        return false;
    }
    auto& calibration_config = finder.get_value();

//    finder = jsonutil::Member_Finder(m_settings.document, "motor_config");
//    if (!finder.is_valid())
//    {
//        SILK_WARNING("Cannot find root");
//        return false;
//    }
//    auto& motor_config = finder.get_value();

//    finder = jsonutil::Member_Finder(m_settings.document, "pid_config");
//    if (!finder.is_valid())
//    {
//        SILK_WARNING("Cannot find root");
//        return false;
//    }
//    auto& pid_config = finder.get_value();

    {
        auto& json = calibration_config;
        auto abias = jsonutil::get_vec3_value<float>(json, "accelerometer_bias");
        auto ascale = jsonutil::get_vec3_value<float>(json, "accelerometer_scale");
        auto gbias = jsonutil::get_vec3_value<float>(json, "gyroscope_bias");
        auto cbias = jsonutil::get_vec3_value<float>(json, "compass_bias");
        if (!abias || !ascale || !gbias || !cbias)
        {
            SILK_WARNING("Cannot read calibration config");
            return false;
        }
        m_calibration_config.accelerometer_bias = *abias;
        m_calibration_config.accelerometer_scale = *ascale;
        m_calibration_config.gyroscope_bias = *gbias;
        m_calibration_config.compass_bias = *cbias;
    }

//    {
//        auto& json = motor_config;
//        {
//            auto freq = jsonutil::get_uint_value(json, "frequency");
//            if (!freq || *freq > static_cast<uint32_t>(PWM_Frequency::_500HZ))
//            {
//                SILK_WARNING("Cannot read motor frequency");
//                return false;
//            }
//            m_req_motor_config.frequency = static_cast<PWM_Frequency>(*freq);

//            finder = jsonutil::Member_Finder(json, "motors");
//            if (!finder.is_type(rapidjson::kArrayType))
//            {
//                SILK_WARNING("Cannot read motors");
//                return false;
//            }
//            m_req_motor_config.motor_count = math::min<uint8_t>(MAX_MOTOR_COUNT, finder.get_value().Size());
//            if (m_req_motor_config.motor_count < 4 || (m_req_motor_config.motor_count & 1) == 1)
//            {
//                SILK_WARNING("To few motors / odd number of morors specified: {}", m_req_motor_config.motor_count);
//                return false;
//            }
//            for (uint8_t i = 0; i < m_req_motor_config.motor_count; i++)
//            {
//                auto& mj = finder.get_value()[i];
//                auto position = jsonutil::get_vec2_value<float>(mj, "position");
//                auto clockwise = jsonutil::get_bool_value(mj, "clockwise");
//                if (!position || !clockwise)
//                {
//                    SILK_WARNING("Cannot read motor {}", i);
//                    return false;
//                }
//                m_req_motor_config.motors[i].position = *position;
//                m_req_motor_config.motors[i].clockwise = *clockwise;
//            }
//        }
//        m_req_motor_config.id++;
//    }

//    {
//        auto& json = pid_config;
//        {
//            finder = jsonutil::Member_Finder(json, "yaw_rate");
//            if (!finder.is_type(rapidjson::kObjectType))
//            {
//                SILK_WARNING("Cannot read yaw_rate");
//                return false;
//            }
//            auto p = jsonutil::get_float_value(finder.get_value(), "p");
//            auto i = jsonutil::get_float_value(finder.get_value(), "p");
//            auto d = jsonutil::get_float_value(finder.get_value(), "p");
//            if (!p || !i || !d)
//            {
//                SILK_WARNING("Cannot read yaw_rate pid");
//                return false;
//            }
//            m_req_pid_config.yaw_rate.p = *p;
//            m_req_pid_config.yaw_rate.i = *i;
//            m_req_pid_config.yaw_rate.d = *d;
//        }
//        {
//            finder = jsonutil::Member_Finder(json, "pitch_rate");
//            if (!finder.is_type(rapidjson::kObjectType))
//            {
//                SILK_WARNING("Cannot read pitch_rate");
//                return false;
//            }
//            auto p = jsonutil::get_float_value(finder.get_value(), "p");
//            auto i = jsonutil::get_float_value(finder.get_value(), "p");
//            auto d = jsonutil::get_float_value(finder.get_value(), "p");
//            if (!p || !i || !d)
//            {
//                SILK_WARNING("Cannot read pitch_rate pid");
//                return false;
//            }
//            m_req_pid_config.pitch_rate.p = *p;
//            m_req_pid_config.pitch_rate.i = *i;
//            m_req_pid_config.pitch_rate.d = *d;
//        }
//        {
//            finder = jsonutil::Member_Finder(json, "roll_rate");
//            if (!finder.is_type(rapidjson::kObjectType))
//            {
//                SILK_WARNING("Cannot read roll_rate");
//                return false;
//            }
//            auto p = jsonutil::get_float_value(finder.get_value(), "p");
//            auto i = jsonutil::get_float_value(finder.get_value(), "p");
//            auto d = jsonutil::get_float_value(finder.get_value(), "p");
//            if (!p || !i || !d)
//            {
//                SILK_WARNING("Cannot read roll_rate pid");
//                return false;
//            }
//            m_req_pid_config.roll_rate.p = *p;
//            m_req_pid_config.roll_rate.i = *i;
//            m_req_pid_config.roll_rate.d = *d;
//        }

//        m_req_pid_config.id++;
//    }


    return true;
}
void IO_Board_Sim::save_settings()
{
    if (!m_settings.document.IsObject())
    {
        m_settings.document.SetObject();
    }

    auto& allocator = m_settings.document.GetAllocator();

    auto& calibration_config = jsonutil::get_or_add_member(m_settings.document, "calibration_config", rapidjson::kObjectType, allocator);
//    auto& motor_config = jsonutil::get_or_add_member(m_settings.document, "motor_config", rapidjson::kObjectType, allocator);
//    auto& pid_config = jsonutil::get_or_add_member(m_settings.document, "pid_config", rapidjson::kObjectType, allocator);

    {
        auto& json = calibration_config;
        jsonutil::set_vec3_value(json, "accelerometer_bias", m_calibration_config.accelerometer_bias, allocator);
        jsonutil::set_vec3_value(json, "accelerometer_scale", m_calibration_config.accelerometer_scale, allocator);
        jsonutil::set_vec3_value(json, "gyroscope_bias", m_calibration_config.gyroscope_bias, allocator);
        jsonutil::set_vec3_value(json, "compass_bias", m_calibration_config.compass_bias, allocator);
    }
//    {
//        auto& json = motor_config;
//        {
//            auto& vj = jsonutil::get_or_add_member(json, "frequency", rapidjson::kNumberType, allocator);
//            vj.SetInt(static_cast<int>(m_crt_motor_config.frequency));
//        }
//        {
//            auto& vj = jsonutil::get_or_add_member(json, "motors", rapidjson::kArrayType, allocator);
//            vj.Clear();
//            for (uint8_t i = 0; i < m_crt_motor_config.motor_count; i++)
//            {
//                rapidjson::Value v;
//                v.SetObject();
//                jsonutil::set_vec2_value(v, "position", m_crt_motor_config.motors[i].position, allocator);
//                v.AddMember("clockwise", m_crt_motor_config.motors[i].clockwise, allocator);
//                vj.PushBack(v, allocator);
//            }
//        }
//    }
//    {
//        auto& json = pid_config;
//        {
//            auto& vj = jsonutil::get_or_add_member(json, "yaw_rate", rapidjson::kObjectType, allocator);
//            jsonutil::set_float_value(vj, "p", m_crt_pid_config.yaw_rate.p, allocator);
//            jsonutil::set_float_value(vj, "i", m_crt_pid_config.yaw_rate.i, allocator);
//            jsonutil::set_float_value(vj, "d", m_crt_pid_config.yaw_rate.d, allocator);
//        }
//        {
//            auto& vj = jsonutil::get_or_add_member(json, "pitch_rate", rapidjson::kObjectType, allocator);
//            jsonutil::set_float_value(vj, "p", m_crt_pid_config.pitch_rate.p, allocator);
//            jsonutil::set_float_value(vj, "i", m_crt_pid_config.pitch_rate.i, allocator);
//            jsonutil::set_float_value(vj, "d", m_crt_pid_config.pitch_rate.d, allocator);
//        }
//        {
//            auto& vj = jsonutil::get_or_add_member(json, "roll_rate", rapidjson::kObjectType, allocator);
//            jsonutil::set_float_value(vj, "p", m_crt_pid_config.roll_rate.p, allocator);
//            jsonutil::set_float_value(vj, "i", m_crt_pid_config.roll_rate.i, allocator);
//            jsonutil::set_float_value(vj, "d", m_crt_pid_config.roll_rate.d, allocator);
//        }
//    }


    typedef rapidjson::UTF8<> JSON_Charset;
    typedef rapidjson::GenericStringBuffer<JSON_Charset> JSON_Buffer;
    typedef rapidjson::PrettyWriter<JSON_Buffer> JSON_Writer;

    JSON_Buffer buffer;
    JSON_Writer writer(buffer);
    m_settings.document.Accept(writer);

    q::data::File_Sink fs(q::Path("io_board.cfg"));
    if (!fs.is_open())
    {
        SILK_WARNING("Cannot open file to save settings");
        return;
    }

    fs.write(reinterpret_cast<uint8_t const*>(buffer.GetString()), buffer.GetSize());
}

auto IO_Board_Sim::connect() -> Connection_Result
{
    if (!is_disconnected())
    {
        return Connection_Result::OK;
    }

//#ifdef RASPBERRY_PI
//    try
//    {
//        std::string com_port = "/dev/ttyAMA0";
//        size_t baud = 500000;

//        m_port.open(com_port);
//        m_port.set_option(serial_port_base::baud_rate(baud));
//        m_port.set_option(serial_port_base::parity(serial_port_base::parity::none));
//        m_port.set_option(serial_port_base::character_size(8));
//        m_port.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));
//        m_port.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));

//        m_port_adapter.start();
//    }
//    catch (...)
//    {
//        return Connection_Result::FAILED;
//    }
//#else
    uint16_t port = 52523;
    try
    {
        m_acceptor = std::make_unique<ip::tcp::acceptor>(m_io_service, ip::tcp::endpoint(ip::tcp::v4(), port));
        m_acceptor->set_option(ip::tcp::acceptor::reuse_address(true));
        m_acceptor->async_accept(m_socket,
                boost::bind(&IO_Board_Sim::handle_accept, this, boost::asio::placeholders::error));
    }
    catch(...)
    {
        SILK_WARNING("Cannot start listening on port {}", port);
        return Connection_Result::FAILED;
    }

    SILK_INFO("Started listening on port {}", port);
    while (!m_socket.is_open())
    {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
        SILK_INFO("Waiting for sim to connect");
    }
//#endif

    set_state(State::HANDSHAKE);
    return Connection_Result::OK;
}

//#ifndef RASPBERRY_PI
void IO_Board_Sim::handle_accept(boost::system::error_code const& error)
{
    if (error)
    {
        SILK_WARNING("Error occured while accepting connection: {}", error.message());
        return;
    }

    m_channel.start();
    SILK_INFO("Connected to {}:{}", m_socket.remote_endpoint().address().to_string(), m_socket.remote_endpoint().port());
}
//#endif

void IO_Board_Sim::disconnect()
{
//#ifdef RASPBERRY_PI
//    try
//    {
//        m_port.close();
//    }
//    catch (...)
//    {
//    }
//#else
    m_acceptor.reset();
//#endif

    set_state(State::DISCONNECTED);
}
bool IO_Board_Sim::is_disconnected() const
{
    return m_state == State::DISCONNECTED;
}
bool IO_Board_Sim::is_running() const
{
    return m_state == State::RUNNING;
}

void IO_Board_Sim::set_motor_throttles(float const* throttles, size_t count)
{
    QASSERT(throttles != nullptr);
    QASSERT(count <= MAX_MOTOR_COUNT);
    if (!throttles)
    {
        return;
    }

    count = math::min(count, MAX_MOTOR_COUNT);
    if (count > m_motor_outputs.size())
    {
        m_motor_outputs.resize(count);
    }

    for (size_t i = 0; i < count; i++)
    {
        m_motor_outputs[i] = throttles[i];
    }
}

void IO_Board_Sim::set_camera_rotation(math::quatf const& rot)
{

}

void IO_Board_Sim::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
{
    if (m_state >= State::RUNNING)
    {
        m_calibration_config.accelerometer_bias = bias;
        m_calibration_config.accelerometer_scale = scale;
    }
}
void IO_Board_Sim::get_accelerometer_calibration_data(math::vec3f &bias, math::vec3f &scale) const
{
    bias = m_calibration_config.accelerometer_bias;
    scale = m_calibration_config.accelerometer_scale;
}

void IO_Board_Sim::set_gyroscope_calibration_data(math::vec3f const& bias)
{
    if (m_state >= State::RUNNING)
    {
        m_calibration_config.gyroscope_bias = bias;
    }
}
void IO_Board_Sim::get_gyroscope_calibration_data(math::vec3f &bias) const
{
    bias = m_calibration_config.gyroscope_bias;
}

void IO_Board_Sim::set_compass_calibration_data(math::vec3f const& bias)
{
    if (m_state >= State::RUNNING)
    {
        m_calibration_config.compass_bias = bias;
    }
}
void IO_Board_Sim::get_compass_calibration_data(math::vec3f &bias) const
{
    bias = m_calibration_config.compass_bias;
}

auto IO_Board_Sim::get_sensor_samples() const -> std::vector<Sensor_Sample> const&
{
    return m_sensor_samples;
}

size_t IO_Board_Sim::get_error_count() const
{
    return m_error_count;
}

auto IO_Board_Sim::process_accelerometer_sensor() -> Channel::Unpack_Result
{
    uint8_t dt = 0;
    math::vec3s16 v;
    auto result = m_channel.unpack_param(dt);
    result = m_channel.unpack_param(v);
    if (result != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive accelerometer data");
        m_error_count++;
        return result;
    }

    auto v2 = (math::vec3f(v) - m_received_config.accelerometer.bias) * m_received_config.accelerometer.scale;

    Accelerometer_Data& data = m_sensors.accelerometer;
    data.value.acceleration = (v2 - m_calibration_config.accelerometer_bias) * m_calibration_config.accelerometer_scale;
    data.value.dt = std::chrono::milliseconds(dt);
    data.sample_idx++;
    return result;
}
auto IO_Board_Sim::process_gyroscope_sensor() -> Channel::Unpack_Result
{
    uint8_t dt = 0;
    math::vec3s16 v;
    auto result = m_channel.unpack_param(dt);
    result = m_channel.unpack_param(v);
    if (result != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive gyroscope data");
        m_error_count++;
        return result;
    }

    auto v2 = (math::vec3f(v) - m_received_config.gyroscope.bias) * m_received_config.gyroscope.scale;

    Gyroscope_Data& data = m_sensors.gyroscope;
    data.value.angular_velocity = v2 - m_calibration_config.gyroscope_bias;
    data.value.dt = std::chrono::milliseconds(dt);
    data.sample_idx++;
    return result;
}
auto IO_Board_Sim::process_compass_sensor() -> Channel::Unpack_Result
{
    math::vec3s16 v;
    auto result = m_channel.unpack_param(v);
    if (result != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive compass data");
        m_error_count++;
        return result;
    }

    auto v2 = (math::vec3f(v) - m_received_config.compass.bias) * m_received_config.compass.scale;

    Compass_Data& data = m_sensors.compass;
    data.value = v2 - m_calibration_config.compass_bias;
    data.sample_idx++;
    return result;
}
auto IO_Board_Sim::process_barometer_sensor() -> Channel::Unpack_Result
{
    float v;
    auto result = m_channel.unpack_param(v);
    if (result != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive barometer data");
        m_error_count++;
        return result;
    }

    auto v2 = (float(v) - m_received_config.barometer.bias) * m_received_config.barometer.scale;

    Barometer_Data& data = m_sensors.barometer;
    data.value = v2;
    data.sample_idx++;
    return result;
}
auto IO_Board_Sim::process_thermometer_sensor() -> Channel::Unpack_Result
{
    int16_t v;
    auto result = m_channel.unpack_param(v);
    if (result != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive thermometer data");
        m_error_count++;
        return result;
    }

    auto v2 = (float(v) - m_received_config.thermometer.bias) * m_received_config.thermometer.scale;

    Thermometer_Data& data = m_sensors.thermometer;
    data.value = v2;
    data.sample_idx++;
    return result;
}
auto IO_Board_Sim::process_sonar_sensor() -> Channel::Unpack_Result
{
    uint16_t v;
    auto result = m_channel.unpack_param(v);
    if (result != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive sonar data");
        m_error_count++;
        return result;
    }

    auto v2 = (float(v) - m_received_config.sonar.bias) * m_received_config.sonar.scale;

    Sonar_Data& data = m_sensors.sonar;
    data.value = v2;
    data.sample_idx++;
    return result;
}
auto IO_Board_Sim::process_voltage_sensor() -> Channel::Unpack_Result
{
    uint16_t v;
    auto result = m_channel.unpack_param(v);
    if (result != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive voltage data");
        m_error_count++;
        return result;
    }

    //auto v2 = (float(v) - m_received_config.voltage.bias) * m_received_config.voltage.scale;

    Voltage_Data& data = m_sensors.voltage;
    //data.value = v2;
    return result;
}
auto IO_Board_Sim::process_current_sensor() -> Channel::Unpack_Result
{
    uint16_t v;
    auto result = m_channel.unpack_param(v);
    if (result != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive current data");
        m_error_count++;
        return result;
    }

    //auto v2 = (float(v) - m_received_config.current.bias) * m_received_config.current.scale;

    Current_Data& data = m_sensors.current;
    //data.value = v2;
    return result;
}

void IO_Board_Sim::process_message_sensor_data()
{
    auto result = m_channel.begin_unpack();
    if (result == Channel::Unpack_Result::OK)
    {
        Sensors sensors;
        result = m_channel.unpack_param(sensors);
        if (result == Channel::Unpack_Result::OK)
        {
            if (sensors.test(Sensor::ACCELEROMETER))
            {
                result = process_accelerometer_sensor();
            }
            if (sensors.test(Sensor::GYROSCOPE))
            {
                result = process_gyroscope_sensor();
            }
            if (sensors.test(Sensor::COMPASS))
            {
                result = process_compass_sensor();
            }
            if (sensors.test(Sensor::BAROMETER))
            {
                result = process_barometer_sensor();
            }
            if (sensors.test(Sensor::THERMOMETER))
            {
                result = process_thermometer_sensor();
            }
            if (sensors.test(Sensor::SONAR))
            {
                result = process_sonar_sensor();
            }
            if (sensors.test(Sensor::VOLTAGE))
            {
                result = process_voltage_sensor();
            }
            if (sensors.test(Sensor::CURRENT))
            {
                result = process_current_sensor();
            }
        }
    }
    m_channel.end_unpack();

    if (result == Channel::Unpack_Result::OK)
    {
        m_sensor_samples.push_back(m_sensors);
    }
    else
    {
        SILK_WARNING("Failed to receive sensor data");
        m_error_count++;
    }
}
void IO_Board_Sim::process_message_gps_data()
{
    GPS v;
    if (m_channel.unpack(v) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive gps data");
        m_error_count++;
    }

    QASSERT(0);
    GPS_Data data;
    data.value = v;
    //m_gps_data.push_back(data);
}

//----------------------------------------------------------------------------------

void IO_Board_Sim::process_state_disconnected()
{

}
void IO_Board_Sim::process_state_handshake()
{
    if (m_has_message)
    {
        if (m_message == Message::CONFIG)
        {
            auto result = m_channel.unpack(m_received_config.version,
                                           m_received_config.has_barometer,
                                           m_received_config.has_sonar,
                                           m_received_config.has_gps,
                                           m_received_config.has_voltage_sensor,
                                           m_received_config.has_current_sensor,
                                           m_received_config.accelerometer,
                                           m_received_config.gyroscope,
                                           m_received_config.compass,
                                           m_received_config.barometer,
                                           m_received_config.sonar,
                                           m_received_config.thermometer);
            if (result == Channel::Unpack_Result::OK)
            {
                SILK_INFO("IO Board version {}:\b \t{} barometer, \t{} gps, \t{} sonar, \t{} voltage sensor, \t{} current sensor",
                           m_received_config.version,
                           m_received_config.has_barometer ? "has a" : "doesn't have",
                           m_received_config.has_gps ? "has a" : "doesn't have",
                           m_received_config.has_sonar ? "has a" : "doesn't have",
                           m_received_config.has_voltage_sensor ? "has a" : "doesn't have",
                           m_received_config.has_current_sensor ? "has a" : "doesn't have");

                set_state(State::RUNNING);
            }
            else
            {
                SILK_WARNING("Failed to communicate with the IO Board");
                m_error_count++;
            }
        }
        else
        {
            SILK_WARNING("Received wrong message: {}", static_cast<int>(m_message));
            m_error_count++;
        }
    }
}
void IO_Board_Sim::process_state_running()
{
    auto now = q::Clock::now();
    if (now - m_resend_timestamp > std::chrono::milliseconds(2))
    {
        m_resend_timestamp = now;

        //m_channel.stream(Message::RATE_TARGETS, m_yaw_rate_target, m_pitch_rate_target, m_roll_rate_target);
        m_channel.begin_stream();
        m_channel.add_to_stream(static_cast<uint8_t>(m_motor_outputs.size()));
        for (auto const& o: m_motor_outputs)
        {
            auto v = static_cast<uint16_t>(math::clamp(o, 0.f, 1.f) * 65535.f);
            m_channel.add_to_stream(v);
        }
        m_channel.end_stream(Message::MOTOR_OUTPUTS);
    }

    send_configs_if_changed();
}

void IO_Board_Sim::send_configs_if_changed()
{
}

void IO_Board_Sim::process()
{
//#ifndef RASPBERRY_PI
    if (!m_socket.is_open())
    {
        disconnect();
    }
//#endif

    if (is_disconnected())
    {
        connect();
        return;
    }

    bool interrupted = false;

    m_sensor_samples.clear();

    auto start = q::Clock::now();
    do
    {
        {
            static q::Clock::time_point ms;
            auto now = q::Clock::now();
            if (now - ms >= std::chrono::seconds(1))
            {
                SILK_INFO("Messages per second: {} pending: {} err: {}", m_message_count, m_channel.get_pending_data_size(), m_channel.get_error_count());
                m_message_count = 0;
                ms = now;
            }
        }

        if (q::Clock::now() - start > std::chrono::milliseconds(5))
        {
            interrupted = true;
            break;
        }

        m_has_message = m_channel.get_next_message(m_message);
        if (m_has_message)
        {
            m_message_count++;
            m_last_message_timestamp = q::Clock::now();

            //SILK_INFO("received message {}", static_cast<int>(m_message));
            switch (m_message)
            {
            case Message::SENSOR_DATA: process_message_sensor_data(); break;
            case Message::GPS_DATA: process_message_gps_data(); break;

//            case Message::CALIBRATION_CONFIG: process_message_calibration_config(); break;
//            case Message::PID_CONFIG: process_message_pid_config(); break;
//            case Message::MOTOR_CONFIG: process_message_motor_config(); break;
            }
        }

        switch (m_state)
        {
        case State::DISCONNECTED: process_state_disconnected(); break;
        case State::HANDSHAKE: process_state_handshake(); break;
        case State::RUNNING: process_state_running(); break;
        default:
            SILK_WARNING("Unknown state: {}", static_cast<int>(m_state));
            m_error_count++;
            break;
        }

    } while (m_has_message);

    if (interrupted)
    {
        //SILK_WARNING("Interrupted while processing messages / {}.", m_sensor_samples.size());
    }

    update_watchdog();
}

void IO_Board_Sim::set_state(State state)
{
    SILK_INFO("Switching from state {} to state {}", static_cast<int>(m_state), static_cast<int>(state));
    m_state = state;
    m_resend_timestamp = q::Clock::time_point();
    m_watchdog.timeout.is_active = false;
    m_channel.cancel_send();
}

void IO_Board_Sim::start_timeout(q::Clock::duration d, State failed_state)
{
    m_watchdog.timeout.is_active = true;
    m_watchdog.timeout.duration = d;
    m_watchdog.timeout.timestamp = q::Clock::now();
    m_watchdog.timeout.failed_state = failed_state;
}

void IO_Board_Sim::update_watchdog()
{
    auto now = q::Clock::now();
    if (m_watchdog.timeout.is_active)
    {
        if (now - m_watchdog.timeout.timestamp > m_watchdog.timeout.duration)
        {
            SILK_WARNING("Timeout!!. Going to {}", static_cast<int>(m_watchdog.timeout.failed_state));
            set_state(m_watchdog.timeout.failed_state);
            m_watchdog.timeout.is_active = false;
            return;
        }
    }

    auto duration = now - m_last_message_timestamp;
    if (m_state > State::HANDSHAKE && duration > std::chrono::milliseconds(1000))
    {
        SILK_WARNING("Failed to receive messages for {}. Going to WAITING_FOR_CONFIG", duration);
        set_state(State::HANDSHAKE);
        return;
    }
}

#endif

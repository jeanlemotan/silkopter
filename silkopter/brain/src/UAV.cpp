#include "BrainStdAfx.h"
#include "UAV.h"
#include "physics/constants.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

using namespace silk;

UAV::UAV(IO_Board &io_board)
    : m_io_board(io_board)
{
    m_last_timestamp = q::Clock::now();

    load_settings();
    save_settings();
}

auto UAV::load_settings() -> bool
{
    TIMED_FUNCTION();

    ////////////////////////////////////////////////////////////////////////
    //defaults

    //configure a quad
    m_motor_mixer.remove_all_motors();
    m_motor_mixer.add_motor({ math::vec2f(0, 1), false });
    m_motor_mixer.add_motor({ math::vec2f(-1, 0), true });
    m_motor_mixer.add_motor({ math::vec2f(0, -1), false });
    m_motor_mixer.add_motor({ math::vec2f(1, 0), true });

    m_pids.yaw_rate.set_params(Yaw_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));
    m_pids.pitch_rate.set_params(Pitch_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));
    m_pids.roll_rate.set_params(Roll_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));

    ////////////////////////////////////////////////////////////////////////


    q::data::File_Source fs(q::Path("uav.cfg"));
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

    auto finder = jsonutil::Member_Finder(m_settings.document, "motor_config");
    if (finder.is_valid())
    {
        auto& json = finder.get_value();
        {
            finder = jsonutil::Member_Finder(json, "motors");
            if (finder.is_type(rapidjson::kArrayType))
            {
                SILK_WARNING("Cannot read motors");
            }
            else
            {
                auto motor_count = math::min<uint8_t>(MAX_MOTOR_COUNT, finder.get_value().Size());
                if (motor_count < 4 || (motor_count & 1) == 1)
                {
                    SILK_WARNING("To few motors / odd number of morors specified: {}", motor_count);
                }
                else
                {
                    std::vector<Motor_Mixer::Motor_Info> motors;
                    for (uint8_t i = 0; i < motor_count; i++)
                    {
                        auto& mj = finder.get_value()[i];
                        auto position = jsonutil::get_vec2_value<float>(mj, "position");
                        auto clockwise = jsonutil::get_bool_value(mj, "clockwise");
                        if (!position || !clockwise)
                        {
                            SILK_WARNING("Cannot read motor {}", i);
                            motors.clear();
                            break;
                        }
                        motors.emplace_back(*position, *clockwise);
                    }

                    if (!motors.empty())
                    {
                        m_motor_mixer.remove_all_motors();
                        for (auto const& m: motors)
                        {
                            m_motor_mixer.add_motor(m);
                        }
                    }
                }
            }
        }
    }

    finder = jsonutil::Member_Finder(m_settings.document, "pid_config");
    if (finder.is_valid())
    {
        auto& json = finder.get_value();
        {
            auto params = jsonutil::get_pid_params<Yaw_Rate_PID::Params>(json, "yaw_rate");
            if (!params)
            {
                SILK_WARNING("Cannot read yaw_rate");
            }
            else
            {
                m_pids.yaw_rate.set_params(*params);
            }
        }
        {
            auto params = jsonutil::get_pid_params<Pitch_Rate_PID::Params>(json, "pitch_rate");
            if (!params)
            {
                SILK_WARNING("Cannot read pitch_rate");
            }
            else
            {
                m_pids.pitch_rate.set_params(*params);
            }
        }
        {
            auto params = jsonutil::get_pid_params<Roll_Rate_PID::Params>(json, "roll_rate");
            if (!params)
            {
                SILK_WARNING("Cannot read roll_rate");
            }
            else
            {
                m_pids.roll_rate.set_params(*params);
            }
        }
        {
            auto params = jsonutil::get_pid_params<Altitude_Rate_PID::Params>(json, "altitude_rate");
            if (!params)
            {
                SILK_WARNING("Cannot read altitude_rate");
            }
            else
            {
                m_pids.altitude_rate.set_params(*params);
            }
        }
    }

    return true;
}
void UAV::save_settings()
{
    TIMED_FUNCTION();

    if (!m_settings.document.IsObject())
    {
        m_settings.document.SetObject();
    }

    auto& allocator = m_settings.document.GetAllocator();

    auto& motor_config = jsonutil::get_or_add_member(m_settings.document, "motor_config", rapidjson::kObjectType, allocator);
    auto& pid_config = jsonutil::get_or_add_member(m_settings.document, "pid_config", rapidjson::kObjectType, allocator);

    {
        auto& json = motor_config;
        json.SetObject();
        {
            auto& vj = jsonutil::get_or_add_member(json, "motors", rapidjson::kArrayType, allocator);
            vj.Clear();
            for (uint8_t i = 0; i < m_motor_mixer.get_motor_count(); i++)
            {
                rapidjson::Value v;
                v.SetObject();
                jsonutil::set_vec2_value(v, "position", m_motor_mixer.get_motor_info(i).position, allocator);
                v.AddMember("clockwise", m_motor_mixer.get_motor_info(i).clockwise, allocator);
                vj.PushBack(v, allocator);
            }
        }
    }
    {
        auto& json = pid_config;
        json.SetObject();
        jsonutil::set_pid_params<Yaw_Rate_PID::Params>(json, "yaw_rate", m_pids.yaw_rate.get_params(), allocator);
        jsonutil::set_pid_params<Pitch_Rate_PID::Params>(json, "pitch_rate", m_pids.pitch_rate.get_params(), allocator);
        jsonutil::set_pid_params<Roll_Rate_PID::Params>(json, "roll_rate", m_pids.roll_rate.get_params(), allocator);
        jsonutil::set_pid_params<Altitude_Rate_PID::Params>(json, "altitude_rate", m_pids.altitude_rate.get_params(), allocator);
    }


    typedef rapidjson::UTF8<> JSON_Charset;
    typedef rapidjson::GenericStringBuffer<JSON_Charset> JSON_Buffer;
    typedef rapidjson::PrettyWriter<JSON_Buffer> JSON_Writer;

    JSON_Buffer buffer;
    JSON_Writer writer(buffer);
    m_settings.document.Accept(writer);

    q::data::File_Sink fs(q::Path("uav.cfg"));
    if (!fs.is_open())
    {
        SILK_WARNING("Cannot open file to save settings");
        return;
    }

    fs.write(reinterpret_cast<uint8_t const*>(buffer.GetString()), buffer.GetSize());
}

void UAV::set_throttle_mode(uav_input::Throttle_Mode mode)
{
    m_input.throttle_mode = mode;
}
void UAV::set_pitch_roll_mode(uav_input::Pitch_Roll_Mode mode)
{
    m_input.pitch_roll_mode = mode;
}
void UAV::set_yaw_mode(uav_input::Yaw_Mode mode)
{
    m_input.yaw_mode = mode;
}
void UAV::set_reference_frame(uav_input::Reference_Frame frame)
{
    m_input.reference_frame = frame;
}
void UAV::set_sticks(uav_input::Sticks const& sticks)
{
    m_input.sticks = sticks;
}
void UAV::set_assists(uav_input::Assists assists)
{
    m_input.assists = assists;
}
void UAV::arm()
{
    m_is_armed = true;
    m_motor_mixer.set_output_range(0.05f, 1.f);
}
void UAV::disarm()
{
    m_is_armed = false;
    m_motor_mixer.set_output_range(0, 1);

    std::array<float, MAX_MOTOR_COUNT> throttles = {0};
    m_io_board.set_motor_throttles(throttles.data(), throttles.size());
}

auto UAV::get_ahrs() -> AHRS const&
{
    return m_ahrs;
}
auto UAV::get_motor_mixer() -> Motor_Mixer const&
{
    return m_motor_mixer;
}

auto UAV::get_linear_acceleration() const -> math::vec3f const&
{
    return m_linear_motion.acceleration;
}
auto UAV::get_velocity() const -> math::vec3f const&
{
    return m_linear_motion.velocity;
}
auto UAV::get_position() const -> math::vec3f const&
{
    return m_linear_motion.position;
}


void UAV::process()
{
    auto now = q::Clock::now();
    auto dt = now - m_last_timestamp;
    m_last_timestamp = now;
    static q::Clock::duration min_dt, max_dt, avg_dt;
    static int xxx = 0;
    if (xxx == 0)
    {
        //SILK_INFO("min {}, max {}, avg {}", min_dt, max_dt, avg_dt);
        min_dt = dt;
        max_dt = dt;
        avg_dt = std::chrono::milliseconds(0);
    }
    min_dt = std::min(min_dt, dt);
    max_dt = std::max(max_dt, dt);
    avg_dt += dt;
    xxx++;
    if (xxx == 1000)
    {
        avg_dt = avg_dt / xxx;
        xxx = 0;
    }

    process_input(dt);
    process_sensor_data(dt);
    process_motors(dt);
}

void UAV::process_input(q::Clock::duration dt)
{
    if (m_input.throttle_mode == uav_input::Throttle_Mode::RATE)
    {
        process_input_throttle_rate(dt);
    }
    else if (m_input.throttle_mode == uav_input::Throttle_Mode::OFFSET)
    {
        process_input_throttle_offset(dt);
    }
    else if (m_input.throttle_mode == uav_input::Throttle_Mode::ASSISTED)
    {
        process_input_throttle_assisted(dt);
    }


    if (m_input.pitch_roll_mode == uav_input::Pitch_Roll_Mode::RATE)
    {
        process_input_pitch_roll_rate(dt);
    }
    else if (m_input.pitch_roll_mode == uav_input::Pitch_Roll_Mode::HORIZONTAL)
    {
        process_input_pitch_roll_horizontal(dt);
    }
    else if (m_input.pitch_roll_mode == uav_input::Pitch_Roll_Mode::ASSISTED)
    {
        process_input_pitch_roll_assisted(dt);
    }

    if (m_input.yaw_mode == uav_input::Yaw_Mode::RATE)
    {
        process_input_yaw_rate(dt);
    }
}

void UAV::process_input_throttle_rate(q::Clock::duration dt)
{
    float dts = q::Seconds(dt).count();

    m_throttle.reference = math::clamp(m_throttle.reference + m_input.sticks.throttle * dts, 0.f, 1.f);
    m_throttle.current = m_throttle.reference;
}
void UAV::process_input_throttle_offset(q::Clock::duration dt)
{
    QUNUSED(dt);
    m_throttle.current = math::clamp(m_throttle.reference + m_input.sticks.throttle, 0.f, 1.f);
}
void UAV::process_input_throttle_assisted(q::Clock::duration dt)
{
    QUNUSED(dt);
    SILK_WARNING("assisted throttle not implemented");
}
void UAV::process_input_pitch_roll_rate(q::Clock::duration dt)
{
    QUNUSED(dt);
    m_pids.pitch_rate.set_target(m_input.sticks.pitch * m_settings.max_pitch_rate);
    m_pids.roll_rate.set_target(m_input.sticks.roll * m_settings.max_roll_rate);
}
void UAV::process_input_pitch_roll_horizontal(q::Clock::duration dt)
{
    QUNUSED(dt);
}
void UAV::process_input_pitch_roll_assisted(q::Clock::duration dt)
{
    QUNUSED(dt);
    SILK_WARNING("assisted pitch/roll not implemented");
}
void UAV::process_input_yaw_rate(q::Clock::duration dt)
{
    QUNUSED(dt);
    m_pids.yaw_rate.set_target(m_input.sticks.yaw * m_settings.max_yaw_rate);
}

void UAV::process_sensor_data(q::Clock::duration dt)
{
    QUNUSED(dt);
//    static int xxx = 100;
//    xxx--;
//    if (xxx > 0)
//    {
//        return;
//    }

    static q::Clock::time_point xxx = q::Clock::now();
    auto now = q::Clock::now();
    if (now - xxx > std::chrono::seconds(10))
    {
        xxx = now;
        m_linear_motion.acceleration.set(0, 0, 0);
        m_linear_motion.velocity.set(0, 0, 0);
        m_linear_motion.position.set(0, 0, 0);
    }

//    auto start = q::Clock::now();
    auto const& samples = m_io_board.get_sensor_samples();
    for (auto const& sample: samples)
    {
        m_ahrs.process(sample);

        if (sample.gyroscope.sample_idx > m_last_gyroscope_sample_idx)
        {
            m_last_gyroscope_sample_idx = sample.gyroscope.sample_idx;

            auto sample_dt = sample.gyroscope.value.dt;
            auto const& input = sample.gyroscope.value.angular_velocity;

            m_pids.pitch_rate.set_input(input.x);
            m_pids.pitch_rate.process(sample_dt);

            m_pids.roll_rate.set_input(input.y);
            m_pids.roll_rate.process(sample_dt);

            m_pids.yaw_rate.set_input(input.z);
            m_pids.yaw_rate.process(sample_dt);
        }

        if (sample.accelerometer.sample_idx > m_last_accelerometer_sample_idx)
        {
            m_last_accelerometer_sample_idx = sample.accelerometer.sample_idx;

            float sample_dts = q::Seconds(sample.accelerometer.value.dt).count();

            //auto old_acceleration = m_linear_motion.acceleration;
            math::vec3f acceleration = sample.accelerometer.value.acceleration;
            //math::vec3f gravity = math::transform(m_ahrs.get_world_to_local_mat(), math::vec3f(0, 0, 1)) * physics::constants::g;
            auto new_acceleration = math::transform(m_ahrs.get_local_to_world_mat(), acceleration) - math::vec3f(0, 0, physics::constants::g);
            //SILK_INFO("acc {}, l {}", new_acceleration, math::length(new_acceleration));

//            m_linear_motion.position += dts * (m_linear_motion.velocity + dts * old_acceleration * 0.5f);
//            m_linear_motion.velocity += dts * (old_acceleration + new_acceleration) * 0.5f;
//            m_linear_motion.acceleration = new_acceleration;

            m_linear_motion.acceleration = new_acceleration;
            m_linear_motion.velocity += m_linear_motion.acceleration * sample_dts;
            m_linear_motion.position += m_linear_motion.velocity * sample_dts;

            //        m_linear_motion.velocity = math::lerp(m_linear_motion.velocity, math::vec3f::zero, dts * 0.5f);

            //SILK_INFO("{}: {} / {} / {}", dts, new_acceleration, m_linear_motion.velocity, m_linear_motion.position);

//            m_linear_motion.position.z = math::lerp<float, math::safe>(
//                        m_linear_motion.position.z,
//                        sample.sonar.value,
//                        dts * 0.9f);

            if (m_linear_motion.position.z < 0)
            {
                m_linear_motion.position.z = 0;
                m_linear_motion.velocity.z = std::max(0.f, m_linear_motion.velocity.z);
            }
        }
    }


//    SILK_INFO("time: {} for {} samples", q::Clock::now() - start, samples.size());


    //m_position = math::lerp(m_position, math::vec3f::zero, 0.01f);
}

void UAV::process_motors(q::Clock::duration dt)
{
    QUNUSED(dt);

    if (m_is_armed)
    {
        m_motor_mixer.set_data(m_input.sticks.throttle,
                               m_pids.yaw_rate.get_output(),
                               m_pids.pitch_rate.get_output(),
                               m_pids.roll_rate.get_output());

        //SILK_INFO("{} / {}, {}", m_angular_velocity, m_pids.pitch_rate.get_output(), m_pids.roll_rate.get_output());

        std::array<float, MAX_MOTOR_COUNT> throttles;
        for (size_t i = 0; i < m_motor_mixer.get_motor_count(); i++)
        {
            throttles[i] = m_motor_mixer.get_motor_output(i);
        }

        SILK_INFO("{.2}", throttles);

        m_io_board.set_motor_throttles(throttles.data(), m_motor_mixer.get_motor_count());
    }
}


auto UAV::get_yaw_rate_pid_params() const -> Yaw_Rate_PID::Params
{
    return m_pids.yaw_rate.get_params();
}
void UAV::set_yaw_rate_pid_params(Yaw_Rate_PID::Params const& params)
{
    m_pids.yaw_rate.set_params(params);
    save_settings();
}
auto UAV::get_pitch_rate_pid_params() const -> Pitch_Rate_PID::Params
{
    return m_pids.pitch_rate.get_params();
}
void UAV::set_pitch_rate_pid_params(Pitch_Rate_PID::Params const& params)
{
    m_pids.pitch_rate.set_params(params);
    save_settings();
}
auto UAV::get_roll_rate_pid_params() const -> Roll_Rate_PID::Params
{
    return m_pids.roll_rate.get_params();
}
void UAV::set_roll_rate_pid_params(Roll_Rate_PID::Params const& params)
{
    m_pids.roll_rate.set_params(params);
    save_settings();
}
auto UAV::get_altitude_rate_pid_params() const -> Altitude_Rate_PID::Params
{
    return m_pids.altitude_rate.get_params();
}
void UAV::set_altitude_rate_pid_params(Altitude_Rate_PID::Params const& params)
{
    m_pids.altitude_rate.set_params(params);
    save_settings();
}
auto UAV::get_yaw_pid_params() const -> Yaw_PID::Params
{
    return m_pids.yaw.get_params();
}
void UAV::set_yaw_pid_params(Yaw_PID::Params const& params)
{
    m_pids.yaw.set_params(params);
    save_settings();
}
auto UAV::get_pitch_pid_params() const -> Pitch_PID::Params
{
    return m_pids.pitch.get_params();
}
void UAV::set_pitch_pid_params(Pitch_PID::Params const& params)
{
    m_pids.pitch.set_params(params);
    save_settings();
}
auto UAV::get_roll_pid_params() const -> Roll_PID::Params
{
    return m_pids.roll.get_params();
}
void UAV::set_roll_pid_params(Roll_PID::Params const& params)
{
    m_pids.roll.set_params(params);
    save_settings();
}
auto UAV::get_altitude_pid_params() const -> Altitude_PID::Params
{
    return m_pids.altitude.get_params();
}
void UAV::set_altitude_pid_params(Altitude_PID::Params const& params)
{
    m_pids.altitude.set_params(params);
    save_settings();
}

auto UAV::get_assist_params() const -> Assist_Params
{
    return m_assist_params;
}
void UAV::set_assist_params(Assist_Params const& params)
{
    m_assist_params = params;
}

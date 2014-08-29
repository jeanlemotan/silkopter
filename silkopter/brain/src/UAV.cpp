#include "BrainStdAfx.h"
#include "UAV.h"
#include "physics/constants.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

using namespace silk;

static uint8_t MAX_MOTOR_COUNT = 12;
UAV::UAV(IO_Board &io_board)
    : m_io_board(io_board)
{
    m_last_timestamp = q::Clock::now();

    if (!load_settings())
    {
        //configure a quad
        m_motor_mixer.remove_all_motors();
        m_motor_mixer.add_motor({ math::vec2f(0, 1), false });
        m_motor_mixer.add_motor({ math::vec2f(-1, 0), true });
        m_motor_mixer.add_motor({ math::vec2f(0, -1), false });
        m_motor_mixer.add_motor({ math::vec2f(1, 0), true });

        m_pids.yaw_rate.set_params(Yaw_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));
        m_pids.pitch_rate.set_params(Pitch_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));
        m_pids.roll_rate.set_params(Roll_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));

        save_settings();
    }
}

auto UAV::load_settings() -> bool
{
    TIMED_FUNCTION();

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
    if (!finder.is_valid())
    {
        SILK_WARNING("Cannot find motor_config");
        return false;
    }
    auto& motor_config = finder.get_value();

    finder = jsonutil::Member_Finder(m_settings.document, "pid_config");
    if (!finder.is_valid())
    {
        SILK_WARNING("Cannot find pid_config");
        return false;
    }
    auto& pid_config = finder.get_value();

    {
        auto& json = motor_config;
        {
            finder = jsonutil::Member_Finder(json, "motors");
            if (!finder.is_type(rapidjson::kArrayType))
            {
                SILK_WARNING("Cannot read motors");
                return false;
            }
            auto motor_count = math::min<uint8_t>(MAX_MOTOR_COUNT, finder.get_value().Size());
            if (motor_count < 4 || (motor_count & 1) == 1)
            {
                SILK_WARNING("To few motors / odd number of morors specified: {}", motor_count);
                return false;
            }
            m_motor_mixer.remove_all_motors();
            for (uint8_t i = 0; i < motor_count; i++)
            {
                auto& mj = finder.get_value()[i];
                auto position = jsonutil::get_vec2_value<float>(mj, "position");
                auto clockwise = jsonutil::get_bool_value(mj, "clockwise");
                if (!position || !clockwise)
                {
                    SILK_WARNING("Cannot read motor {}", i);
                    return false;
                }
                m_motor_mixer.add_motor({*position, *clockwise});
            }
        }
    }

    {
        auto& json = pid_config;
        {
            auto params = jsonutil::get_pid_params<Yaw_Rate_PID::Params>(json, "yaw_rate");
            if (!params)
            {
                SILK_WARNING("Cannot read yaw_rate");
                return false;
            }
            m_pids.yaw_rate.set_params(*params);
        }
        {
            auto params = jsonutil::get_pid_params<Pitch_Rate_PID::Params>(json, "pitch_rate");
            if (!params)
            {
                SILK_WARNING("Cannot read pitch_rate");
                return false;
            }
            m_pids.pitch_rate.set_params(*params);
        }
        {
            auto params = jsonutil::get_pid_params<Roll_Rate_PID::Params>(json, "roll_rate");
            if (!params)
            {
                SILK_WARNING("Cannot read roll_rate");
                return false;
            }
            m_pids.roll_rate.set_params(*params);
        }
        {
            auto params = jsonutil::get_pid_params<Altitude_Rate_PID::Params>(json, "altitude_rate");
            if (!params)
            {
                SILK_WARNING("Cannot read altitude_rate");
                return false;
            }
            m_pids.altitude_rate.set_params(*params);
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
        jsonutil::set_pid_params<Yaw_Rate_PID::Params>(json, "yaw_rate", m_pids.yaw_rate.get_params(), allocator);
        jsonutil::set_pid_params<Pitch_Rate_PID::Params>(json, "pitch_rate", m_pids.pitch_rate.get_params(), allocator);
        jsonutil::set_pid_params<Roll_Rate_PID::Params>(json, "roll_rate", m_pids.roll_rate.get_params(), allocator);
        jsonutil::set_pid_params<Altitude_Rate_PID::Params>(json, "altitude_rate", m_pids.altitude_rate.get_params(), allocator);
    }


    typedef rapidjson::UTF8<> JSONCharset;
    typedef rapidjson::GenericStringBuffer<JSONCharset> JSONBuffer;
    typedef rapidjson::PrettyWriter<JSONBuffer> JSONWriter;

    JSONBuffer buffer;
    JSONWriter writer(buffer);
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
}
void UAV::disarm()
{
    m_is_armed = false;
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

    process_sensor_data();
    process_motors();
}

void UAV::process_sensor_data()
{
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

            process_rate_pids(sample.gyroscope.value.dt, sample.gyroscope.value.angular_velocity);
        }

        if (sample.accelerometer.sample_idx > m_last_accelerometer_sample_idx)
        {
            m_last_accelerometer_sample_idx = sample.accelerometer.sample_idx;

            float dts = q::Seconds(sample.accelerometer.value.dt).count();

            //auto old_acceleration = m_linear_motion.acceleration;
            math::vec3f acceleration = sample.accelerometer.value.acceleration;
            //math::vec3f gravity = math::transform(m_ahrs.get_world_to_local_mat(), math::vec3f(0, 0, 1)) * physics::constants::g;
            auto new_acceleration = math::transform(m_ahrs.get_local_to_world_mat(), acceleration) - math::vec3f(0, 0, physics::constants::g);
            //SILK_INFO("acc {}, l {}", new_acceleration, math::length(new_acceleration));

//            m_linear_motion.position += dts * (m_linear_motion.velocity + dts * old_acceleration * 0.5f);
//            m_linear_motion.velocity += dts * (old_acceleration + new_acceleration) * 0.5f;
//            m_linear_motion.acceleration = new_acceleration;

            m_linear_motion.acceleration = new_acceleration;
            m_linear_motion.velocity += m_linear_motion.acceleration * dts;
            m_linear_motion.position += m_linear_motion.velocity * dts;

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

void UAV::process_motors()
{
    if (m_is_armed)
    {
        m_motor_mixer.set_output_range(0.05f, 1.f);
        m_motor_mixer.set_data(m_input.sticks.throttle,
                               m_pids.yaw_rate.get_output(),
                               m_pids.pitch_rate.get_output(),
                               m_pids.roll_rate.get_output());

        //SILK_INFO("{} / {}, {}", m_angular_velocity, m_pids.pitch_rate.get_output(), m_pids.roll_rate.get_output());

        for (size_t i = 0; i < m_motor_mixer.get_motor_count(); i++)
        {
            m_io_board.set_motor_output(i, m_motor_mixer.get_motor_output(i));
        }
    }
    else
    {
        m_motor_mixer.set_output_range(0.f, 1.f);
        for (size_t i = 0; i < m_motor_mixer.get_motor_count(); i++)
        {
            m_io_board.set_motor_output(i, 0.f);
        }
    }
}

void UAV::process_rate_pids(q::Clock::duration dt, math::vec3f const& angular_velocity)
{
    m_pids.pitch_rate.process(dt, angular_velocity.x, m_input.sticks.pitch*3.1415f);
    m_pids.roll_rate.process(dt, angular_velocity.y, m_input.sticks.roll*3.1415f);
    m_pids.yaw_rate.process(dt, angular_velocity.z, m_input.sticks.yaw*3.1415f);
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
    return Yaw_PID::Params();
}
void UAV::set_yaw_pid_params(Yaw_PID::Params const& params)
{

}
auto UAV::get_pitch_pid_params() const -> Pitch_PID::Params
{
    return Pitch_PID::Params();
}
void UAV::set_pitch_pid_params(Pitch_PID::Params const& params)
{

}
auto UAV::get_roll_pid_params() const -> Roll_PID::Params
{
    return Roll_PID::Params();
}
void UAV::set_roll_pid_params(Roll_PID::Params const& params)
{

}
auto UAV::get_altitude_pid_params() const -> Altitude_PID::Params
{
    return Altitude_PID::Params();
}
void UAV::set_altitude_pid_params(Altitude_PID::Params const& params)
{

}

auto UAV::get_assist_params() const -> Assist_Params
{
    return m_assist_params;
}
void UAV::set_assist_params(Assist_Params const& params)
{
    m_assist_params = params;
}

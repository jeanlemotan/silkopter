#include "BrainStdAfx.h"
#include "AHRS.h"

extern size_t s_test;

using namespace silk;

void AHRS::process(Gyroscope_Sample const& gyroscope,
                   Accelerometer_Sample const& accelerometer,
                   Compass_Sample const& compass)
{
    bool rotation_dirty = false;
    float av_length = 0.f;

    m_angular_velocity.set(0, 0, 0);

    //integrate gyros
    if (gyroscope.sample_idx > m_last_gyroscope_sample_idx)
    {
        m_last_gyroscope_sample_idx = gyroscope.sample_idx;

        auto omega = gyroscope.value;
        auto dts = q::Seconds(gyroscope.dt).count();

        auto theta = omega * dts;
        auto theta_magnitude = math::length(theta);
        if (theta_magnitude > std::numeric_limits<float>::epsilon())
        {
            auto av = theta*0.5f;
            av_length = theta_magnitude;
            auto& a = m_b2e_quat;
            float w = /*(av.w * a.w)*/ - (av.x * a.x) - (av.y * a.y) - (av.z * a.z);
            float x = (av.x * a.w) /*+ (av.w * a.x)*/ + (av.z * a.y) - (av.y * a.z);
            float y = (av.y * a.w) /*+ (av.w * a.y)*/ + (av.x * a.z) - (av.z * a.x);
            float z = (av.z * a.w) /*+ (av.w * a.z)*/ + (av.y * a.x) - (av.x * a.y);

            a.x += x;
            a.y += y;
            a.z += z;
            a.w += w;
            a.normalize<math::safe>();

//            math::quatf dq;
//            dq.set_from_angle_axis(theta_magnitude, theta / theta_magnitude);

//            av_length = theta_magnitude;
//            m_b2e_quat = m_b2e_quat * dq;
//            m_b2e_quat.normalize<math::safe>();

            m_angular_velocity = gyroscope.value;
//            m_euler += gyroscope.value.angular_velocity * dts;
//            static int xxx = 0;
//            xxx++;
//            if (xxx > 100)
//            {
//                xxx = 0;
//                SILK_INFO("{}/{}", math::degrees(m_euler), gyroscope.sample_idx);
//            }
        }

        rotation_dirty = true;
    }

    if (accelerometer.sample_idx > m_last_accelerometer_sample_idx &&
            compass.sample_idx > m_last_compass_sample_idx)
    {
        m_last_compass_sample_idx = compass.sample_idx;
        m_last_accelerometer_sample_idx = accelerometer.sample_idx;

//        SILK_INFO("{} / {}", accelerometer.value.acceleration, compass.value);

        m_earth_frame_up = math::normalized<float, math::safe>(accelerometer.value); //acceleration points opposite of gravity - so up
        m_earth_frame_front = math::normalized<float, math::safe>(compass.value); //this is always good
        m_earth_frame_right = math::normalized<float, math::safe>(math::cross(m_earth_frame_front, m_earth_frame_up));
        m_earth_frame_front = math::cross(m_earth_frame_right, m_earth_frame_up);
        //m_earth_frame_up = math::cross(m_earth_frame_right, m_earth_frame_front);

        math::mat3f mat(m_earth_frame_front, m_earth_frame_right, m_earth_frame_up);
        m_b2e_quat_noisy.set_from_mat3(mat);
        m_b2e_quat_noisy.invert();

        auto now = q::Clock::now();
        auto dt = now - m_last_correction_timestamp;
        m_last_correction_timestamp = now;
        float dtf = q::Seconds(dt).count();

        //cancel drift
        static int xxx = 50;
        if (xxx > 0)
        {
            xxx--;
            m_b2e_quat = m_b2e_quat_noisy;
        }
        else
        {
            if (s_test == 0)
            {
                //take the rate of rotation into account here - the quicker the rotation the bigger the mu
                //like this we compensate for gyro saturation errors
                float mu = dtf * 0.5f + av_length;
                m_b2e_quat = math::nlerp<float, math::safe>(m_b2e_quat, m_b2e_quat_noisy, mu);
                m_b2e_quat = math::normalized<float, math::safe>(m_b2e_quat);
            }
        }

        rotation_dirty = true;
    }

    if (rotation_dirty)
    {
        m_e2b_quat = math::inverse<float, math::safe>(m_b2e_quat);
        m_b2e_quat.get_as_mat3_and_inv<math::safe>(m_b2e_mat, m_e2b_mat);
    }


    //q::quick_logf("front {}, right {}, down {}", get_front_vector(), get_right_vector(), get_up_vector());
}

auto AHRS::get_angular_velocity() const -> math::vec3f const&
{
    return m_angular_velocity;
}
math::quatf const& AHRS::get_b2e_quat() const
{
    return m_b2e_quat;
}
math::mat3f const& AHRS::get_b2e_mat() const
{
    return m_b2e_mat;
}
math::quatf const& AHRS::get_e2b_quat() const
{
    return m_e2b_quat;
}
math::mat3f const& AHRS::get_e2b_mat() const
{
    return m_e2b_mat;
}
math::vec3f const& AHRS::get_front_vector() const
{
    return m_b2e_mat.get_axis_y();
}
math::vec3f const& AHRS::get_right_vector() const
{
    return m_b2e_mat.get_axis_x();
}
math::vec3f const& AHRS::get_up_vector() const
{
    return m_b2e_mat.get_axis_z();
}

void AHRS::reset()
{
    m_b2e_mat = math::mat3f::identity;
    m_b2e_quat = math::quatf::identity;
    m_e2b_mat = math::mat3f::identity;
    m_e2b_quat = math::quatf::identity;

    m_earth_frame_front = math::vec3f::zero;
    m_earth_frame_right = math::vec3f::zero;
    m_earth_frame_up = math::vec3f::zero;
    m_b2e_quat_noisy = math::quatf::identity;

    m_angular_velocity = math::vec3f::zero;
}

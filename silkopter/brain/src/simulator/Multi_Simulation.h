#pragma once

#include "common/node/ISimulator.h"

namespace silk
{
namespace node
{

inline math::vec3f bt_to_vec3f(btVector3 const& v)
{
    return math::vec3f(v.x(), v.y(), v.z());
}
inline math::quatf bt_to_quatf(btQuaternion const& v)
{
    return math::quatf(v.x(), v.y(), v.z(), v.w());
}
inline btVector3 vec3f_to_bt(math::vec3f const& v)
{
    return btVector3(v.x, v.y, v.z);
}
inline btQuaternion quatf_to_bt(math::quatf const& v)
{
    return btQuaternion(v.x, v.y, v.z, v.w);
}

class Multi_Simulation : q::util::Noncopyable
{
public:

    Multi_Simulation();
    ~Multi_Simulation();

    auto init(uint32_t rate) -> bool;

    auto init_uav(ISimulator::UAV_Config const& config) -> bool;

    void reset();
    void stop_motion();

    void process(q::Clock::duration dt, std::function<void(Multi_Simulation&, q::Clock::duration)> const& callback);

    void set_gravity_enabled(bool yes);
    void set_ground_enabled(bool yes);
    void set_simulation_enabled(bool yes);

    auto get_uav_state() const -> ISimulator::UAV_State const&;

    void set_motor_throttle(size_t motor, float throttle);

private:
    void process_world(q::Clock::duration dt);

    bool m_is_simulation_enabled = true;
    bool m_is_ground_enabled = false;
    bool m_is_gravity_enabled = true;

    uint32_t m_rate = 0;
    q::Clock::duration m_dt;

    struct UAV
    {
        ISimulator::UAV_Config config;
        std::unique_ptr<btCylinderShapeZ> shape;
        std::unique_ptr<btMotionState> motion_state;
        std::unique_ptr<btRigidBody> body;
        ISimulator::UAV_State state;
    } m_uav;

//    math::vec3f m_old_linear_velocity;
//    math::quatf m_old_rotation;

//    silk::Accelerometer_Sample m_accelerometer_sample;
//    q::Clock::time_point m_last_accelerometer_time_point;

//    silk::Gyroscope_Sample m_gyroscope_sample;
//    q::Clock::time_point m_last_gyroscope_time_point;

//    silk::Compass_Sample m_compass_sample;
//    q::Clock::time_point m_last_compass_time_point;

//    silk::Barometer_Sample m_barometer_sample;
//    q::Clock::time_point m_last_barometer_time_point;

//    silk::Thermometer_Sample m_thermometer_sample;
//    q::Clock::time_point m_last_thermometer_time_point;

//    silk::Sonar_Sample m_sonar_sample;
//    q::Clock::time_point m_last_sonar_time_point;

//    silk::GPS_Sample m_gps_sample;
//    q::Clock::time_point m_last_gps_time_point;

	std::unique_ptr<btDefaultCollisionConfiguration> m_collision_configuration;
	std::unique_ptr<btCollisionDispatcher> m_dispatcher;
	std::unique_ptr<btDbvtBroadphase> m_broadphase;
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;

	std::unique_ptr<btCollisionShape> m_ground_shape;
	std::unique_ptr<btRigidBody> m_ground_body;

	std::unique_ptr<btDiscreteDynamicsWorld> m_world;


    q::Clock::time_point m_physics_timestamp;
    q::Clock::duration m_physics_duration{0};
//	q::Clock::duration m_uav_duration;

    void process_uav(q::Clock::duration dt);
    void process_air_drag(q::Clock::duration dt);
};


}
}

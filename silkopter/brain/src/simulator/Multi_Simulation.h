#pragma once

#include "common/node/IMulti_Simulator.h"
#include "common/config/Multi.h"

class btCylinderShapeZ;
class btMotionState;
class btRigidBody;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btDbvtBroadphase;
class btSequentialImpulseConstraintSolver;
class btCollisionShape;
class btRigidBody;
class btDiscreteDynamicsWorld;


namespace silk
{
namespace node
{

class Multi_Simulation : q::util::Noncopyable
{
public:

    Multi_Simulation();
    ~Multi_Simulation();

    auto init(uint32_t rate) -> bool;

    auto init_uav(config::Multi const& config) -> bool;

    void reset();
    void stop_motion();

    void process(q::Clock::duration dt, std::function<void(Multi_Simulation&, q::Clock::duration)> const& callback);

    void set_gravity_enabled(bool yes);
    void set_ground_enabled(bool yes);
    void set_simulation_enabled(bool yes);
    void set_drag_enabled(bool yes);

    auto get_uav_state() const -> IMulti_Simulator::UAV_State const&;

    void set_motor_throttle(size_t motor, float throttle);

private:
    void process_world(q::Clock::duration dt);

    bool m_is_simulation_enabled = true;
    bool m_is_ground_enabled = true;
    bool m_is_gravity_enabled = true;
    bool m_is_drag_enabled = true;

    uint32_t m_rate = 0;
    q::Clock::duration m_dt;

    struct UAV
    {
        config::Multi config;
        std::shared_ptr<btCylinderShapeZ> shape;
        std::shared_ptr<btMotionState> motion_state;
        std::shared_ptr<btRigidBody> body;
        IMulti_Simulator::UAV_State state;
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

    std::shared_ptr<btDefaultCollisionConfiguration> m_collision_configuration;
    std::shared_ptr<btCollisionDispatcher> m_dispatcher;
    std::shared_ptr<btDbvtBroadphase> m_broadphase;
    std::shared_ptr<btSequentialImpulseConstraintSolver> m_solver;

    std::shared_ptr<btCollisionShape> m_ground_shape;
    std::shared_ptr<btRigidBody> m_ground_body;

    std::shared_ptr<btDiscreteDynamicsWorld> m_world;


    q::Clock::time_point m_physics_timestamp;
    q::Clock::duration m_physics_duration{0};
//	q::Clock::duration m_uav_duration;

    void process_uav(q::Clock::duration dt);
    struct
    {
        math::vec3f enu_velocity_sum;
        math::vec3f prev_enu_velocity;
        math::vec3f enu_linear_acceleration_sum;
        math::vec3f angular_velocity_sum;
    } m_accumulated_data;

    void process_uav_sensors(q::Clock::duration dt);
    void process_air_drag(q::Clock::duration dt);
};


}
}

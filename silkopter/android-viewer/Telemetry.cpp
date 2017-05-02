#include "Telemetry.h"

Telemetry::Telemetry(QObject *parent) : QObject(parent)
{

}

void Telemetry::setData(silk::stream::IMultirotor_State::Value const& multirotor_state,
              silk::stream::IMultirotor_Commands::Value const& multirotor_commands)
{
    m_multirotor_state = multirotor_state;
    m_multirotor_commands = multirotor_commands;

    emit telemetryChanged();
}

float Telemetry::batteryChargeUsed() const
{
    return m_multirotor_state.battery_state.charge_used;
}

float Telemetry::batteryAverageVoltage() const
{
    return m_multirotor_state.battery_state.average_voltage;
}

float Telemetry::batteryAverageCurrent() const
{
    return m_multirotor_state.battery_state.average_current;
}

float Telemetry::batteryCapacityLeft() const
{
    return m_multirotor_state.battery_state.capacity_left;
}

Telemetry::Mode Telemetry::uavMode() const
{
    return m_multirotor_state.mode;
}

QVector3D Telemetry::localFrameEuler() const
{
    math::vec3f euler;
    m_multirotor_state.local_frame.get_as_euler_zxy(euler);
    return QVector3D(euler.x, euler.y, euler.z);
}

QQuaternion Telemetry::localFrame() const
{
    math::quatf const& q = m_multirotor_state.local_frame;
    return QQuaternion(q.w, q.x, q.y, q.z);
}

QGeoCoordinate Telemetry::homeLocation() const
{
    if (!m_multirotor_state.home_ecef_position.is_initialized())
    {
        return QGeoCoordinate();
    }
    util::coordinates::LLA lla = util::coordinates::ecef_to_lla(*m_multirotor_state.home_ecef_position);
    return QGeoCoordinate(lla.latitude, lla.longitude, lla.altitude);
}

QGeoCoordinate Telemetry::location() const
{
    util::coordinates::LLA lla = util::coordinates::ecef_to_lla(m_multirotor_state.ecef_position);
    return QGeoCoordinate(lla.latitude, lla.longitude, lla.altitude);
}

QVector3D Telemetry::localVelocity() const
{
    math::quatf enuToLocal = math::inverse(m_multirotor_state.local_frame);
    math::vec3f v = math::rotate(enuToLocal, m_multirotor_state.enu_velocity);
    return QVector3D(v.x, v.y, v.z);
}

QVector3D Telemetry::enuVelocity() const
{
    math::vec3f const& v = m_multirotor_state.enu_velocity;
    return QVector3D(v.x, v.y, v.z);
}

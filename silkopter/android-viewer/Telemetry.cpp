#include "Telemetry.h"

static QGeoCoordinate llaToQGeoCoordinate(util::coordinates::LLA const& lla)
{
    double latitude = math::degrees(lla.latitude);
    if (latitude > 90.0)
    {
        latitude -= 360.0;
    }
    if (latitude < -90.0)
    {
        latitude += 360.0;
    }
    double longitude = math::degrees(lla.longitude);
    if (longitude > 180.0)
    {
        longitude -= 360.0;
    }
    if (longitude < -180.0)
    {
        longitude += 360.0;
    }

    return QGeoCoordinate(latitude, longitude, lla.altitude);
}

static QGeoCoordinate ecefToQGeoCoordinate(util::coordinates::ECEF const& ecef)
{
    util::coordinates::LLA lla = util::coordinates::ecef_to_lla(ecef);
    return llaToQGeoCoordinate(lla);
}

///////////////////////////////////////////////////////////////////////

Telemetry::Telemetry(QObject *parent) : QObject(parent)
{

}

void Telemetry::setData(silk::stream::IMultirotor_State::Value const& multirotor_state,
              silk::stream::IMultirotor_Commands::Value const& multirotor_commands)
{
    m_multirotor_state = multirotor_state;
    m_multirotor_commands = multirotor_commands;

    emit telemetryChanged();

    processPath();
}

void Telemetry::processPath()
{
    if (!m_multirotor_state.home_ecef_position.is_initialized())
    {
        m_lastPathPoint = QGeoCoordinate();
        emit pathCleared();
    }
    else
    {
        if (!m_lastPathPoint.isValid())
        {
            m_lastPathPoint = ecefToQGeoCoordinate(*m_multirotor_state.home_ecef_position);
            emit pathPointAdded(m_lastPathPoint);
        }
        else
        {
            QGeoCoordinate coordinate = ecefToQGeoCoordinate(m_multirotor_state.ecef_position);
            if (!m_lastPathPoint.isValid() || m_lastPathPoint.distanceTo(coordinate) > 5.f)
            {
                m_lastPathPoint = coordinate;
                emit pathPointAdded(coordinate);
            }
        }
    }
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

QGeoCoordinate Telemetry::homePosition() const
{
    if (!m_multirotor_state.home_ecef_position.is_initialized())
    {
        return QGeoCoordinate();
    }
    return ecefToQGeoCoordinate(*m_multirotor_state.home_ecef_position);
}

QGeoCoordinate Telemetry::position() const
{
    return ecefToQGeoCoordinate(m_multirotor_state.ecef_position);
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

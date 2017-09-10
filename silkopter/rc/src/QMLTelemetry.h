#pragma once

#include <QObject>
#include <QGeoCoordinate>
#include <QVector3D>
#include <QQuaternion>

#include "common/stream/IMultirotor_State.h"
#include "common/stream/IMultirotor_Commands.h"


class Telemetry : public QObject
{
    Q_OBJECT
public:
    explicit Telemetry(QObject *parent = 0);

    void setData(silk::stream::IMultirotor_State::Value const& multirotor_state,
                  silk::stream::IMultirotor_Commands::Value const& multirotor_commands);

    Q_PROPERTY(float batteryChargeUsed READ batteryChargeUsed NOTIFY telemetryChanged)
    Q_PROPERTY(float batteryAverageVoltage READ batteryAverageVoltage NOTIFY telemetryChanged)
    Q_PROPERTY(float batteryAverageCurrent READ batteryAverageCurrent NOTIFY telemetryChanged)
    Q_PROPERTY(float batteryCapacityLeft READ batteryCapacityLeft NOTIFY telemetryChanged)

    typedef silk::stream::IMultirotor_State::Mode Mode;
    Q_ENUMS(Mode);

    Q_PROPERTY(Mode uavMode READ uavMode NOTIFY telemetryChanged)

    Q_PROPERTY(QVector3D localFrameEuler READ localFrameEuler NOTIFY telemetryChanged)
    Q_PROPERTY(QQuaternion localFrame READ localFrame NOTIFY telemetryChanged)
    Q_PROPERTY(QGeoCoordinate homePosition READ homePosition NOTIFY telemetryChanged)
    Q_PROPERTY(QGeoCoordinate position READ position NOTIFY telemetryChanged)
    Q_PROPERTY(QVector3D localVelocity READ localVelocity NOTIFY telemetryChanged)
    Q_PROPERTY(QVector3D enuVelocity READ enuVelocity NOTIFY telemetryChanged)

    float batteryChargeUsed() const;
    float batteryAverageVoltage() const;
    float batteryAverageCurrent() const;
    float batteryCapacityLeft() const;
    Mode uavMode() const;
    QVector3D localFrameEuler() const;
    QQuaternion localFrame() const;
    QGeoCoordinate homePosition() const;
    QGeoCoordinate position() const;
    QVector3D localVelocity() const;
    QVector3D enuVelocity() const;

signals:
    void telemetryChanged();
    void pathPointAdded(QGeoCoordinate const& point);
    void pathCleared();

public slots:

private:
    silk::stream::IMultirotor_State::Value m_multirotor_state;
    silk::stream::IMultirotor_Commands::Value m_multirotor_commands;

    void processPath();
    QGeoCoordinate m_lastPathPoint;
};

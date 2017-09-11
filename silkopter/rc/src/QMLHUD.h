#pragma once

#include <QObject>
#include <QGeoCoordinate>
#include <QVector3D>
#include <QQuaternion>
#include <thread>

#include "HAL.h"
#include "common/stream/IMultirotor_State.h"
#include "common/stream/IMultirotor_Commands.h"

class QMLHUD : public QObject
{
    Q_OBJECT
public:
    explicit QMLHUD(QObject* parent = 0);
    void init(silk::HAL& hal);

    void process();

    enum class Mode
    {
        MODE_IDLE = (int)silk::stream::IMultirotor_Commands::Mode::IDLE,
        MODE_TAKE_OFF = (int)silk::stream::IMultirotor_Commands::Mode::TAKE_OFF,
        MODE_FLY = (int)silk::stream::IMultirotor_Commands::Mode::FLY,
        MODE_RETURN_HOME = (int)silk::stream::IMultirotor_Commands::Mode::RETURN_HOME,
        MODE_LAND = (int)silk::stream::IMultirotor_Commands::Mode::LAND
    };
    Q_ENUM(Mode)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)
    Mode mode() const;
    void setMode(Mode mode);
    Q_PROPERTY(bool isModeConfirmed READ isModeConfirmed NOTIFY modeConfirmedChanged)
    bool isModeConfirmed() const;

    enum class VerticalMode
    {
        VERTICAL_MODE_THRUST = (int)silk::stream::IMultirotor_Commands::Vertical_Mode::THRUST,
        VERTICAL_MODE_ALTITUDE = (int)silk::stream::IMultirotor_Commands::Vertical_Mode::ALTITUDE,
    };
    Q_ENUM(VerticalMode)
    Q_PROPERTY(VerticalMode verticalMode READ verticalMode WRITE setVerticalMode NOTIFY verticalModeChanged)
    VerticalMode verticalMode() const;
    void setVerticalMode(VerticalMode mode);
    Q_PROPERTY(bool isVerticalModeConfirmed READ isVerticalModeConfirmed NOTIFY verticalModeConfirmedChanged)
    bool isVerticalModeConfirmed() const;

    enum class HorizontalMode
    {
        HORIZONTAL_MODE_ANGLE_RATE = (int)silk::stream::IMultirotor_Commands::Horizontal_Mode::ANGLE_RATE,
        HORIZONTAL_MODE_ANGLE = (int)silk::stream::IMultirotor_Commands::Horizontal_Mode::ANGLE,
        HORIZONTAL_MODE_POSITION = (int)silk::stream::IMultirotor_Commands::Horizontal_Mode::POSITION,
    };
    Q_ENUM(HorizontalMode)
    Q_PROPERTY(HorizontalMode horizontalMode READ horizontalMode WRITE setHorizontalMode NOTIFY horizontalModeChanged)
    HorizontalMode horizontalMode() const;
    void setHorizontalMode(HorizontalMode mode);
    Q_PROPERTY(bool isHorizontalModeConfirmed READ isHorizontalModeConfirmed NOTIFY horizontalModeConfirmedChanged)
    bool isHorizontalModeConfirmed() const;

    enum class YawMode
    {
        YAW_MODE_ANGLE_RATE = (int)silk::stream::IMultirotor_Commands::Yaw_Mode::ANGLE_RATE,
        YAW_MODE_ANGLE = (int)silk::stream::IMultirotor_Commands::Yaw_Mode::ANGLE
    };
    Q_ENUM(YawMode)
    Q_PROPERTY(YawMode yawMode READ yawMode WRITE setYawMode NOTIFY yawModeChanged)
    YawMode yawMode() const;
    void setYawMode(YawMode mode);
    Q_PROPERTY(bool isYawModeConfirmed READ isYawModeConfirmed NOTIFY yawModeConfirmedChanged)
    bool isYawModeConfirmed() const;

    enum class StreamQuality
    {
        STREAM_QUALITY_LOW = (int)silk::stream::ICamera_Commands::Quality::LOW,
        STREAM_QUALITY_HIGH = (int)silk::stream::ICamera_Commands::Quality::HIGH,
    };
    Q_ENUM(StreamQuality)
    Q_PROPERTY(StreamQuality streamQuality READ streamQuality WRITE setStreamQuality NOTIFY streamQualityChanged)
    StreamQuality streamQuality() const;
    void setStreamQuality(StreamQuality quality);
    Q_PROPERTY(bool isStreamQualityConfirmed READ isStreamQualityConfirmed NOTIFY streamQualityConfirmedChanged)
    bool isStreamQualityConfirmed() const;

    Q_PROPERTY(bool isRecording READ isRecording WRITE setRecording NOTIFY recordingChanged)
    bool isRecording() const;
    void setRecording(bool recording);
    Q_PROPERTY(bool isRecordingConfirmed READ isRecordingConfirmed NOTIFY recordingConfirmedChanged)
    bool isRecordingConfirmed() const;

    Q_PROPERTY(float batteryChargeUsed READ batteryChargeUsed NOTIFY telemetryChanged)
    float batteryChargeUsed() const;
    Q_PROPERTY(float batteryAverageVoltage READ batteryAverageVoltage NOTIFY telemetryChanged)
    float batteryAverageVoltage() const;
    Q_PROPERTY(float batteryAverageCurrent READ batteryAverageCurrent NOTIFY telemetryChanged)
    float batteryAverageCurrent() const;
    Q_PROPERTY(float batteryCapacityLeft READ batteryCapacityLeft NOTIFY telemetryChanged)
    float batteryCapacityLeft() const;

    Q_PROPERTY(int radioTxRSSI READ radioTxRSSI NOTIFY telemetryChanged)
    int radioTxRSSI() const;
    Q_PROPERTY(int radioRxRSSI READ radioRxRSSI NOTIFY telemetryChanged)
    int radioRxRSSI() const;
    Q_PROPERTY(int videoRxRSSI READ videoRxRSSI NOTIFY telemetryChanged)
    int videoRxRSSI() const;

    Q_PROPERTY(QVector3D localFrameEuler READ localFrameEuler NOTIFY telemetryChanged)
    QVector3D localFrameEuler() const;
    Q_PROPERTY(QQuaternion localFrame READ localFrame NOTIFY telemetryChanged)
    QQuaternion localFrame() const;
    Q_PROPERTY(QGeoCoordinate homePosition READ homePosition NOTIFY telemetryChanged)
    QGeoCoordinate homePosition() const;
    Q_PROPERTY(QGeoCoordinate position READ position NOTIFY telemetryChanged)
    QGeoCoordinate position() const;
    Q_PROPERTY(QVector3D localVelocity READ localVelocity NOTIFY telemetryChanged)
    QVector3D localVelocity() const;
    Q_PROPERTY(QVector3D enuVelocity READ enuVelocity NOTIFY telemetryChanged)
    QVector3D enuVelocity() const;

    Q_PROPERTY(float gimbalPitch READ gimbalPitch WRITE setGimbalPitch NOTIFY gimbalPitchChanged)
    float gimbalPitch() const;
    void setGimbalPitch(float pitch);

signals:
    void modeChanged();
    void modeConfirmedChanged();

    void verticalModeChanged();
    void verticalModeConfirmedChanged();

    void horizontalModeChanged();
    void horizontalModeConfirmedChanged();

    void yawModeChanged();
    void yawModeConfirmedChanged();

    void telemetryChanged();
    void gimbalPitchChanged();

    void streamQualityChanged();
    void streamQualityConfirmedChanged();

    void recordingChanged();
    void recordingConfirmedChanged();

    void pathPointAdded(QGeoCoordinate const& point);
    void pathCleared();

public slots:
    void clearPath();

private:
    mutable std::recursive_mutex m_mutex;

    bool m_isInitialized = false;
    silk::HAL* m_hal = nullptr;

    silk::stream::IMultirotor_State::Value m_multirotorState;

    silk::stream::IMultirotor_Commands::Value m_multirotorCommands;
    silk::stream::ICamera_Commands::Value m_cameraCommands;

    void processModeIdle();
    void processModeTakeOff();
    void processModeFly();
    void processModeReturnHome();
    void processModeLand();

    void processPath();
    QGeoCoordinate m_lastPathPoint;
};

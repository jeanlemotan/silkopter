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

    typedef silk::stream::IMultirotor_State::Mode Mode;
    Q_ENUMS(Mode);
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)
    Mode mode() const;
    void setMode(Mode mode);
    Q_PROPERTY(bool isModeConfirmed READ isModeConfirmed NOTIFY modeConfirmedChanged)
    bool isModeConfirmed() const;

    typedef silk::stream::IMultirotor_Commands::Vertical_Mode VerticalMode;
    Q_ENUMS(VerticalMode);
    Q_PROPERTY(VerticalMode verticalMode READ verticalMode WRITE setVerticalMode NOTIFY verticalModeChanged)
    VerticalMode verticalMode() const;
    void setVerticalMode(VerticalMode mode);
    Q_PROPERTY(bool isVerticalModeConfirmed READ isVerticalModeConfirmed NOTIFY verticalModeConfirmedChanged)
    bool isVerticalModeConfirmed() const;

    typedef silk::stream::IMultirotor_Commands::Horizontal_Mode HorizontalMode;
    Q_ENUMS(HorizontalMode);
    Q_PROPERTY(HorizontalMode horizontalMode READ horizontalMode WRITE setHorizontalMode NOTIFY horizontalModeChanged)
    HorizontalMode horizontalMode() const;
    void setHorizontalMode(HorizontalMode mode);
    Q_PROPERTY(bool isHorizontalModeConfirmed READ isHorizontalModeConfirmed NOTIFY horizontalModeConfirmedChanged)
    bool isHorizontalModeConfirmed() const;

    typedef silk::stream::IMultirotor_Commands::Yaw_Mode YawMode;
    Q_ENUMS(YawMode);
    Q_PROPERTY(YawMode yawMode READ yawMode WRITE setYawMode NOTIFY yawModeChanged)
    YawMode yawMode() const;
    void setYawMode(YawMode mode);
    Q_PROPERTY(bool isYawModeConfirmed READ isYawModeConfirmed NOTIFY yawModeConfirmedChanged)
    bool isYawModeConfirmed() const;

    typedef silk::stream::ICamera_Commands::Quality StreamQuality;
    Q_ENUMS(StreamQuality);
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

};

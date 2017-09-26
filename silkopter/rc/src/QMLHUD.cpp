#include "QMLHUD.h"

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

QMLHUD::QMLHUD(QObject *parent)
    : QObject(parent)
{
}

void QMLHUD::init(silk::HAL& hal)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    m_hal = &hal;

    m_multirotorState = m_hal->get_comms().get_multirotor_state();

    setTargetMode((Mode)m_multirotorState.mode);

    //start up in a safe state if flying
    if (m_multirotorState.mode == silk::stream::IMultirotor_Commands::Mode::FLY)
    {
        setTargetVerticalMode(VerticalMode::VERTICAL_MODE_ALTITUDE);
        setTargetHorizontalMode(HorizontalMode::HORIZONTAL_MODE_POSITION);
        setTargetYawMode(YawMode::YAW_MODE_ANGLE);
    }

    m_isInitialized = true;
}


QMLHUD::Mode QMLHUD::currentMode() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return (Mode)m_multirotorState.mode;
}

QMLHUD::Mode QMLHUD::targetMode() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return (Mode)m_multirotorCommands.mode;
}

void QMLHUD::setTargetMode(Mode newMode)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    Mode oldMode = (Mode)m_multirotorCommands.mode;
    if (oldMode == newMode && m_isInitialized)
    {
        return;
    }

    QLOGI("Mode: {}", newMode);

    //m_last_mode_change_tp = Clock::now();

    if (newMode == Mode::MODE_IDLE)
    {
        //input.get_haptic().vibrate(k_alert_haptic);
        //m_idle_mode_data.is_pressed = false;
        setTargetVerticalMode(VerticalMode::VERTICAL_MODE_THRUST);
        setTargetHorizontalMode(HorizontalMode::HORIZONTAL_MODE_ANGLE);
    }

    if (oldMode == Mode::MODE_IDLE)
    {
        //input.get_haptic().vibrate(k_alert_haptic);
    }
    else if (oldMode == Mode::MODE_RETURN_HOME)
    {
        //when leaving RTH, these are the best modes
        setTargetVerticalMode(VerticalMode::VERTICAL_MODE_ALTITUDE);
        setTargetHorizontalMode(HorizontalMode::HORIZONTAL_MODE_POSITION);
        //no need to change the yaw as it's user controllable in RTH
    }

    bool oldConfirmed = isTargetModeConfirmed();

    m_multirotorCommands.mode = (silk::stream::IMultirotor_Commands::Mode)newMode;
    emit targetModeChanged();

    if (isTargetModeConfirmed() != oldConfirmed)
    {
        emit targetModeConfirmedChanged();
    }
}

bool QMLHUD::isTargetModeConfirmed() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_multirotorCommands.mode == m_multirotorState.mode;
}

QMLHUD::VerticalMode QMLHUD::currentVerticalMode() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return (VerticalMode)m_multirotorState.vertical_mode;
}
QMLHUD::VerticalMode QMLHUD::targetVerticalMode() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return (VerticalMode)m_multirotorCommands.vertical_mode;
}
void QMLHUD::setTargetVerticalMode(VerticalMode newMode)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    if ((VerticalMode)m_multirotorCommands.vertical_mode == newMode && m_isInitialized)
    {
        return;
    }

    QLOGI("Vertical Mode: {}", newMode);

    //m_last_vertical_mode_change_tp = Clock::now();
    //input.get_haptic().vibrate(k_mode_change_haptic);

    if (newMode == VerticalMode::VERTICAL_MODE_ALTITUDE)
    {
        //input.get_sticks().set_throttle_deadband_position(ISticks::Deadband_Position::MIDDLE);
        m_multirotorCommands.sticks.throttle = 0.5f;
    }
    else
    {
        //input.get_sticks().set_throttle_deadband_position(ISticks::Deadband_Position::LOW);
        m_multirotorCommands.sticks.throttle = m_multirotorState.throttle;
    }
    //input.get_stick_actuators().set_target_throttle(m_multirotor_commands.sticks.throttle);

    bool oldConfirmed = isTargetVerticalModeConfirmed();

    m_multirotorCommands.vertical_mode = (silk::stream::IMultirotor_Commands::Vertical_Mode)newMode;
    emit targetVerticalModeChanged();

    if (isTargetVerticalModeConfirmed() != oldConfirmed)
    {
        emit targetVerticalModeConfirmedChanged();
    }
}
bool QMLHUD::isTargetVerticalModeConfirmed() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_multirotorState.vertical_mode == m_multirotorCommands.vertical_mode;
}

QMLHUD::HorizontalMode QMLHUD::currentHorizontalMode() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return (HorizontalMode)m_multirotorState.horizontal_mode;
}
QMLHUD::HorizontalMode QMLHUD::targetHorizontalMode() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return (HorizontalMode)m_multirotorCommands.horizontal_mode;
}
void QMLHUD::setTargetHorizontalMode(HorizontalMode newMode)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    if ((HorizontalMode)m_multirotorCommands.horizontal_mode == newMode && m_isInitialized)
    {
        return;
    }

    QLOGI("Horizontal Mode: {}", newMode);

    //m_last_horizontal_mode_change_tp = Clock::now();
    //input.get_haptic().vibrate(k_mode_change_haptic);

    bool oldConfirmed = isTargetHorizontalModeConfirmed();

    m_multirotorCommands.horizontal_mode = (silk::stream::IMultirotor_Commands::Horizontal_Mode)newMode;
    emit targetHorizontalModeChanged();

    if (isTargetHorizontalModeConfirmed() != oldConfirmed)
    {
        emit targetHorizontalModeConfirmedChanged();
    }
}
bool QMLHUD::isTargetHorizontalModeConfirmed() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_multirotorState.horizontal_mode == m_multirotorCommands.horizontal_mode;
}

QMLHUD::YawMode QMLHUD::currentYawMode() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return (YawMode)m_multirotorState.yaw_mode;
}
QMLHUD::YawMode QMLHUD::targetYawMode() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return (YawMode)m_multirotorCommands.yaw_mode;
}
void QMLHUD::setTargetYawMode(YawMode newMode)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    if ((YawMode)m_multirotorCommands.yaw_mode == newMode && m_isInitialized)
    {
        return;
    }

    QLOGI("Yaw Mode: {}", newMode);

    //m_last_yaw_mode_change_tp = Clock::now();
    //input.get_haptic().vibrate(k_mode_change_haptic);
    bool oldConfirmed = isTargetYawModeConfirmed();

    m_multirotorCommands.yaw_mode = (silk::stream::IMultirotor_Commands::Yaw_Mode)newMode;
    emit targetYawModeChanged();

    if (isTargetYawModeConfirmed() != oldConfirmed)
    {
        emit targetYawModeConfirmedChanged();
    }
}
bool QMLHUD::isTargetYawModeConfirmed() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_multirotorState.yaw_mode == m_multirotorCommands.yaw_mode;
}

QMLHUD::StreamQuality QMLHUD::streamQuality() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return (StreamQuality)m_cameraCommands.quality;
}
void QMLHUD::setStreamQuality(StreamQuality quality)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    if ((StreamQuality)m_cameraCommands.quality != quality)
    {
        QLOGI("Stream quality: {}", quality);
        bool oldConfirmed = isStreamQualityConfirmed();

        m_cameraCommands.quality = (silk::stream::ICamera_Commands::Quality)quality;
        emit streamQualityChanged();

        if (isStreamQualityConfirmed() != oldConfirmed)
        {
            emit streamQualityConfirmedChanged();
        }
    }
}
bool QMLHUD::isStreamQualityConfirmed() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return true;
}

bool QMLHUD::isRecording() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_cameraCommands.recording;
}
void QMLHUD::setRecording(bool recording)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    if (m_cameraCommands.recording != recording)
    {
        QLOGI("Recording: {}", recording);
        bool oldConfirmed = isRecordingConfirmed();

        m_cameraCommands.recording = recording;
        emit recordingChanged();

        if (isRecordingConfirmed() != oldConfirmed)
        {
            emit recordingConfirmedChanged();
        }
    }
}
bool QMLHUD::isRecordingConfirmed() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return true;
}

float QMLHUD::batteryChargeUsed() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_multirotorState.battery_state.charge_used;
}
float QMLHUD::batteryAverageVoltage() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_multirotorState.battery_state.average_voltage;
}
float QMLHUD::batteryAverageCurrent() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_multirotorState.battery_state.average_current;
}
float QMLHUD::batteryCapacityLeft() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_multirotorState.battery_state.capacity_left;
}

int QMLHUD::radioTxRSSI() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_hal->get_comms().get_tx_dBm();
}
int QMLHUD::radioRxRSSI() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_hal->get_comms().get_rx_dBm();
}
int QMLHUD::videoRxRSSI() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return 0;//m_hal->get_comms().get_video_streamer().get_input_dBm();
}

QVector3D QMLHUD::localFrameEuler() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    math::vec3f euler;
    m_multirotorState.local_frame.get_as_euler_zxy(euler);
    return QVector3D(euler.x, euler.y, euler.z);
}
QQuaternion QMLHUD::localFrame() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    math::quatf const& q = m_multirotorState.local_frame;
    return QQuaternion(q.w, q.x, q.y, q.z);
}
QGeoCoordinate QMLHUD::homePosition() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    if (!m_multirotorState.home_ecef_position.is_initialized())
    {
        return QGeoCoordinate();
    }
    return ecefToQGeoCoordinate(*m_multirotorState.home_ecef_position);
}
QGeoCoordinate QMLHUD::position() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return ecefToQGeoCoordinate(m_multirotorState.ecef_position);
}
QVector3D QMLHUD::localVelocity() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    math::quatf enuToLocal = math::inverse(m_multirotorState.local_frame);
    math::vec3f v = math::rotate(enuToLocal, m_multirotorState.enu_velocity);
    return QVector3D(v.x, v.y, v.z);
}
QVector3D QMLHUD::enuVelocity() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    math::vec3f const& v = m_multirotorState.enu_velocity;
    return QVector3D(v.x, v.y, v.z);
}
float QMLHUD::gimbalPitch() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    return m_multirotorCommands.gimbal_pitch;
}
void QMLHUD::setGimbalPitch(float pitch)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
    if (m_multirotorCommands.gimbal_pitch != pitch)
    {
        QLOGI("Gimbal Pitch: {}", pitch);

        m_multirotorCommands.gimbal_pitch = pitch;
        emit gimbalPitchChanged();
    }
}

void QMLHUD::process()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    QASSERT(m_isInitialized);
    if (!m_isInitialized)
    {
        return;
    }

    Clock::time_point now = Clock::now();
//    Clock::duration dt = now - m_last_tp;
//    m_last_tp = now;

//    if (input.get_menu_switch().was_released())
//    {
//        //leave the uav in a safe state
//        if (m_multirotor_state.mode == stream::IMultirotor_State::Mode::FLY)
//        {
//            set_vertical_mode(input, Vertical_Mode::ALTITUDE);
//            set_horizontal_mode(input, Horizontal_Mode::POSITION);
//            set_yaw_mode(input, Yaw_Mode::ANGLE);
//        }

//        return false;
//    }

    silk::Comms& comms = m_hal->get_comms();

    m_multirotorState = comms.get_multirotor_state();

    emit telemetryChanged();
    processPath();

//    m_rx_strength = math::lerp<math::safe>(m_rx_strength, dBm_to_strength(comms.get_rx_dBm()), std::chrono::duration<float>(dt).count());
//    m_slow_rx_strength = math::lerp<math::safe>(m_slow_rx_strength, m_rx_strength, std::chrono::duration<float>(dt).count() / 5.f);

//    m_tx_strength = math::lerp<math::safe>(m_tx_strength, dBm_to_strength(comms.get_tx_dBm()), std::chrono::duration<float>(dt).count());
//    m_slow_tx_strength = math::lerp<math::safe>(m_slow_tx_strength, m_tx_strength, std::chrono::duration<float>(dt).count() / 5.f);

    silk::stream::IMultirotor_State::Value const& state = m_multirotorState;
    if (state.mode != m_multirotorCommands.mode)
    {
//        if (now - m_last_mode_change_tp >= std::chrono::seconds(2))
//        {
//            QLOGI("REVERTED to mode {}!!!", state.mode);
//            setMode(state.mode);
//            //input.get_haptic().vibrate(k_alert_haptic);
//            return true;
//        }
    }
    else
    {
        switch ((Mode)m_multirotorCommands.mode)
        {
        case Mode::MODE_IDLE: processModeIdle(); break;
        case Mode::MODE_TAKE_OFF: processModeTakeOff(); break;
        case Mode::MODE_FLY: processModeFly(); break;
        case Mode::MODE_RETURN_HOME: processModeReturnHome(); break;
        case Mode::MODE_LAND: processModeLand(); break;
        }
    }

    //m_multirotorCommands.gimbal_pitch = m_hal.get_gimbal_control().get_pitch();

    comms.send_multirotor_commands_value(m_multirotorCommands);
    comms.send_camera_commands_value(m_cameraCommands);
}

void QMLHUD::processModeIdle()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);
}

void QMLHUD::processModeTakeOff()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

}

void QMLHUD::processModeFly()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    silk::stream::IMultirotor_State::Value const& state = m_multirotorState;
    QASSERT((Mode)state.mode == Mode::MODE_FLY);

//    m_multirotor_commands.sticks.yaw = sticks.get_yaw();
//    m_multirotor_commands.sticks.pitch = sticks.get_pitch();
//    m_multirotor_commands.sticks.roll = sticks.get_roll();

//    //only apply the throttle some time after the mode chang, to let the actuator settle
//    if (Clock::now() - m_last_vertical_mode_change_tp > std::chrono::milliseconds(200))
//    {
//        if (m_multirotor_commands.vertical_mode == VerticalMode::THRUST)
//        {
//            input.get_stick_actuators().set_target_throttle(boost::none);
//        }

//        m_multirotor_commands.sticks.throttle = sticks.get_throttle();
//    }
}

void QMLHUD::processModeReturnHome()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

}

void QMLHUD::processModeLand()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

}

void QMLHUD::processPath()
{
    if (!m_multirotorState.home_ecef_position.is_initialized())
    {
        clearPath();
    }
    else
    {
        if (!m_lastPathPoint.isValid())
        {
            m_lastPathPoint = ecefToQGeoCoordinate(*m_multirotorState.home_ecef_position);
            emit pathPointAdded(m_lastPathPoint);
        }
        else
        {
            QGeoCoordinate coordinate = ecefToQGeoCoordinate(m_multirotorState.ecef_position);
            if (!m_lastPathPoint.isValid() || m_lastPathPoint.distanceTo(coordinate) > 5.f)
            {
                m_lastPathPoint = coordinate;
                emit pathPointAdded(coordinate);
            }
        }
    }
}

void QMLHUD::clearPath()
{
    m_lastPathPoint = QGeoCoordinate();
    emit pathCleared();
}

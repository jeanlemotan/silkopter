#include "stdafx.h"
#include "Setup_Motors_Widget.h"

Setup_Motors_Widget::Setup_Motors_Widget(qinput::Input_Mgr& input, silk::Comms& comms, QWidget* parent)
    : QWidget(parent)
    , m_input_mgr(input)
    , m_comms(comms)
{
    m_ui.setupUi(this);


    auto original_ss = m_ui.test->styleSheet();

    QObject::connect(m_ui.test, &QPushButton::released, [this, &comms]()
    {
        bool armed = m_operation_mode ? ((*m_operation_mode) == silk::comms::Operation_Mode::MOTOR_TEST) : false;
        comms.set_operation_mode(armed ? silk::comms::Operation_Mode::IDLE : silk::comms::Operation_Mode::MOTOR_TEST);
        comms.request_operation_mode();
    });

    comms.operation_mode_received.connect([this, &comms, original_ss](silk::comms::Operation_Mode m)
    {
        m_operation_mode = m;
        m_ui.test->setStyleSheet(m == silk::comms::Operation_Mode::MOTOR_TEST ? "background-color: red" : original_ss);
        m_ui.test->setText(m == silk::comms::Operation_Mode::MOTOR_TEST ? "Stop" : "Test");
    });

    auto* timer = new QTimer(this);
    timer->setSingleShot(false);
    timer->start(30);
    connect(timer, &QTimer::timeout, this, &Setup_Motors_Widget::process);
}

void Setup_Motors_Widget::process()
{
    if (!m_gamepad)
    {
        auto gamepads = m_input_mgr.get_all_gamepads();
        if (!gamepads.empty())
        {
            m_gamepad = gamepads[0];
        }
    }

    std::fill(m_input.throttles.begin(), m_input.throttles.end(), 0.f);

    if (m_gamepad)
    {
        using namespace qinput;
        //sticks
        {
            auto rt = m_gamepad->get_axis_data(Gamepad::Axis::RIGHT_TRIGGER);
            float throttle = rt.value;

            m_input.throttles[0] = m_ui.motor1->isChecked() ? throttle : 0;
            m_input.throttles[1] = m_ui.motor2->isChecked() ? throttle : 0;
            m_input.throttles[2] = m_ui.motor3->isChecked() ? throttle : 0;
            m_input.throttles[3] = m_ui.motor4->isChecked() ? throttle : 0;
            m_input.throttles[4] = m_ui.motor5->isChecked() ? throttle : 0;
            m_input.throttles[5] = m_ui.motor6->isChecked() ? throttle : 0;
            m_input.throttles[6] = m_ui.motor7->isChecked() ? throttle : 0;
            m_input.throttles[7] = m_ui.motor8->isChecked() ? throttle : 0;

            m_ui.throttle->setValue(throttle * 100);
        }
    }

    m_comms.send_motor_test_input(m_input);
}



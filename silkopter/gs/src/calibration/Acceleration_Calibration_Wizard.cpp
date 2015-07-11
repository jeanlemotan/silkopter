#include "calibration/Acceleration_Calibration_Wizard.h"

#include "ui_Acceleration_Calibration_Wizard_Intro.h"

Acceleration_Calibration_Wizard::Acceleration_Calibration_Wizard(silk::HAL& hal, silk::Comms& comms, silk::node::stream::Acceleration_ptr stream, QWidget* parent)
    : QDialog(parent)
    , m_stream(stream)
    , m_hal(hal)
    , m_comms(comms)
{
    m_stream = stream;
    m_node = stream->node.lock();
    QASSERT(m_stream);
    QASSERT(m_node);

    m_hal.set_stream_telemetry_active(stream->name, true);
    //stream->

    m_step = Step::INTRO;

    m_content = new QWidget(this);
    layout()->addWidget(m_content);

    Ui::Acceleration_Calibration_Wizard_Intro ui;
    ui.setupUi(m_content);
    ui.description->setText(q::util::format2<std::string>(
                                R"(This wizard will calibrate the acceleration stream of node {}."
                                "It will ask you to hold the UAV still in various positions and will take about 1 minute."
                                ""
                                "Ready?)", m_node->name).c_str());

    QObject::connect(ui.buttonBox, &QDialogButtonBox::accepted, [this]() { advance(); });
    QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, [this]() { cancel(); });
}

void Acceleration_Calibration_Wizard::advance()
{

}

void Acceleration_Calibration_Wizard::cancel()
{

}

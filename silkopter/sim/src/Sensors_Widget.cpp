#include "stdafx.h"
#include "Sensors_Widget.h"

Sensors_Widget::Sensors_Widget(QWidget* parent)
    : QWidget(parent)
    , m_config(&m_dummy_config)
{
    m_ui.setupUi(this);

    notify_config_changed();

    //accelerometer
    QObject::connect(m_ui.accelerometer_sample_rate, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     [this](int v) { m_config->accelerometer.sample_period = std::chrono::microseconds(1000000 / v); });
    QObject::connect(m_ui.accelerometer_range, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->accelerometer.range = static_cast<float>(v); });
    QObject::connect(m_ui.accelerometer_noise, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->accelerometer.noise = static_cast<float>(v); });

    QObject::connect(m_ui.accelerometer_bias_x, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->accelerometer.bias.x = static_cast<float>(v); });
    QObject::connect(m_ui.accelerometer_bias_y, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->accelerometer.bias.y = static_cast<float>(v); });
    QObject::connect(m_ui.accelerometer_bias_z, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->accelerometer.bias.z = static_cast<float>(v); });
    QObject::connect(m_ui.accelerometer_scale_x, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->accelerometer.scale.x = static_cast<float>(v); });
    QObject::connect(m_ui.accelerometer_scale_y, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->accelerometer.scale.y = static_cast<float>(v); });
    QObject::connect(m_ui.accelerometer_scale_z, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->accelerometer.scale.z = static_cast<float>(v); });


    //gyroscope
    QObject::connect(m_ui.gyroscope_sample_rate, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     [this](int v) { m_config->gyroscope.sample_period = std::chrono::microseconds(1000000 / v); });
    QObject::connect(m_ui.gyroscope_range, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->gyroscope.range_degrees = static_cast<float>(v); });
    QObject::connect(m_ui.gyroscope_noise, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->gyroscope.noise_degrees = static_cast<float>(v); });
    QObject::connect(m_ui.gyroscope_bias_x, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->gyroscope.bias_degrees.x = static_cast<float>(v); });
    QObject::connect(m_ui.gyroscope_bias_y, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->gyroscope.bias_degrees.y = static_cast<float>(v); });
    QObject::connect(m_ui.gyroscope_bias_z, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->gyroscope.bias_degrees.z = static_cast<float>(v); });

    //compass
    QObject::connect(m_ui.compass_sample_rate, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     [this](int v) { m_config->compass.sample_period = std::chrono::microseconds(1000000 / v); });
    QObject::connect(m_ui.compass_noise, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->compass.noise = static_cast<float>(v); });
    QObject::connect(m_ui.compass_bias_x, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->compass.bias.x = static_cast<float>(v); });
    QObject::connect(m_ui.compass_bias_y, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->compass.bias.y = static_cast<float>(v); });
    QObject::connect(m_ui.compass_bias_z, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->compass.bias.z = static_cast<float>(v); });

    //barometer
    QObject::connect(m_ui.barometer_sample_rate, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     [this](int v) { m_config->barometer.sample_period = std::chrono::microseconds(1000000 / v); });
    QObject::connect(m_ui.barometer_noise, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->barometer.noise = static_cast<float>(v); });
    QObject::connect(m_ui.barometer, &QGroupBox::toggled, [this](bool v) { m_config->barometer.is_enabled = v; });

    //thermometer
    QObject::connect(m_ui.thermometer_sample_rate, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     [this](int v) { m_config->thermometer.sample_period = std::chrono::microseconds(1000000 / v); });
    QObject::connect(m_ui.thermometer_noise, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->thermometer.noise = static_cast<float>(v); });

    //sonar
    QObject::connect(m_ui.sonar_sample_rate, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     [this](int v) { m_config->sonar.sample_period = std::chrono::microseconds(1000000 / v); });
    QObject::connect(m_ui.sonar_range, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->sonar.range = static_cast<float>(v); });
    QObject::connect(m_ui.sonar_noise, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [this](double v) { m_config->sonar.noise = static_cast<float>(v); });
    QObject::connect(m_ui.sonar, &QGroupBox::toggled, [this](bool v) { m_config->sonar.is_enabled = v; });
}


void Sensors_Widget::set_config(Config* config)
{
    m_config = config ? config : &m_dummy_config;
}

void Sensors_Widget::notify_config_changed()
{
    //accelerometer
    m_ui.accelerometer_sample_rate->setValue(1.0 / q::Seconds(m_config->accelerometer.sample_period).count());
    m_ui.accelerometer_range->setValue(m_config->accelerometer.range);
    m_ui.accelerometer_noise->setValue(m_config->accelerometer.noise);
    m_ui.accelerometer_bias_x->setValue(m_config->accelerometer.bias.x);
    m_ui.accelerometer_bias_y->setValue(m_config->accelerometer.bias.y);
    m_ui.accelerometer_bias_z->setValue(m_config->accelerometer.bias.z);
    m_ui.accelerometer_scale_x->setValue(m_config->accelerometer.scale.x);
    m_ui.accelerometer_scale_y->setValue(m_config->accelerometer.scale.y);
    m_ui.accelerometer_scale_z->setValue(m_config->accelerometer.scale.z);


    //gyroscope
    m_ui.gyroscope_sample_rate->setValue(1.0 / q::Seconds(m_config->gyroscope.sample_period).count());
    m_ui.gyroscope_range->setValue(m_config->gyroscope.range_degrees);
    m_ui.gyroscope_noise->setValue(m_config->gyroscope.noise_degrees);
    m_ui.gyroscope_bias_x->setValue(m_config->gyroscope.bias_degrees.x);
    m_ui.gyroscope_bias_y->setValue(m_config->gyroscope.bias_degrees.y);
    m_ui.gyroscope_bias_z->setValue(m_config->gyroscope.bias_degrees.z);


    //compass
    m_ui.compass_sample_rate->setValue(1.0 / q::Seconds(m_config->compass.sample_period).count());
    m_ui.compass_noise->setValue(m_config->compass.noise);
    m_ui.compass_bias_x->setValue(m_config->compass.bias.x);
    m_ui.compass_bias_y->setValue(m_config->compass.bias.y);
    m_ui.compass_bias_z->setValue(m_config->compass.bias.z);


    //barometer
    m_ui.barometer_sample_rate->setValue(1.0 / q::Seconds(m_config->barometer.sample_period).count());
    m_ui.barometer_noise->setValue(m_config->barometer.noise);
    m_ui.barometer->setEnabled(m_config->barometer.is_enabled);


    //thermometer
    m_ui.thermometer_sample_rate->setValue(1.0 / q::Seconds(m_config->thermometer.sample_period).count());
    m_ui.thermometer_noise->setValue(m_config->thermometer.noise);


    //sonar
    m_ui.sonar_sample_rate->setValue(1.0 / q::Seconds(m_config->sonar.sample_period).count());
    m_ui.sonar_range->setValue(m_config->sonar.range);
    m_ui.sonar_noise->setValue(m_config->sonar.noise);
    m_ui.sonar->setEnabled(m_config->sonar.is_enabled);
}


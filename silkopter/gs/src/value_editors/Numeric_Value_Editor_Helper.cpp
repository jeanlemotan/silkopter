#include "value_editors/details/Numeric_Value_Editor_Helper.h"

#include <QDoubleSpinBox>
#include <QSlider>
#include <QHBoxLayout>

Numeric_Value_Editor_Helper::Numeric_Value_Editor_Helper(double value, double min_value, double max_value, double step, size_t decimals)
{
	m_editor = new QWidget();
	QHBoxLayout* layout = new QHBoxLayout(m_editor);
	layout->setMargin(0);
	layout->setSpacing(0);

	m_spinbox = new QDoubleSpinBox(m_editor);
    m_spinbox->setMinimum(min_value);
    m_spinbox->setMaximum(max_value);
	m_spinbox->setDecimals(static_cast<int>(decimals));
	m_spinbox->setSingleStep(step);
	m_spinbox->setValue(value);
	layout->addWidget(m_spinbox);

	QObject::connect(m_spinbox, &QDoubleSpinBox::editingFinished, [this]()
	{
        sig_value_changed.execute(m_spinbox->value());
	});

    if (max_value < std::numeric_limits<float>::max() && min_value > std::numeric_limits<float>::lowest())
	{
		m_slider = new QSlider(Qt::Orientation::Horizontal, m_editor);

		m_slider->setMinimum(0);
		m_slider->setMaximum(1000000);
		layout->addWidget(m_slider);
		QObject::connect(m_slider, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), [=](int)
		{
			//we divide by 10 (arbitrary number) since we only care about proportions, otherwise with max and min values we could get overflow
            double max = max_value / 10.0;
            double min = min_value / 10.0;
			//the distance between max and min (range of values)
			double range = max - min;
			//the proportion of the set value for the slider (inside the slider range)
			double prop = static_cast<double>(m_slider->value()) / static_cast<double>(m_slider->maximum());
			//we compute the actual distance within the range with the slider proportion
			double dist = prop * range;
			//we compute the value computing the offset from the min
			double val = min + dist;
			//we multiply by 10 to bring the actual value
            set_value(val * 10.0);

            sig_value_changed.execute(m_spinbox->value());
		});
	}
}

QWidget* Numeric_Value_Editor_Helper::get_widget()
{
	return m_editor;
}
void Numeric_Value_Editor_Helper::set_value(double value)
{
	m_spinbox->setValue(value);

	if (m_slider)
	{
		//actual value to slider proportion (inverse procedure)
		double max = m_spinbox->maximum() / 10.0;
		double min = m_spinbox->minimum() / 10.0;
		double range = max - min;
		double dist = (value / 10.0) - min;
		double prop = dist / range;
		m_slider->blockSignals(true);
		m_slider->setValue(prop * static_cast<double>(m_slider->maximum()));
		m_slider->blockSignals(false);
	}
}
double Numeric_Value_Editor_Helper::get_value() const
{
	return m_spinbox->value();
}

void Numeric_Value_Editor_Helper::set_read_only(bool read_only)
{
    m_spinbox->setDisabled(read_only);
	if (m_slider)
	{
        m_slider->setDisabled(read_only);
	}
}

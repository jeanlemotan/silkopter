#include "value_editors/Int_Value_Editor.h"

#include <QSpinBox>
#include <QSlider>
#include <QHBoxLayout>


Int_Value_Editor::Int_Value_Editor(const Qualified_Value<ts::IInt_Value>& qualified_value)
    : m_qualified_value(qualified_value)
{
    std::shared_ptr<const ts::IInt_Type> type = m_qualified_value.get_const_value()->get_specialized_type();

	m_editor = new QWidget();
	QHBoxLayout* layout = new QHBoxLayout(m_editor);
	layout->setMargin(0);
	layout->setSpacing(0);

	m_spinbox = new QSpinBox(m_editor);
    m_spinbox->setMinimum(type->get_min_value());
    m_spinbox->setMaximum(std::min<int64_t>(type->get_max_value(), std::numeric_limits<int>::max()));
    m_spinbox->setSingleStep(1);//type->GetStep());
    m_spinbox->setValue(m_qualified_value.get_const_value()->get_value());
    m_spinbox->setAutoFillBackground(true);
    layout->addWidget(m_spinbox);

	m_editingFinishedConnection = QObject::connect(m_spinbox, &QDoubleSpinBox::editingFinished, [this]()
	{
        set_value(m_spinbox->value());
	});

    if (type->get_max_value() < std::numeric_limits<int64_t>::max() &&
        type->get_min_value() > std::numeric_limits<int64_t>::lowest())
	{
		m_slider = new QSlider(Qt::Orientation::Horizontal, m_editor);
        m_slider->setAutoFillBackground(true);

		m_slider->setMinimum(0);
		m_slider->setMaximum(1000000);
		layout->addWidget(m_slider);
		m_sliderConnection = QObject::connect(m_slider, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), [=](int)
		{
			//we divide by 10 (arbitrary number) since we only care about proportions, otherwise with max and min values we could get overflow
            double max = static_cast<double>(std::min<int64_t>(type->get_max_value(), std::numeric_limits<int>::max())) / 10.0;
            double min = static_cast<double>(type->get_min_value()) / 10.0;
			//the distance between max and min (range of values)
			double range = max - min;
			//the proportion of the set value for the slider (inside the slider range)
			double prop = static_cast<double>(m_slider->value()) / static_cast<double>(m_slider->maximum());
			//we compute the actual distance within the range with the slider proportion
			double dist = prop * range;
			//we compute the value computing the offset from the min
			double val = min + dist;
			//we multiply by 10 to bring the actual value
            set_value(static_cast<int64_t>(val * 10.0));
		});
	}

    refresh_read_only_state();
}

Int_Value_Editor::~Int_Value_Editor()
{
	QObject::disconnect(m_editingFinishedConnection);
	QObject::disconnect(m_sliderConnection);
}

QWidget* Int_Value_Editor::get_widget()
{
	return m_editor;
}

Qualified_Value<ts::IValue> Int_Value_Editor::get_qualified_value()
{
    return Qualified_Value<ts::IValue>(m_qualified_value);
}

void Int_Value_Editor::refresh_editor()
{
    std::shared_ptr<const ts::IInt_Value> value = m_qualified_value.get_const_value();
    m_spinbox->setValue(value->get_value());

	if (m_slider)
	{
        std::shared_ptr<const ts::IInt_Type> type = value->get_specialized_type();

		//actual value to slider proportion (inverse procedure)
        double max = static_cast<double>(type->get_max_value()) / 10.0;
        double min = static_cast<double>(type->get_min_value()) / 10.0;
		double range = max - min;
        double dist = (value->get_value() / 10.0) - min;
		double prop = dist / range;
		m_slider->blockSignals(true);
		m_slider->setValue(prop * static_cast<double>(m_slider->maximum()));
		m_slider->blockSignals(false);
	}
}
void Int_Value_Editor::refresh_value()
{
    set_value(m_spinbox->value());
}

void Int_Value_Editor::set_read_only_override(bool read_only)
{
    m_read_only_override = read_only;
    refresh_read_only_state();
}

bool Int_Value_Editor::is_read_only() const
{
    return m_read_only_override || m_qualified_value.is_read_only();
}

void Int_Value_Editor::refresh_read_only_state()
{
    bool isReadOnly = is_read_only();

	m_spinbox->setDisabled(isReadOnly);
	if (m_slider)
	{
		m_slider->setDisabled(isReadOnly);
	}
}

void Int_Value_Editor::set_value(int64_t value)
{
    if (!is_read_only())
	{
        if (std::shared_ptr<ts::IInt_Value> mutable_value = m_qualified_value.get_mutable_value())
		{
            auto result = mutable_value->set_value(value);
            QASSERT(result == ts::success);
        }
	}
}


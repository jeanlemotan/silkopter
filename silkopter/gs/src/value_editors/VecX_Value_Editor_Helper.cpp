#include "value_editors/details/VecX_Value_Editor_Helper.h"

#include <QDoubleSpinBox>
#include <QSlider>
#include <QHBoxLayout>

VecX_Value_Editor_Helper::VecX_Value_Editor_Helper(const std::vector<Component>& components, size_t decimals)
{
	m_editor = new QWidget();
	QHBoxLayout* layout = new QHBoxLayout(m_editor);
	layout->setMargin(0);
	layout->setSpacing(0);

	for (const Component& component: components)
	{
		QDoubleSpinBox* spinbox = new QDoubleSpinBox(m_editor);
		spinbox->setObjectName(component.name.c_str());
		spinbox->setMinimum(component.minValue);
		spinbox->setMaximum(component.maxValue);
		spinbox->setSingleStep(component.step);
		spinbox->setDecimals(static_cast<int>(decimals));
		spinbox->setValue(component.value);
		layout->addWidget(spinbox);

		SpinData spinData;
		spinData.spinbox = spinbox;

		spinData.connection = QObject::connect(spinbox, &QDoubleSpinBox::editingFinished, [this]()
		{
            std::vector<double> values = get_values();
            sig_value_changed(values);
		});

		m_spinboxes.push_back(spinData);
	}
}

VecX_Value_Editor_Helper::~VecX_Value_Editor_Helper()
{
	for (SpinData& spinData: m_spinboxes)
	{
		QObject::disconnect(spinData.connection);
	}
}

QWidget* VecX_Value_Editor_Helper::get_widget()
{
	return m_editor;
}
void VecX_Value_Editor_Helper::set_values(const std::vector<double>& values)
{
	if (m_spinboxes.size() != values.size())
	{
        QASSERT_MSG(false, "Too {} values sent to SetValue", values.size() > m_spinboxes.size() ? "many" : "few");
	}

	for (size_t i = 0; i < m_spinboxes.size(); i++)
	{
		m_spinboxes[i].spinbox->setValue(values[i]);
	}
}
std::vector<double> VecX_Value_Editor_Helper::get_values() const
{
	std::vector<double> values(m_spinboxes.size());

	for (size_t i = 0; i < m_spinboxes.size(); i++)
	{
		values[i] = m_spinboxes[i].spinbox->value();
	}

	return values;
}

void VecX_Value_Editor_Helper::set_read_only(bool read_only)
{
	for (SpinData& spinData: m_spinboxes)
	{
        spinData.spinbox->setDisabled(read_only);
	}
}

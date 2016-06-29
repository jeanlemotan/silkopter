#pragma once

class QDoubleSpinBox;
class QSlider;
class QWidget;

class VecX_Value_Editor_Helper final
{
	struct SignalKey;
public:
	struct Component
	{
        std::string name;
		double value = 0;
		double minValue = std::numeric_limits<double>::lowest();
		double maxValue = std::numeric_limits<double>::max();
		double step = 1.0;
	};

    VecX_Value_Editor_Helper(const std::vector<Component>& components, size_t decimals);
    ~VecX_Value_Editor_Helper();

	QWidget*	get_widget();
    std::vector<double>	get_values() const;
    void 		set_values(const std::vector<double>& values);
	void		set_read_only(bool readOnly);

    boost::signals2::signal<void(const std::vector<double>&)> sig_value_changed;

private:
	QWidget* m_editor = nullptr;
	struct SpinData
	{
		QDoubleSpinBox* spinbox = nullptr;
		QMetaObject::Connection connection;
	};
	std::vector<SpinData> m_spinboxes;
};

#pragma once

class QDoubleSpinBox;
class QSlider;
class QWidget;

class Numeric_Value_Editor_Helper final
{
    struct SignalKey;
public:
    Numeric_Value_Editor_Helper(double value, double min_value, double max_value, double step, size_t decimals);

    QWidget*	get_widget();
    double 	get_value() const;
    void 	set_value(double value);
    void	set_read_only(bool readOnly);

    boost::signals2::signal<void(double)> sig_value_changed;

private:
    QWidget* m_editor = nullptr;
    QDoubleSpinBox* m_spinbox = nullptr;
    QSlider* m_slider = nullptr;
};

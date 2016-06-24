#pragma once

#include "IValue_Editor.h"
#include "Qualified_Value.h"

class QSpinBox;
class QSlider;

class Int_Value_Editor : public IValue_Editor
{
public:
    Int_Value_Editor(const Qualified_Value<ts::IInt_Value>& qualified_value);
    ~Int_Value_Editor();

    QWidget*	get_widget() override;
    Qualified_Value<ts::IValue> get_qualified_value() override;
    void		refresh_editor() override;
    void		refresh_value() override;
    void		set_read_only_override(bool read_only) override;
    bool		is_read_only() const override;

private:
    void		refresh_read_only_state();
    void		set_value(int64_t value);

    Qualified_Value<ts::IInt_Value> m_qualified_value;
	QWidget* m_editor = nullptr;
	QSpinBox* m_spinbox = nullptr;
	QSlider* m_slider = nullptr;
	QMetaObject::Connection m_editingFinishedConnection;
	QMetaObject::Connection m_sliderConnection;

    bool m_read_only_override = false;
};


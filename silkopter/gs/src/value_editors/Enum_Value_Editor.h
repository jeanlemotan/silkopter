#pragma once

#include "IValue_Editor.h"
#include "Qualified_Value.h"


class QComboBox;

class Enum_Value_Editor : public IValue_Editor
{
public:
    Enum_Value_Editor(const Qualified_Value<ts::IEnum_Value>& qualified_value);
	~Enum_Value_Editor();

    QWidget*	get_widget() override;
    Qualified_Value<ts::IValue> get_qualified_value() override;
    void		refresh_editor() override;
    void		refresh_value() override;
    void		set_read_only_override(bool read_only) override;
    bool		is_read_only() const override;

private:
    void		refresh_read_only_state();
    void		set_value(std::shared_ptr<const ts::IEnum_Item> element);

    Qualified_Value<ts::IEnum_Value> m_qualified_value;
	QComboBox* m_editor = nullptr;
    QMetaObject::Connection m_changed_connection;
    bool m_read_only_override = false;
};


#pragma once

#include "IValue_Editor.h"
#include "Qualified_Value.h"

class QLineEdit;

class String_Value_Editor : public IValue_Editor
{
public:
    String_Value_Editor(const Qualified_Value<ts::IString_Value>& qualified_value, std::string const& editor_str, std::string const& suffix_str);
	~String_Value_Editor();

    QWidget*	get_widget() override;
    Qualified_Value<ts::IValue> get_qualified_value() override;
    void		refresh_editor() override;
    void		refresh_value() override;
    void		set_read_only_override(bool read_only) override;
    bool		is_read_only() const override;

private:
    void		refresh_read_only_state();
    void		set_value(std::string const& value);

    Qualified_Value<ts::IString_Value> m_qualified_value;
	QLineEdit* m_editor = nullptr;
	QMetaObject::Connection m_editingFinishedConnection;
    bool m_read_only_override = false;
};


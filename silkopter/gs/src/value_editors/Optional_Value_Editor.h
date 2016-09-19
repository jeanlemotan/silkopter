#pragma once

#include "IValue_Editor.h"
#include "Qualified_Value.h"

class QWidget;
class QCheckBox;

class Value_Editor_Factory;

class Optional_Value_Editor : public IValue_Editor
{
public:
    Optional_Value_Editor(const Qualified_Value<ts::IOptional_Value>& qualified_value, std::string const& editor_str, std::string const& suffix_str, std::shared_ptr<Value_Editor_Factory> valueEditorFactory);
	~Optional_Value_Editor();

    QWidget*	get_widget() override;
    Qualified_Value<ts::IValue> get_qualified_value() override;
    void		refresh_editor() override;
    void		refresh_value() override;
    void		set_read_only_override(bool read_only) override;
    bool		is_read_only() const override;

private:
    void		create_inner_editor();
    void		refresh_read_only_state();
    void		set_value(bool value);

    std::shared_ptr<Value_Editor_Factory> m_value_editor_factory;
    Qualified_Value<ts::IOptional_Value> m_qualified_value;
	QWidget* m_editor = nullptr;
	QCheckBox* m_checkbox = nullptr;
    QMetaObject::Connection m_changed_connection;
    QMetaObject::Connection m_toggled_connection;
    std::shared_ptr<IValue_Editor> m_inner_editor;
    bool m_read_only_override = false;

    std::string m_editor_str;
    std::string m_suffix_str;
};


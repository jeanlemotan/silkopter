#pragma once

#include "IValue_Editor.h"
#include "Qualified_Value.h"

class QWidget;
class QCheckBox;
class QComboBox;

class Value_Editor_Factory;

class Variant_Value_Editor : public IValue_Editor
{
public:
    Variant_Value_Editor(const Qualified_Value<ts::IVariant_Value>& qualified_value, std::string const& suffix_str, std::shared_ptr<Value_Editor_Factory> value_editor_factory);
	~Variant_Value_Editor();

    QWidget*	get_widget() override;
    Qualified_Value<ts::IValue> get_qualified_value() override;
    void		refresh_editor() override;
    void		refresh_value() override;
    void		set_read_only_override(bool read_only) override;
    bool		is_read_only() const override;

private:
    void		create_inner_editor();
    void		refresh_read_only_state();

    std::shared_ptr<Value_Editor_Factory> m_value_editor_factory;
    Qualified_Value<ts::IVariant_Value> m_qualified_value;
	QWidget* m_editor = nullptr;
    QMetaObject::Connection m_qt_connection;
	QComboBox* m_combobox = nullptr;
    size_t m_inner_editor_type_index = 0;
    std::shared_ptr<IValue_Editor> m_inner_editor;
    bool m_read_only_override = false;

    std::string m_suffix_str;
};


#pragma once

#include "IValue_Editor.h"
#include "Qualified_Value.h"

class QWidget;

class Empty_Value_Editor : public IValue_Editor
{
public:
    Empty_Value_Editor(const Qualified_Value<ts::IValue>& qualified_value, std::string const& suffix_str);

    QWidget*	get_widget() override;
    Qualified_Value<ts::IValue> get_qualified_value() override;
    void    refresh_editor() override;
    void    refresh_value() override;
    void    set_read_only_override(bool read_only) override;
    bool    is_read_only() const override;

private:
    void    refresh_read_only_state();

    Qualified_Value<ts::IValue> m_qualified_value;
    bool m_read_only_override = false;
    QWidget* m_editor = nullptr;
};


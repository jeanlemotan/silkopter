#pragma once

#include "IValue_Editor.h"
#include "Qualified_Value.h"

class VecX_Value_Editor_Helper;

class Vec2i_Value_Editor : public IValue_Editor
{
public:
    Vec2i_Value_Editor(const Qualified_Value<ts::IVec2i_Value>& qualified_value, std::string const& editor_str, std::string const& suffix_str);

    QWidget*	get_widget() override;
    Qualified_Value<ts::IValue> get_qualified_value() override;
    void		refresh_editor() override;
    void		refresh_value() override;
    void		set_read_only_override(bool read_only) override;
    bool		is_read_only() const override;

private:
    void		refresh_read_only_state();
    void		set_value(const ts::vec2i& value);

    Qualified_Value<ts::IVec2i_Value> m_qualified_value;
    std::shared_ptr<VecX_Value_Editor_Helper> m_helper;
	q::util::Scoped_Connection m_connection;

    bool m_read_only_override = false;
};


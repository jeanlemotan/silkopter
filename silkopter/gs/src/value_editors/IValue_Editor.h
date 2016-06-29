#pragma once

#include "value_editors/Qualified_Value.h"

class QWidget;

class IValue_Editor
{
public:
    virtual ~IValue_Editor() = default;

    virtual QWidget*	get_widget() = 0;
    virtual Qualified_Value<ts::IValue> get_qualified_value() = 0;
    virtual void        refresh_editor() = 0;
    virtual void	refresh_value() = 0;
    virtual void	set_read_only_override(bool read_only) = 0;
    virtual bool	is_read_only() const = 0;

    boost::signals2::signal<void()>     sig_editing_finished;
};




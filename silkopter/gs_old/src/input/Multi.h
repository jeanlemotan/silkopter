#pragma once

namespace input
{

class Multi : q::util::Noncopyable
{
public:
    virtual ~Multi() {}

    virtual auto get_input() const -> silk::stream::IMulti_Commands::Value const& = 0;
    virtual void process() = 0;

};

}

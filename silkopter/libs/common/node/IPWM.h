#pragma once

namespace silk
{
namespace node
{

class IPWM : q::util::Noncopyable
{
public:
    virtual ~IPWM() {}

    virtual auto get_name() const -> q::String const& = 0;

    struct Config
    {
        float min = 0;
        float max = 1;
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual void set_value(float value) = 0;
};

DECLARE_CLASS_PTR(IPWM);

}
}

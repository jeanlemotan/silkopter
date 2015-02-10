#pragma once

namespace silk
{
namespace actuators
{

class IGimbal : q::util::Noncopyable
{
public:
    virtual ~IGimbal() {}

    virtual auto get_name() const -> q::String const& = 0;

    //----------------------------------------------------------------------
    //mount

    virtual void set_rotation(math::vec3f const& rot) = 0;

    //----------------------------------------------------------------------

    virtual void process() = 0;
};

DECLARE_CLASS_PTR(IGimbal);

}
}

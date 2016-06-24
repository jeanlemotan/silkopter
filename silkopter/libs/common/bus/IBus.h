#pragma once

namespace silk
{
namespace hal
{
struct IBus_Descriptor;
}
}


namespace silk
{
namespace bus
{

class IBus : q::util::Noncopyable
{
    DEFINE_RTTI_BASE_CLASS(IBus);
public:
    virtual ~IBus() = default;

    virtual bool init(hal::IBus_Descriptor const& descriptor) = 0;
    virtual std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const = 0;
};

}
}

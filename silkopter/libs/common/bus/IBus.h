#pragma once

namespace silk
{
struct Bus_Descriptor_Base;
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

    virtual bool init(std::shared_ptr<Bus_Descriptor_Base> descriptor) = 0;
    virtual std::shared_ptr<const Bus_Descriptor_Base> get_descriptor() const = 0;
};

}
}

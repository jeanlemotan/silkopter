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

class IBus
{
public:
    virtual ~IBus() = default;

    virtual ts::Result<void> init(hal::IBus_Descriptor const& descriptor) = 0;
    virtual std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const = 0;
};

}
}

#pragma once

namespace silk
{
namespace bus
{

class IBus : q::util::Noncopyable
{
    DEFINE_RTTI_BASE_CLASS(IBus);
public:
    virtual ~IBus() = default;

    virtual bool init(std::shared_ptr<ts::IValue> descriptor) = 0;
    virtual std::shared_ptr<const ts::IValue> get_descriptor() const = 0;
};

}
}

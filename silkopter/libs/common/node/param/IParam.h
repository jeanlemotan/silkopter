#pragma once

namespace silk
{
namespace node
{
namespace param
{

enum class Type
{
    TOGGLE,
    FLOAT,
};

typedef uint8_t Id;

class IParam : q::util::Noncopyable
{
public:
    virtual auto get_param() const -> uint8_t = 0;
    virtual auto get_type() const -> Type = 0;
};
DECLARE_CLASS_PTR(IParam);

template <Type TYPE_VALUE>
class Param_Base : q::util::Noncopyable
{
public:
    static constexpr Type TYPE = TYPE_VALUE;
    virtual auto get_param() const -> uint8_t { return m_id; }
    virtual auto get_type() const -> Type { return TYPE; }
private:
    static Id get_new_id()
    {
        static Id last_id = 0;
        return ++last_id;
    }

    uint8_t m_id = get_new_id();
};
template<Type T> Type constexpr Param_Base<T>::TYPE;


//A stream sample
template<class T> struct Value
{
    Value() : value() {}
    T value;
    uint32_t sample_idx = 0; //different for each sample. NOTE - it's allowed to wrap so don't compare like this: if (my_sample_idx > sample_idx) !!!!!!!
    q::Clock::time_point tp{q::Clock::duration(0)}; //when was the sample generated.
};

}
}
}


namespace ts
{

template<typename T>
T const* IInitializer::get_specialized_initializer_value() const
{
    return dynamic_cast<T const*>(get_initializer_value());
}

}

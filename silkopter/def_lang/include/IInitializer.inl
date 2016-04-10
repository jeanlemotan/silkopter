
namespace ts
{

template<typename T>
auto IInitializer::get_specialized_initializer_value() const -> T const*
{
    return dynamic_cast<T const*>(get_initializer_value());
}

}


namespace ts
{

template<typename T>
std::shared_ptr<const T> IInitializer::get_specialized_initializer_value() const
{
    return std::dynamic_pointer_cast<const T>(get_initializer_value());
}

}

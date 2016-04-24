namespace ts
{


template<typename T>
std::shared_ptr<const T> IValue::select_specialized(Value_Selector&& selector) const
{
    return std::dynamic_pointer_cast<T>(select(std::move(selector)));
}

template<typename T>
std::shared_ptr<T> IValue::select_specialized(Value_Selector&& selector)
{
    return std::dynamic_pointer_cast<T>(select(std::move(selector)));
}

}

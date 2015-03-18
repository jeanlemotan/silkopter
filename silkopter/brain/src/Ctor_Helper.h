#pragma once

namespace silk
{
namespace detail
{

template<int...> struct index_tuple{};

template<int I, typename IndexTuple, typename... Types>
struct make_indexes_impl;

template<int I, int... Indexes, typename T, typename ... Types>
struct make_indexes_impl<I, index_tuple<Indexes...>, T, Types...>
{
    typedef typename make_indexes_impl<I + 1, index_tuple<Indexes..., I>, Types...>::type type;
};

template<int I, int... Indexes>
struct make_indexes_impl<I, index_tuple<Indexes...> >
{
    typedef index_tuple<Indexes...> type;
};

template<typename ... Types>
struct make_indexes : make_indexes_impl<0, index_tuple<>, Types...>
{};

template<class T, class... Args, int... Indexes >
T* apply_helper(index_tuple< Indexes... >, std::tuple<Args...>&& tup)
{
    return new T( std::forward<Args>( std::get<Indexes>(tup))... );
}

template<class T, class ... Args>
T* apply(const std::tuple<Args...>&  tup)
{
    return apply_helper<T>(typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
}

template<class T, class ... Args>
T* apply(std::tuple<Args...>&&  tup)
{
    return apply_helper<T>(typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
}


template<class Base>
class Ctor_Helper_Base
{
public:
    virtual ~Ctor_Helper_Base() {}
    virtual auto create() -> Base* = 0;
};

template<class Base, class T, typename... Params>
class Ctor_Helper : public Ctor_Helper_Base<Base>
{
public:
    Ctor_Helper(Params&&... params)
        : tuple(params...)
    {
    }
    auto create() -> Base*
    {
        return apply<T>(tuple);
    }
    std::tuple<Params...> tuple;
};


}
}

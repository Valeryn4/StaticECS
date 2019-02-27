#pragma once

#include <tuple>
#include <type_traits>
#include <typeinfo>


/*
    Здесь лежит вспомогательный функционал и много шаблонной магии
*/
namespace StaticECS
{
    template<class F, class...Ts, std::size_t...Is>
    void for_each_in_tuple(std::tuple<Ts...> & tuple, F func, std::index_sequence<Is...>) {
        using expander = int[];
        (void)expander {
            0, ((void)func(std::get<Is>(tuple)), 0)...
        };
    }

    template<class F, class...Ts>
    void for_each_in_tuple(std::tuple<Ts...> & tuple, F func) {
        for_each_in_tuple(tuple, func, std::make_index_sequence<sizeof...(Ts)>());
    }

    ///Спер со стак оверфлоу. 
    template <typename T, typename Tuple>
    struct has_type;

    template <typename T>
    struct has_type<T, std::tuple<>> : std::false_type {};

    template <typename T, typename U, typename... Ts>
    struct has_type<T, std::tuple<U, Ts...>> : has_type<T, std::tuple<Ts...>> {};

    template <typename T, typename... Ts>
    struct has_type<T, std::tuple<T, Ts...>> : std::true_type {};

    template <typename T, typename Tuple>
    using tuple_contains_type = typename has_type<T, Tuple>::type;
}
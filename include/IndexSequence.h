#ifndef INDEX_SEQUENCE_H
#define INDEX_SEQUENCE_H

#include <type_traits>
#include <utility>
#include <iostream>

namespace Sequence{
    template<auto... Vs>
    struct Instance{
        using Type = std::common_type_t<decltype(Vs)...>;

        static void print(){
            (std::cout << ... << Vs);
        }
    };



    template<typename Sequence>
    struct First;
    template<auto V, auto... Vs>
    struct First<Instance<V,Vs...>>{
        static constexpr auto result = V;
    };



    template<typename Sequence>
    struct Last;
    template<auto V, auto... Vs>
    struct Last<Instance<V,Vs...>>{
        static constexpr auto result = sizeof...(Vs) ==  0 ? V : (Vs,...);
    };

    

    template<typename Sequence1, typename Sequence2>
    struct ConcatImpl;
    template<auto... Vs1, auto...Vs2>
    struct ConcatImpl<Instance<Vs1...>, Instance<Vs2...>> {
        using Result = Instance<Vs1..., Vs2...>;
    };
    template<typename Sequence1, typename Sequence2>
    using Concat = typename ConcatImpl<Sequence1, Sequence2>::Result;




    template<typename Type, Type From, Type To, Type... Pack>
    struct CreateImpl{
        using Result = typename CreateImpl<Type, From, To-1, To-1, Pack...>::Result;
    };
    template<typename Type, Type Begin, Type... Pack>
    struct CreateImpl<Type, Begin, Begin, Pack...>{
        using Result = Instance<Pack...>;
    };

    template<auto From, auto To>
    using CreateRange = typename CreateImpl<std::common_type_t<decltype(From), decltype(To)>, From, To>::Result;
    template<auto To>
    using CreateFromZero = typename CreateImpl<std::common_type_t<decltype(0),decltype(To)>, 0, To>::Result;
    
    template<auto End, auto Offset>
    struct CreateOffsetImpl{
        template<auto... Is>
        static constexpr auto offset(Instance<Is...>) -> Instance<(Is+Offset)...>{}

        using Result = decltype(offset(CreateFromZero<End-Offset>{}));
    };
    template<auto End, auto Offset>
    using CreateOffset = typename CreateOffsetImpl<End, Offset>::Result;



    template<auto From, auto To, auto Skip>
    struct CreateFromToSkipImpl{
        static_assert(From <= To,  "");
        static_assert(Skip <= To-1,"");
        static_assert(From <= Skip,"");

        using Left  = CreateFromZero<Skip>;
        using Right = CreateRange<Skip+1,To>;
        
        using Result = Concat<Left, Right>;
    };
    template<auto From, auto To, auto Skip>
    using CreateFromToSkip = typename CreateFromToSkipImpl<From, To, Skip>::Result;
    


    
    template<typename Sequence>
    struct ReverseImpl;
    template<auto... Vs, auto... Is>
    constexpr auto reverse_impl(Instance<Vs...>, Instance<Is...>){
        using T = typename Instance<Vs...>::Type;
        constexpr T arr[] = { Vs... };
        return Instance<arr[sizeof...(Vs) - 1 - Is]...>{};
    }
    template<auto...Vs>
    struct ReverseImpl<Instance<Vs...>>{
        using Result = decltype(reverse_impl(Instance<Vs...>{}, CreateFromZero<sizeof...(Vs)>{}));
    };
    template<typename Sequence>
    using Reverse = typename ReverseImpl<Sequence>::Result;

    static_assert(std::is_same_v<Reverse<Instance<0,1,2>>, Instance<2,1,0>>, "");
}

static_assert(std::is_same_v<Sequence::CreateOffset<5, 0>, Sequence::Instance<0,1,2,3,4>>, "");
static_assert(std::is_same_v<Sequence::CreateFromToSkip<0, 10, 0>, Sequence::Instance<  1,2,3,4,5,6,7,8,9>>, "");
static_assert(std::is_same_v<Sequence::CreateFromToSkip<0, 10, 1>, Sequence::Instance<0,  2,3,4,5,6,7,8,9>>, "");
static_assert(std::is_same_v<Sequence::CreateFromToSkip<0, 10, 5>, Sequence::Instance<0,1,2,3,4,  6,7,8,9>>, "");
static_assert(std::is_same_v<Sequence::CreateFromToSkip<0, 10, 9>, Sequence::Instance<0,1,2,3,4,5,6,7,8  >>, "");
static_assert(std::is_same_v<Sequence::CreateRange<0, 10>, Sequence::Instance<0,1,2,3,4,5,6,7,8,9>>, "");
static_assert(std::is_same_v<Sequence::CreateFromZero<10>, Sequence::Instance<0,1,2,3,4,5,6,7,8,9>>, "");

#endif
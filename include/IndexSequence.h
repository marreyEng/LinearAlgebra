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
    using CreateFromTo = typename CreateImpl<std::common_type_t<decltype(From), decltype(To)>, From, To>::Result;
    template<auto To>
    using CreateTo = typename CreateImpl<std::common_type_t<decltype(0),decltype(To)>, 0, To>::Result;    
    
    template<auto End, auto Offset>
    struct CreateOffsetImpl{
        template<auto... Is>
        static constexpr auto offset_sequence(Instance<Is...>) -> Instance<(Is+Offset)...>{}

        using Result = decltype(offset_sequence(CreateTo<End-Offset>{}));
    };
    template<auto End, auto Offset>
    using CreateOffset = typename CreateOffsetImpl<End, Offset>::Result;


    template<auto From, auto To, auto Skip>
    struct CreateFromToSkipImpl{
        using A = CreateTo<Skip>;
        using B = CreateFromTo<Skip+1,To>;
        
        using Result = Concat<A,B>;
    };
    template<auto From, auto To, auto Skip>
    using CreateFromToSkip = typename CreateFromToSkipImpl<From, To, Skip>::Result;
}

static_assert(std::is_same_v<Sequence::CreateOffset<5, 0>, Sequence::Instance<0,1,2,3,4>>, "");
static_assert(std::is_same_v<Sequence::CreateFromToSkip<0, 10, 1>, Sequence::Instance<0,  2,3,4,5,6,7,8,9>>, "");
static_assert(std::is_same_v<Sequence::CreateFromToSkip<0, 10, 5>, Sequence::Instance<0,1,2,3,4,  6,7,8,9>>, "");
static_assert(std::is_same_v<Sequence::CreateFromToSkip<0, 10, 9>, Sequence::Instance<0,1,2,3,4,5,6,7,8  >>, "");
static_assert(std::is_same_v<Sequence::CreateFromTo<0, 10>, Sequence::Instance<0,1,2,3,4,5,6,7,8,9>>, "");
static_assert(std::is_same_v<Sequence::CreateTo<10>, Sequence::Instance<0,1,2,3,4,5,6,7,8,9>>, "");

#endif
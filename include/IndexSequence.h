#ifndef INDEX_SEQUENCE_H
#define INDEX_SEQUENCE_H

#include <type_traits>


template<size_t End, size_t Offset>
struct MakeOffsetSequenceImpl{
    template<size_t... Is>
    static constexpr auto offset_sequence(std::index_sequence<Is...>) -> std::index_sequence<(Is+Offset)...>{}

    using Result = decltype(offset_sequence(std::make_index_sequence<End-Offset>{}));
};
template<size_t Begin, size_t End>
using MakeOffsetSequence = typename MakeOffsetSequenceImpl<End, Begin>::Result;

namespace AssertMakeOffsetSequence{
    using from_0_to_10 = MakeOffsetSequence<0,10>;
    using from_1_to_10 = MakeOffsetSequence<1,10>;

    static_assert(std::is_same_v<from_0_to_10, std::index_sequence<0,1,2,3,4,5,6,7,8,9>>,    "from_0_to_10 = 0,1,2,3,4,5,6,7,8,9");
    static_assert(std::is_same_v<from_1_to_10, std::index_sequence<1,2,3,4,5,6,7,8,9>>,      "from_1_to_10 = 1,2,3,4,5,6,7,8,9");
}

//Class-like IndexSequence. Every struct is like a static method of the class
namespace IndexSequence{

    template<size_t...Is>
    struct Instance{};

    template<size_t From, size_t To,  size_t... Ns>
    struct CreateHelper{
        using Result = CreateHelper<From, To-1, To-1, Ns...>::Result;
    };
    template<size_t NM, size_t... Ns>
    struct CreateHelper<NM,NM,Ns...>{
        using Result = Instance<Ns...>;
    };

    template<size_t From, size_t To, size_t Skip, size_t... Ns>
    struct CreateSkipHelper;
    template<size_t Current, size_t End, size_t Skip, size_t... Ns>
    struct CreateSkipHelper{
        using Result = typename CreateSkipHelper<Current + 1, End, Skip, Ns..., Current>::Result;
    };
    template<size_t Skip, size_t End, size_t... Ns>
    struct CreateSkipHelper<Skip, End, Skip, Ns...>{
        using Result = typename CreateSkipHelper<Skip + 2, End, Skip, Ns..., Skip+1>::Result;
    };
    template<size_t End, size_t Skip, size_t... Ns>
    struct CreateSkipHelper<End,End,Skip,Ns...>{
        using Result = Instance<Ns...>;
    };

    //Dispatcher to:
    //If 1 arg  - Sequence<0,...>,
    //if 2 args - Sequence<Begin,End>,
    //if 3 args - Sequence<Begin,End,Skip> 
    template<size_t... Args>
    struct CreateDispatcher;
    template<size_t From, size_t To, size_t Skip>
    struct CreateDispatcher<From, To, Skip>{
        using Result = typename CreateSkipHelper<From, To, Skip>::Result;
    };
    template<size_t From, size_t To>
    struct CreateDispatcher<From, To>{
        using Result = typename CreateHelper<From, To>::Result;
    };
    template<size_t To>
    struct CreateDispatcher<To>{
        using Result = typename CreateHelper<0,To>::Result;
    };
    template<size_t... FromTo>
    using Create = typename CreateDispatcher<FromTo...>::Result;

    //Concate 2 sequences
    template<typename Seq1, typename Seq2>
    struct Concate;
    template<size_t... Is1, size_t... Is2>
    struct Concate<Instance<Is1...>, Instance<Is2...>>{
        using Result = Instance<Is1..., Is2...>;
    };


    namespace AssertIndexSequence{
        using from_0_to_10 = Create<10>;
        using from_1_to_10 = Create<1,10>;
        using from_1_to_10_skip_7 = Create<1,10,7>;

        static_assert(std::is_same_v<from_0_to_10, Instance<0,1,2,3,4,5,6,7,8,9>>,    "from_0_to_10 = 0,1,2,3,4,5,6,7,8,9");
        static_assert(std::is_same_v<from_1_to_10, Instance<1,2,3,4,5,6,7,8,9>>,      "from_1_to_10 = 1,2,3,4,5,6,7,8,9");
        static_assert(std::is_same_v<from_1_to_10_skip_7, Instance<1,2,3,4,5,6,8,9>>, "from_1_to_10_skip_7 = 1,2,3,4,5,6,8,9");
    }
}

#endif
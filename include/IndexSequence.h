#ifndef INDEX_SEQUENCE_H
#define INDEX_SEQUENCE_H

#include <type_traits>

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
using from_1_to_5_skip_3 = typename CreateSkipHelper<1,10,3>::Result;
// static_assert(std::is_same_v<from_1_to_5_skip_3, Instance<1,2,3,4>>, "");

// template<size_t From, size_t To>
// struct makeIndexSequence{
//     using Result = typename CreateHelper<From,To>::Result;
// };
// template<size_t To>
// struct makeIndexSequence<To>{
//     using Result = typename makeIndexSequenceHelper<0,To>::Result;
// };

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


template<typename Seq1, typename Seq2>
struct Concate;
template<size_t... Is1, size_t... Is2>
struct Concate<Instance<Is1...>, Instance<Is2...>>{
    using Result = Instance<Is1..., Is2...>;
};


namespace AssertIndexSequence{
using from_0_to_10 = Create<10>;
static_assert(std::is_same_v<from_0_to_10, Instance<0,1,2,3,4,5,6,7,8,9>>, "");

using from_1_to_10 = Create<1,10>;
static_assert(std::is_same_v<from_1_to_10, Instance<1,2,3,4,5,6,7,8,9>>, "");

using from_1_to_10_skip_7 = Create<1,10,7>;
static_assert(std::is_same_v<from_1_to_10_skip_7, Instance<1,2,3,4,5,6,8,9>>, "");

// using from_1_to_5_skip_3 = typename CreateSkipHelper<1,5,3>::Result;
// static_assert(std::is_same_v<from_1_to_5_skip_3, Instance<1,2,3,4>>, "");

// static_assert(std::is_same_v<from_0_to_5, indexSequence<0,1,2,3,4>>, "");
// using AA = typename Concate<A,A>::Result;
// static_assert(std::is_same_v<AA, indexSequence<0,1,2,3,4, 0,1,2,3,4>>, "");

}
}

#endif
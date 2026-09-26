#ifndef INDEX_SEQUENCE_H
#define INDEX_SEQUENCE_H

#include <type_traits>

template<size_t...Is>
struct indexSequence{};

template<size_t From, size_t To,  size_t... Ns>
struct makeIndexSequenceHelper{
    using Result = makeIndexSequenceHelper<From, To-1, To-1, Ns...>::Result;
};
template<size_t NM, size_t... Ns>
struct makeIndexSequenceHelper<NM,NM,Ns...>{
    using Result = indexSequence<Ns...>;
};

// template<size_t From, size_t To>
// struct makeIndexSequence{
//     using Result = typename makeIndexSequenceHelper<From,To>::Result;
// };
// template<size_t To>
// struct makeIndexSequence<To>{
//     using Result = typename makeIndexSequenceHelper<0,To>::Result;
// };

template<size_t... Args>
struct makeIndexSequence_wrapper;
template<size_t From, size_t To>
struct makeIndexSequence_wrapper<From, To>{
    using Result = typename makeIndexSequenceHelper<From, To>::Result;
};
template<size_t To>
struct makeIndexSequence_wrapper<To>{
    using Result = typename makeIndexSequenceHelper<0,To>::Result;
};

template<size_t... FromTo>
using makeIndexSequence_t = typename makeIndexSequence_wrapper<FromTo...>::Result;


template<typename Seq1, typename Seq2>
struct Concate;
template<size_t... Is1, size_t... Is2>
struct Concate<indexSequence<Is1...>, indexSequence<Is2...>>{
    using Result = indexSequence<Is1..., Is2...>;
};


namespace AssertIndexSequence{
using from_0_to_5 = makeIndexSequence_t<5>;
static_assert(std::is_same_v<from_0_to_5, indexSequence<0,1,2,3,4>>, "");

using from_1_to_5 = makeIndexSequence_t<1,5>;
static_assert(std::is_same_v<from_1_to_5, indexSequence<1,2,3,4>>, "");

// static_assert(std::is_same_v<from_0_to_5, indexSequence<0,1,2,3,4>>, "");
// using AA = typename Concate<A,A>::Result;
// static_assert(std::is_same_v<AA, indexSequence<0,1,2,3,4, 0,1,2,3,4>>, "");

}

#endif
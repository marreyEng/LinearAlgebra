#ifndef INDEX_SEQUENCE_H
#define INDEX_SEQUENCE_H

template<size_t...Is>
struct IndexSequence{};

template<size_t N>
struct MakeIndexSequence{
    using Result = MakeIndexSequence<>
};

template<size_t N, size_t... Ns>
struct MakeIndexSequence<1>{
    using Result = IndexSequence<N, Ns...>; 
};



#endif
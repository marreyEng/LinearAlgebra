#include <iostream>
#include <Vector.h>
#include <IndexSequence.h>

template<size_t...Is>
void print(IndexSequence::Instance<Is...>){
    (std::cout << ... << Is);
}

int main(){

    auto i = IndexSequence::Instance<0,1,2>{};
    print(i);

    return 0;
}
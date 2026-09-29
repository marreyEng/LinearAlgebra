#include <iostream>
#include <Vector.h>
#include <IndexSequence.h>

template<typename T, T...Is>
void print(Sequence::Instance<Is...>){
    (std::cout << ... << Is);
}

int main(){

    auto i = Sequence::Instance<0,1,2>{};
    
    i.print();

    return 0;
}
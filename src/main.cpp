#include <iostream>
#include <Vector.h>
#include <IndexSequence.h>

template<typename T, T...Is>
void print(Sequence::Instance<Is...>){
    (std::cout << ... << Is);
}

int main(){
    const Vector v((int)1,(int)2,(int)3);

    auto a = cross(v,v);

    for(int i = 0; i < a.Size; i++){
        std::cout << a[i] << ' ';
    }
    return 0;
}
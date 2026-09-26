#ifndef VECTOR_H
#define VECTOR_H

#include <type_traits>

template<typename T, size_t N>
class Vector{
    T data[N];
public:
    using Type = T;
    static constexpr auto Size = N;
public:
    template<typename... Us, typename = std::enable_if_t<sizeof...(Us)!=1>>
    constexpr Vector(Us&&... args)
    : data{static_cast<T>(args)...}
    {}
protected:
    template<size_t... Is>
    constexpr Vector(const Vector& o, std::index_sequence<Is...>)
    : data{o.data[Is]...}
    {}
public:
    constexpr Vector(const Vector& o)
    : Vector(o, std::make_index_sequence<N>{})
    {}


    friend constexpr Vector operator+(Vector lhs, const Vector& rhs ) {
        return lhs += rhs;
    }
    friend constexpr Vector operator-(Vector lhs, const Vector& rhs ) {
        return lhs -= rhs;
    }

protected:
    template<size_t... Is>
    constexpr Vector& IncrementImpl(const Vector& v, std::index_sequence<Is...>){
        ((data[Is] += v.data[Is]), ...);
        return *this;
    }
    template<size_t... Is>
    constexpr Vector& DecrementImpl(const Vector& v, std::index_sequence<Is...>){
        ((data[Is] -= v.data[Is]), ...);
        return *this;
    }
    template<size_t... Is>
    constexpr bool Equal(const Vector& v, std::index_sequence<Is...>) const {
        return ((data[Is] == v.data[Is]) && ...);
    }
public:
    constexpr Vector operator+=(const Vector& other) {
        return IncrementImpl(other, std::make_index_sequence<N>{});
    }
    constexpr Vector operator-=(const Vector& other) {
        return DecrementImpl(other, std::make_index_sequence<N>{});
    }
    template<typename U, size_t M>
    constexpr bool operator==(const Vector<U, M>& other) const {
        return Equal(other, std::make_index_sequence<N>{});
    }
    template<typename U, size_t M>
    constexpr bool operator!=(const Vector<U,M>& other) const {
        return !Equal(other, std::make_index_sequence<N>{});
    }

protected:
    template<size_t... Is>
    friend constexpr Type dotImpl(const Vector& lhs,const Vector& rhs, std::index_sequence<Is...>){
        return ((lhs.data[Is]*rhs.data[Is]) + ...);
    }
public:
    friend constexpr Type dot(const Vector& lhs,const Vector& rhs){
        return dotImpl(lhs, rhs, std::make_index_sequence<N>{});
    }

protected:
    template<size_t... Is>
    friend constexpr Vector crossImpl(const Vector& lhs,const Vector& rhs, std::index_sequence<Is...>){
        return {Is...};
    }
public:
    template<size_t M = N, typename = std::enable_if_t<(M == 3)>>
    friend constexpr Vector cross(const Vector& lhs, const Vector& rhs){
        return crossImpl(lhs, rhs, std::make_index_sequence<M>{});
    }
};

template<typename... Ts>
Vector(Ts&&...)->Vector<std::common_type_t<Ts...>, sizeof...(Ts)>;

namespace Asserts_Vector
{
    constexpr auto v123 = Vector(1,2,3);
    constexpr auto v456 = Vector(4,5,6);
    constexpr auto v789 = Vector(7,8,9);

    static_assert(v123 == v123, "Equal");
    static_assert(v456 == v456, "Equal");
    static_assert(v789 == v789, "Equal");
    static_assert(v123 != v456, "NotEqual");
    static_assert(v456 != v789, "NotEqual");
    static_assert(v789 != v123, "NotEqual");

    static_assert((v123 + v456) == Vector( 5, 7, 9), "Add");
    static_assert((v456 + v789) == Vector(11,13,15), "Add");
    static_assert((v789 + v123) == Vector( 8,10,12), "Add");

    static_assert((v123 - v456) == Vector(-3,-3,-3), "Sub");
    static_assert((v456 - v789) == Vector(-3,-3,-3), "Sub");
    static_assert((v789 - v123) == Vector( 6, 6, 6), "Sub");

    static_assert(dot(v123,v456) == ( 4 + 10 + 18), "Dot");
    static_assert(dot(v456,v789) == (28 + 40 + 54), "Dot");
    static_assert(dot(v789,v123) == ( 7 + 16 + 27), "Dot");

    static_assert(cross(v123,v456) == Vector<int,3>{0,1,2}, "cross");
    // static_assert(cross(v456,v789) == Vector<int,3>{}), "cross");
    // static_assert(cross(v789,v123) == Vector<int,3>{}), "cross");
}

#endif
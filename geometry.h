#pragma once
#include <cmath>
#include <cassert>
#include <iostream>
#include <concepts>

// template<int n> struct vec {
//     double data[n] = {0};
//     double& operator[](const int i)       { assert(i>=0 && i<n); return data[i]; }
//     double  operator[](const int i) const { assert(i>=0 && i<n); return data[i]; }
// };

// template<> struct vec<3> {
//     double x = 0, y = 0, z = 0;
//     double& operator[](const int i)       { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
//     double  operator[](const int i) const { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
// };

// typedef vec<3> vec3;

struct vec2 {
    double  data[2] = {0};
    constexpr double& operator[](const int i)       { assert(i>=0 && i<2); return data[i]; }
    constexpr double  operator[](const int i) const { assert(i>=0 && i<2); return data[i]; }

    constexpr vec2& operator+=(const vec2& other) noexcept {
        data[0] += other.data[0];
        data[1] += other.data[1];
        return *this;
    }
    constexpr vec2& operator-=(const vec2& other) noexcept {
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        return *this;
    }
    constexpr vec2& operator*=(const vec2& other) noexcept {
        data[0] *= other.data[0];
        data[1] *= other.data[1];
        return *this;
    }
    constexpr vec2& operator/=(const vec2& other) { 
        assert(other.data[0] != 0 && other.data[1] != 0);
        data[0] /= other.data[0];
        data[1] /= other.data[1];
        return *this;
    }

    constexpr vec2& operator*=(double scalar) noexcept {
        data[0] *= scalar;
        data[1] *= scalar;
        return *this; 
    }

    constexpr vec2& operator/=(double scalar) {
        assert(scalar != 0.0);
        data[0] /= scalar;
        data[1] /= scalar;
        return *this; 
    }

    friend constexpr vec2 operator+(vec2 a, const vec2& b) noexcept { return a += b; }
    friend constexpr vec2 operator-(vec2 a, const vec2& b) noexcept { return a -= b; }
    friend constexpr vec2 operator*(vec2 a, const vec2& b) noexcept { return a *= b; }
    friend constexpr vec2 operator/(vec2 a, const vec2& b)          { return a /= b; }

    friend constexpr vec2 operator*(vec2 a, double scalar) noexcept { return a *= scalar; }
    friend constexpr vec2 operator*(double scalar, vec2 a) noexcept { return a *= scalar; }
    friend constexpr vec2 operator/(vec2 a, double scalar)          { return a /= scalar; }
};

template<typename T>
concept FloatingPointOnly = std::same_as<T, float> || std::same_as<T, double>;

template<int n, FloatingPointOnly T = double> struct vec {
    static_assert(n > 0, "minimal size of vector is 1");

    T data[n]{};

    constexpr T& operator[](int i)       { assert(i>=0 && i<n); return data[i]; }
    constexpr T  operator[](int i) const { assert(i>=0 && i<n); return data[i]; }

    constexpr vec& operator+=(const vec& other) noexcept {
        for(int i = 0; i < n; ++i) { data[i] += other.data[i]; }
        return *this;
    }
    constexpr vec& operator-=(const vec& other) noexcept {
        for(int i = 0; i < n; ++i) { data[i] -= other.data[i]; }
        return *this;
    }
    constexpr vec& operator*=(const vec& other) noexcept {
        for(int i = 0; i < n; ++i) { data[i] *= other.data[i]; }
        return *this;
    }
    constexpr vec& operator/=(const vec& other) {
        for(int i = 0; i < n; ++i) { 
            assert(other.data[i] != 0);
            data[i] /= other.data[i]; 
        }
        return *this;
    }

    constexpr vec& operator*=(T scalar) noexcept {
        for(int i = 0; i < n; ++i) { data[i] *= scalar; }
        return *this;
    }
    constexpr vec& operator/=(T scalar) noexcept {
        assert(scalar != 0);
        for(int i = 0; i < n; ++i) { 
            data[i] /= scalar; 
        }
        return *this;
    }


    friend constexpr vec operator+(vec a, const vec& b) noexcept { return a += b; }
    friend constexpr vec operator-(vec a, const vec& b) noexcept { return a -= b; }
    friend constexpr vec operator*(vec a, const vec& b) noexcept { return a *= b; }
    friend constexpr vec operator/(vec a, const vec& b)          { return a /= b; }

    friend constexpr vec operator*(vec a, T scalar) noexcept { return a *= scalar; }
    friend constexpr vec operator*(T scalar, vec a) noexcept { return a *= scalar; }
    friend constexpr vec operator/(vec a, T scalar)          { return a /= scalar; }

};

template<int n, FloatingPointOnly T>
std::ostream& operator<<(std::ostream& out, const vec<n, T>& v) {
    for (int i = 0; i < n; ++i) out << v[i] << " ";
    return out;
}

struct mat2 {
    double data[4] = {0};

    mat2& operator*=(const mat2& other);

    mat2& transpose();
    mat2& inversion();

    friend mat2 operator*(mat2 a, mat2 b);

    // ajouter l'operation matrice x vecteur
};

struct mat3 {

};

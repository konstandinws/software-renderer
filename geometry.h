#pragma once
#include <cmath>
#include <cassert>
#include <iostream>

// template<int n> struct vec {
//     double data[n] = {0};
//     double& operator[](const int i)       { assert(i>=0 && i<n); return data[i]; }
//     double  operator[](const int i) const { assert(i>=0 && i<n); return data[i]; }
// };

template<int n> std::ostream& operator<<(std::ostream& out, const vec<n>& v) {
    for (int i=0; i<n; i++) out << v[i] << " ";
    return out;
}

template<> struct vec<3> {
    double x = 0, y = 0, z = 0;
    double& operator[](const int i)       { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
    double  operator[](const int i) const { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
};

typedef vec<3> vec3;

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

    // manque mult/div par scalaire ?
};

template<int n> struct vec {
    float data[n] = {0};
    constexpr float& operator[](int i)       { assert(i>=0 && i<n); return data[i]; }
    constexpr float  operator[](int i) const { assert(i>=0 && i<n); return data[i]; }

    constexpr vec<n>& operator+=(const vec<n>& other) noexcept {
        for(int i = 0; i < n; ++i) { data[i] += other.data[i]; }
        return *this;
    }
    constexpr vec<n>& operator-=(const vec<n>& other) noexcept {
        for(int i = 0; i < n; ++i) { data[i] -= other.data[i]; }
        return *this;
    }
    constexpr vec<n>& operator*=(const vec<n>& other) noexcept {
        for(int i = 0; i < n; ++i) { data[i] *= other.data[i]; }
        return *this;
    }
    constexpr vec<n>& operator/=(const vec<n>& other) {
        for(int i = 0; i < n; ++i) { 
            assert(other.data[i] != 0);
            data[i] /= other.data[i]; 
        }
        return *this;
    }

    constexpr vec<n>& operator*=(float scalar) noexcept {
        for(int i = 0; i < n; ++i) { data[i] *= scalar; }
        return *this;
    }
    constexpr vec<n>& operator/=(float scalar) noexcept {
        for(int i = 0; i < n; ++i) { 
            assert(scalar != 0);
            data[i] /= scalar; 
        }
        return *this;
    }


    friend constexpr vec<n> operator+(vec<n> a, const vec<n>& b) noexcept { return a += b; }
    friend constexpr vec<n> operator-(vec<n> a, const vec<n>& b) noexcept { return a -= b; }
    friend constexpr vec<n> operator*(vec<n> a, const vec<n>& b) noexcept { return a *= b; }
    friend constexpr vec<n> operator/(vec<n> a, const vec<n>& b)          { return a /= b; }

    friend constexpr vec<n> operator*(vec<n> a, float scalar) noexcept { return a *= scalar; }
    friend constexpr vec<n> operator/(vec<n> a, float scalar)          { return a /= scalar; }

};

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

#pragma once
#include <cmath>
#include <cassert>
#include <iostream>

template<int n> struct vec {
    double data[n] = {0};
    double& operator[](const int i)       { assert(i>=0 && i<n); return data[i]; }
    double  operator[](const int i) const { assert(i>=0 && i<n); return data[i]; }
};

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
    double& operator[](const int i)       { assert(i>=0 && i<2); return data[i]; }
    double  operator[](const int i) const { assert(i>=0 && i<2); return data[i]; }

    vec2& operator*=(const vec2& other);
    vec2& operator/=(const vec2& other);

    friend vec2 operator+(vec2 a, vec2& b);
    friend vec2 operator-(vec2 a, vec2& b);
    friend vec2 operator*(vec2 a, vec2& b);
    friend vec2 operator/(vec2 a, vec2& b);
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

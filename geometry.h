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

// struct vec2 {
//     double  data[2] = {0};
//     constexpr double& operator[](const int i)       { assert(i>=0 && i<2); return data[i]; }
//     constexpr double  operator[](const int i) const { assert(i>=0 && i<2); return data[i]; }

//     constexpr vec2& operator+=(const vec2& other) noexcept {
//         data[0] += other.data[0];
//         data[1] += other.data[1];
//         return *this;
//     }
//     constexpr vec2& operator-=(const vec2& other) noexcept {
//         data[0] -= other.data[0];
//         data[1] -= other.data[1];
//         return *this;
//     }
//     constexpr vec2& operator*=(const vec2& other) noexcept {
//         data[0] *= other.data[0];
//         data[1] *= other.data[1];
//         return *this;
//     }
//     constexpr vec2& operator/=(const vec2& other) { 
//         assert(other.data[0] != 0 && other.data[1] != 0);
//         data[0] /= other.data[0];
//         data[1] /= other.data[1];
//         return *this;
//     }

//     constexpr vec2& operator*=(double scalar) noexcept {
//         data[0] *= scalar;
//         data[1] *= scalar;
//         return *this; 
//     }

//     constexpr vec2& operator/=(double scalar) {
//         assert(scalar != 0.0);
//         data[0] /= scalar;
//         data[1] /= scalar;
//         return *this; 
//     }

//     friend constexpr vec2 operator+(vec2 a, const vec2& b) noexcept { return a += b; }
//     friend constexpr vec2 operator-(vec2 a, const vec2& b) noexcept { return a -= b; }
//     friend constexpr vec2 operator*(vec2 a, const vec2& b) noexcept { return a *= b; }
//     friend constexpr vec2 operator/(vec2 a, const vec2& b)          { return a /= b; }

//     friend constexpr vec2 operator*(vec2 a, double scalar) noexcept { return a *= scalar; }
//     friend constexpr vec2 operator*(double scalar, vec2 a) noexcept { return a *= scalar; }
//     friend constexpr vec2 operator/(vec2 a, double scalar)          { return a /= scalar; }
// };

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

template<int nrows, int ncols, FloatingPointOnly T = double>
struct mat {
    static_assert(nrows > 0 && ncols > 0, "Dimensions minimales de la matrice : 1x1");

    // Stockage contigu par lignes (row-major)
    vec<ncols, T> rows[nrows]{};

    // Accès ligne par ligne : permet la syntaxe naturelle m[i][j]
    constexpr vec<ncols, T>& operator[](int i) noexcept {
        assert(i >= 0 && i < nrows);
        return rows[i];
    }
    constexpr const vec<ncols, T>& operator[](int i) const noexcept {
        assert(i >= 0 && i < nrows);
        return rows[i];
    }

    // --- Matrice Identité (uniquement pour les matrices carrées) ---
    static constexpr mat identity() noexcept requires (nrows == ncols) {
        mat res{};
        for (int i = 0; i < nrows; ++i) {
            res[i][i] = static_cast<T>(1);
        }
        return res;
    }

    // --- Transposition : (nrows x ncols) devient (ncols x nrows) ---
    constexpr mat<ncols, nrows, T> transpose() const noexcept {
        mat<ncols, nrows, T> res{};
        for (int i = 0; i < nrows; ++i) {
            for (int j = 0; j < ncols; ++j) {
                res[j][i] = rows[i][j];
            }
        }
        return res;
    }

    // --- Opération Matrice * Vecteur : (nrows x ncols) * (ncols) -> (nrows) ---
    constexpr vec<nrows, T> operator*(const vec<ncols, T>& v) const noexcept {
        vec<nrows, T> res{};
        for (int i = 0; i < nrows; ++i) {
            T sum = 0;
            for (int j = 0; j < ncols; ++j) {
                sum += rows[i][j] * v[j];
            }
            res[i] = sum;
        }
        return res;
    }

    // --- operator*= (réservé aux matrices carrées) ---
    constexpr mat& operator*=(const mat<ncols, ncols, T>& other) noexcept requires (nrows == ncols) {
        *this = *this * other;
        return *this;
    }
};

// --- Produit Matriciel Rapide : (R x K) * (K x C) -> (R x C) ---
// Parcours i-k-j : vectorisation SIMD maximale et zéro cache miss
template<int R, int K, int C, FloatingPointOnly T>
constexpr mat<R, C, T> operator*(const mat<R, K, T>& a, const mat<K, C, T>& b) noexcept {
    mat<R, C, T> res{};

    for (int i = 0; i < R; ++i) {
        for (int k = 0; k < K; ++k) {
            const T a_ik = a[i][k];
            // Cette boucle interne opère sur la mémoire contiguë de res[i] et b[k]
            for (int j = 0; j < C; ++j) {
                res[i][j] += a_ik * b[k][j];
            }
        }
    }
    return res;
}
// #pragma once
// #include <cmath>
// #include <cassert>
// #include <iostream>
// #include <concepts>

// // template<int n> struct vec {
// //     double data[n] = {0};
// //     double& operator[](const int i)       { assert(i>=0 && i<n); return data[i]; }
// //     double  operator[](const int i) const { assert(i>=0 && i<n); return data[i]; }
// // };

// // template<> struct vec<3> {
// //     double x = 0, y = 0, z = 0;
// //     double& operator[](const int i)       { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
// //     double  operator[](const int i) const { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
// // };

// // typedef vec<3> vec3;

// // struct vec2 {
// //     double  data[2] = {0};
// //     constexpr double& operator[](const int i)       { assert(i>=0 && i<2); return data[i]; }
// //     constexpr double  operator[](const int i) const { assert(i>=0 && i<2); return data[i]; }

// //     constexpr vec2& operator+=(const vec2& other) noexcept {
// //         data[0] += other.data[0];
// //         data[1] += other.data[1];
// //         return *this;
// //     }
// //     constexpr vec2& operator-=(const vec2& other) noexcept {
// //         data[0] -= other.data[0];
// //         data[1] -= other.data[1];
// //         return *this;
// //     }
// //     constexpr vec2& operator*=(const vec2& other) noexcept {
// //         data[0] *= other.data[0];
// //         data[1] *= other.data[1];
// //         return *this;
// //     }
// //     constexpr vec2& operator/=(const vec2& other) { 
// //         assert(other.data[0] != 0 && other.data[1] != 0);
// //         data[0] /= other.data[0];
// //         data[1] /= other.data[1];
// //         return *this;
// //     }

// //     constexpr vec2& operator*=(double scalar) noexcept {
// //         data[0] *= scalar;
// //         data[1] *= scalar;
// //         return *this; 
// //     }

// //     constexpr vec2& operator/=(double scalar) {
// //         assert(scalar != 0.0);
// //         data[0] /= scalar;
// //         data[1] /= scalar;
// //         return *this; 
// //     }

// //     friend constexpr vec2 operator+(vec2 a, const vec2& b) noexcept { return a += b; }
// //     friend constexpr vec2 operator-(vec2 a, const vec2& b) noexcept { return a -= b; }
// //     friend constexpr vec2 operator*(vec2 a, const vec2& b) noexcept { return a *= b; }
// //     friend constexpr vec2 operator/(vec2 a, const vec2& b)          { return a /= b; }

// //     friend constexpr vec2 operator*(vec2 a, double scalar) noexcept { return a *= scalar; }
// //     friend constexpr vec2 operator*(double scalar, vec2 a) noexcept { return a *= scalar; }
// //     friend constexpr vec2 operator/(vec2 a, double scalar)          { return a /= scalar; }
// // };

// template<typename T>
// concept FloatingPointOnly = std::same_as<T, float> || std::same_as<T, double>;

// template<int n, FloatingPointOnly T = double> struct vec {
//     static_assert(n > 0, "minimal size of vector is 1");

//     T data[n]{};

//     constexpr T& operator[](int i)       { assert(i>=0 && i<n); return data[i]; }
//     constexpr T  operator[](int i) const { assert(i>=0 && i<n); return data[i]; }

//     constexpr vec& operator+=(const vec& other) noexcept {
//         for(int i = 0; i < n; ++i) { data[i] += other.data[i]; }
//         return *this;
//     }
//     constexpr vec& operator-=(const vec& other) noexcept {
//         for(int i = 0; i < n; ++i) { data[i] -= other.data[i]; }
//         return *this;
//     }
//     constexpr vec& operator*=(const vec& other) noexcept {
//         for(int i = 0; i < n; ++i) { data[i] *= other.data[i]; }
//         return *this;
//     }
//     constexpr vec& operator/=(const vec& other) {
//         for(int i = 0; i < n; ++i) { 
//             assert(other.data[i] != 0);
//             data[i] /= other.data[i]; 
//         }
//         return *this;
//     }

//     constexpr vec& operator*=(T scalar) noexcept {
//         for(int i = 0; i < n; ++i) { data[i] *= scalar; }
//         return *this;
//     }
//     constexpr vec& operator/=(T scalar) noexcept {
//         assert(scalar != 0);
//         for(int i = 0; i < n; ++i) { 
//             data[i] /= scalar; 
//         }
//         return *this;
//     }


//     friend constexpr vec operator+(vec a, const vec& b) noexcept { return a += b; }
//     friend constexpr vec operator-(vec a, const vec& b) noexcept { return a -= b; }
//     friend constexpr vec operator*(vec a, const vec& b) noexcept { return a *= b; }
//     friend constexpr vec operator/(vec a, const vec& b)          { return a /= b; }

//     friend constexpr vec operator*(vec a, T scalar) noexcept { return a *= scalar; }
//     friend constexpr vec operator*(T scalar, vec a) noexcept { return a *= scalar; }
//     friend constexpr vec operator/(vec a, T scalar)          { return a /= scalar; }

// };

// template<int n, FloatingPointOnly T>
// std::ostream& operator<<(std::ostream& out, const vec<n, T>& v) {
//     for (int i = 0; i < n; ++i) out << v[i] << " ";
//     return out;
// }

// template<int nrows, int ncols, FloatingPointOnly T = double>
// struct mat {
//     static_assert(nrows > 0 && ncols > 0, "Dimensions minimales de la matrice : 1x1");

//     // Stockage contigu par lignes (row-major)
//     vec<ncols, T> rows[nrows]{};

//     // Accès ligne par ligne : permet la syntaxe naturelle m[i][j]
//     constexpr vec<ncols, T>& operator[](int i) noexcept {
//         assert(i >= 0 && i < nrows);
//         return rows[i];
//     }
//     constexpr const vec<ncols, T>& operator[](int i) const noexcept {
//         assert(i >= 0 && i < nrows);
//         return rows[i];
//     }

//     // --- Matrice Identité (uniquement pour les matrices carrées) ---
//     static constexpr mat identity() noexcept requires (nrows == ncols) {
//         mat res{};
//         for (int i = 0; i < nrows; ++i) {
//             res[i][i] = static_cast<T>(1);
//         }
//         return res;
//     }

//     // --- Transposition : (nrows x ncols) devient (ncols x nrows) ---
//     constexpr mat<ncols, nrows, T> transpose() const noexcept {
//         mat<ncols, nrows, T> res{};
//         for (int i = 0; i < nrows; ++i) {
//             for (int j = 0; j < ncols; ++j) {
//                 res[j][i] = rows[i][j];
//             }
//         }
//         return res;
//     }

//     // --- Opération Matrice * Vecteur : (nrows x ncols) * (ncols) -> (nrows) ---
//     constexpr vec<nrows, T> operator*(const vec<ncols, T>& v) const noexcept {
//         vec<nrows, T> res{};
//         for (int i = 0; i < nrows; ++i) {
//             T sum = 0;
//             for (int j = 0; j < ncols; ++j) {
//                 sum += rows[i][j] * v[j];
//             }
//             res[i] = sum;
//         }
//         return res;
//     }

//     // --- operator*= (réservé aux matrices carrées) ---
//     constexpr mat& operator*=(const mat<ncols, ncols, T>& other) noexcept requires (nrows == ncols) {
//         *this = *this * other;
//         return *this;
//     }
// };

// // --- Produit Matriciel Rapide : (R x K) * (K x C) -> (R x C) ---
// // Parcours i-k-j : vectorisation SIMD maximale et zéro cache miss
// template<int R, int K, int C, FloatingPointOnly T>
// constexpr mat<R, C, T> operator*(const mat<R, K, T>& a, const mat<K, C, T>& b) noexcept {
//     mat<R, C, T> res{};

//     for (int i = 0; i < R; ++i) {
//         for (int k = 0; k < K; ++k) {
//             const T a_ik = a[i][k];
//             // Cette boucle interne opère sur la mémoire contiguë de res[i] et b[k]
//             for (int j = 0; j < C; ++j) {
//                 res[i][j] += a_ik * b[k][j];
//             }
//         }
//     }
//     return res;
// }

#pragma once
#include <cmath>
#include <cassert>
#include <iostream>

template<int n> struct vec {
    double data[n] = {0};
    double& operator[](const int i)       { assert(i>=0 && i<n); return data[i]; }
    double  operator[](const int i) const { assert(i>=0 && i<n); return data[i]; }
};

template<int n> double operator*(const vec<n>& lhs, const vec<n>& rhs) {
    double ret = 0;                         // N.B. Do not ever, ever use such for loops! They are highly confusing.
    for (int i=n; i--; ret+=lhs[i]*rhs[i]); // Here I used them as a tribute to old-school game programmers fighting for every CPU cycle.
    return ret;                             // Once upon a time reverse loops were faster than the normal ones, it is not the case anymore.
}

template<int n> vec<n> operator+(const vec<n>& lhs, const vec<n>& rhs) {
    vec<n> ret = lhs;
    for (int i=n; i--; ret[i]+=rhs[i]);
    return ret;
}

template<int n> vec<n> operator-(const vec<n>& lhs, const vec<n>& rhs) {
    vec<n> ret = lhs;
    for (int i=n; i--; ret[i]-=rhs[i]);
    return ret;
}

template<int n> vec<n> operator*(const vec<n>& lhs, const double& rhs) {
    vec<n> ret = lhs;
    for (int i=n; i--; ret[i]*=rhs);
    return ret;
}

template<int n> vec<n> operator*(const double& lhs, const vec<n> &rhs) {
    return rhs * lhs;
}

template<int n> vec<n> operator/(const vec<n>& lhs, const double& rhs) {
    vec<n> ret = lhs;
    for (int i=n; i--; ret[i]/=rhs);
    return ret;
}

template<int n> std::ostream& operator<<(std::ostream& out, const vec<n>& v) {
    for (int i=0; i<n; i++) out << v[i] << " ";
    return out;
}

template<> struct vec<2> {
    double x = 0, y = 0;
    double& operator[](const int i)       { assert(i>=0 && i<2); return i ? y : x; }
    double  operator[](const int i) const { assert(i>=0 && i<2); return i ? y : x; }
};

template<> struct vec<3> {
    double x = 0, y = 0, z = 0;
    double& operator[](const int i)       { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
    double  operator[](const int i) const { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
};

template<> struct vec<4> {
    double x = 0, y = 0, z = 0, w = 0;
    double& operator[](const int i)       { assert(i>=0 && i<4); return i<2 ? (i ? y : x) : (2==i ? z : w); }
    double  operator[](const int i) const { assert(i>=0 && i<4); return i<2 ? (i ? y : x) : (2==i ? z : w); }
    vec<2> xy()  const { return {x, y};    }
    vec<3> xyz() const { return {x, y, z}; }
};

typedef vec<2> vec2;
typedef vec<3> vec3;
typedef vec<4> vec4;

template<int n> double norm(const vec<n>& v) {
    return std::sqrt(v*v);
}

template<int n> vec<n> normalized(const vec<n>& v) {
    return v / norm(v);
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return {v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}

template<int n> struct dt;

template<int nrows,int ncols> struct mat {
    vec<ncols> rows[nrows] = {{}};

          vec<ncols>& operator[] (const int idx)       { assert(idx>=0 && idx<nrows); return rows[idx]; }
    const vec<ncols>& operator[] (const int idx) const { assert(idx>=0 && idx<nrows); return rows[idx]; }

    double det() const {
        return dt<ncols>::det(*this);
    }

    double cofactor(const int row, const int col) const {
        mat<nrows-1,ncols-1> submatrix;
        for (int i=nrows-1; i--; )
            for (int j=ncols-1;j--; submatrix[i][j]=rows[i+int(i>=row)][j+int(j>=col)]);
        return submatrix.det() * ((row+col)%2 ? -1 : 1);
    }

    mat<nrows,ncols> invert_transpose() const {
        mat<nrows,ncols> adjugate_transpose; // transpose to ease determinant computation, check the last line
        for (int i=nrows; i--; )
            for (int j=ncols; j--; adjugate_transpose[i][j]=cofactor(i,j));
        return adjugate_transpose/(adjugate_transpose[0]*rows[0]);
    }

    mat<nrows,ncols> invert() const {
        return invert_transpose().transpose();
    }

    mat<ncols,nrows> transpose() const {
        mat<ncols,nrows> ret;
        for (int i=ncols; i--; )
            for (int j=nrows; j--; ret[i][j]=rows[j][i]);
        return ret;
    }
};

template<int nrows,int ncols> vec<ncols> operator*(const vec<nrows>& lhs, const mat<nrows,ncols>& rhs) {
    return (mat<1,nrows>{{lhs}}*rhs)[0];
}

template<int nrows,int ncols> vec<nrows> operator*(const mat<nrows,ncols>& lhs, const vec<ncols>& rhs) {
    vec<nrows> ret;
    for (int i=nrows; i--; ret[i]=lhs[i]*rhs);
    return ret;
}

template<int R1,int C1,int C2>mat<R1,C2> operator*(const mat<R1,C1>& lhs, const mat<C1,C2>& rhs) {
    mat<R1,C2> result;
    for (int i=R1; i--; )
        for (int j=C2; j--; )
            for (int k=C1; k--; result[i][j]+=lhs[i][k]*rhs[k][j]);
    return result;
}

template<int nrows,int ncols>mat<nrows,ncols> operator*(const mat<nrows,ncols>& lhs, const double& val) {
    mat<nrows,ncols> result;
    for (int i=nrows; i--; result[i] = lhs[i]*val);
    return result;
}

template<int nrows,int ncols>mat<nrows,ncols> operator/(const mat<nrows,ncols>& lhs, const double& val) {
    mat<nrows,ncols> result;
    for (int i=nrows; i--; result[i] = lhs[i]/val);
    return result;
}

template<int nrows,int ncols>mat<nrows,ncols> operator+(const mat<nrows,ncols>& lhs, const mat<nrows,ncols>& rhs) {
    mat<nrows,ncols> result;
    for (int i=nrows; i--; )
        for (int j=ncols; j--; result[i][j]=lhs[i][j]+rhs[i][j]);
    return result;
}

template<int nrows,int ncols>mat<nrows,ncols> operator-(const mat<nrows,ncols>& lhs, const mat<nrows,ncols>& rhs) {
    mat<nrows,ncols> result;
    for (int i=nrows; i--; )
        for (int j=ncols; j--; result[i][j]=lhs[i][j]-rhs[i][j]);
    return result;
}

template<int nrows,int ncols> std::ostream& operator<<(std::ostream& out, const mat<nrows,ncols>& m) {
    for (int i=0; i<nrows; i++) out << m[i] << std::endl;
    return out;
}

template<int n> struct dt { // template metaprogramming to compute the determinant recursively
    static double det(const mat<n,n>& src) {
        double ret = 0;
        for (int i=n; i--; ret += src[0][i] * src.cofactor(0,i));
        return ret;
    }
};

template<> struct dt<1> {   // template specialization to stop the recursion
    static double det(const mat<1,1>& src) {
        return src[0][0];
    }
};

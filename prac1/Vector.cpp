#include <initializer_list>
#include "Vector.h"


template <int n>
Vector<n>::Vector() {
    arr = new float[n];
    for (int i = 0; i < n; i++) {
        arr[i] = 0.0f;
    }
}

template <int n>
Vector<n>::Vector(std::initializer_list<float> list) {
    arr = new float[n];
    int i = 0;
    for (auto tracker = list.begin(); tracker != list.end() && i < n; ++tracker, ++i) {
        arr[i] = *tracker;
    }
}

template <int n>
Vector<n>::Vector(float *list) {
    arr = list;
}

template <int n>
Vector<n>::~Vector() {
    delete[] arr;
}

template <int n>
Vector<n>::Vector(const Vector<n> &other) {
    arr = new float[n];
    for (int i = 0; i < n; i++) {
        arr[i] = other.arr[i];
    }

}

template <int n>
Vector<n>::Vector(const Matrix<n, 1> &m) {


    //should create a vector from matrix that gets passe din, row or column vector?unsure
    arr = new float[n];
    for (int i = 0; i < n; i++) {
        arr[i] = m[i][0];
}
}

template <int n>
Vector<n> &Vector<n>::operator=(const Vector<n> &other) {
    if (this != &other) {
        delete[] arr;
        arr = new float[n];
        for (int i = 0; i < n; i++) {
            arr[i] = other.arr[i];
        }
    }
    return *this;
}

template <int n>
Vector<n> Vector<n>::operator+(const Vector<n> other) const {
    Vector<n> result;
    for (int i = 0; i < n; i++) {
        result.arr[i] = arr[i] + other.arr[i];
    }
    return result;
}

template <int n>
Vector<n> Vector<n>::operator-(const Vector<n> other) const {
    Vector<n> result;
    for (int i = 0; i < n; i++) {
        result.arr[i] = arr[i] - other.arr[i];
    }
    return result;
}

template <int n>
Vector<n> Vector<n>::operator*(const float scalar) const {
    Vector<n> result;
    for (int i = 0; i < n; i++) {
        result.arr[i] = arr[i] * scalar;
    }
    return result;
}

template <int n>
float Vector<n>::operator*(const Vector<n> other) const {
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        sum += arr[i] * other.arr[i];
    }
    return sum;
}

template <int n>
float Vector<n>::magnitude() const { 
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        sum += arr[i]*arr[i];

 }
    return sqrt(sum);
}

template <int n>
Vector<n>::operator Matrix<n, 1>() const { return Matrix<n, 1>(); }

template <>
Vector<3> Vector<3>::crossProduct(const Vector<3>) const { return Vector<3>(); }

template <int n>
Vector<n> Vector<n>::unitVector() const { return Vector<n>(); }

template <int n>
int Vector<n>::getN() const { return n; }
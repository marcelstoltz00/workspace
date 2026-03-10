#ifndef VECTOR_CPP
#define VECTOR_CPP
#include <initializer_list>


template <int n>
Vector<n>::Vector() {
  arr = new float[n];
  for (int i = 0; i < n; i++) {
    arr[i] = 0;
  }
}

template <int n>
  Vector<n>::Vector(std::initializer_list<float> list){
          arr = new float[n];
  int i = 0;
  for (float waarde : list) {
    if (i < n) arr[i++] = waarde;
    else break;
  }
    };

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
    arr[i] = other[i];
  }
}

template <int n>
Vector<n>::Vector(const Matrix<n, 1> &m) {
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
      arr[i] = other[i];
    }
  }
  return *this;
}

template <int n>
Vector<n> Vector<n>::operator+(const Vector<n> other) const {
  Vector<n> re;
  for (int i = 0; i < n; i++) {
    re[i] = arr[i] + other[i];
  }
  return re;
}

template <int n>
Vector<n> Vector<n>::operator-(const Vector<n> other) const {
  Vector<n> re;
  for (int i = 0; i < n; i++) {
    re[i] = arr[i] - other[i];
  }
  return re;
}

template <int n>
Vector<n> Vector<n>::operator*(const float scalar) const {
  Vector<n> re;
  for (int i = 0; i < n; i++) {
    re[i] = arr[i] * scalar;
  }
  return re;
}

template <int n>
float Vector<n>::operator*(const Vector<n> other) const {
  float totalS = 0;
  for (int i = 0; i < n; i++) {
    totalS += arr[i] * other[i];
  }
  return totalS;
}

template <int n>
float Vector<n>::magnitude() const {
  float totalS = 0;
  for (int i = 0; i < n; i++) {
    totalS += arr[i] * arr[i];
  }
  return std::sqrt(totalS);
}

template <int n>
Vector<n>::operator Matrix<n, 1>() const {
  Matrix<n, 1> m;
  for (int i = 0; i < n; i++) {
    m[i][0] = arr[i];
  }
  return m;
}

template <>
inline Vector<3> Vector<3>::crossProduct(const Vector<3> other) const {
  Vector<3> re;
  re[0] = arr[1] * other[2] - arr[2] * other[1];
  re[1] = arr[2] * other[0] - arr[0] * other[2];
  re[2] = arr[0] * other[1] - arr[1] * other[0];
  return re;
}

template <int n>
Vector<n> Vector<n>::unitVector() const {
  Vector<n> re;
  float magga = magnitude();
  if (magga == 0) {
    throw "Invalid unit vector";
  }
  for (int i = 0; i < n; i++) {
    re[i] = arr[i] / magga;
  }
  return re;
}

template <int n>
int Vector<n>::getN() const { 
    return n; 
}

#endif
#include "Matrix.h"

template<int n, int m>
Matrix<n, m>::Matrix() {
    arr = new float*[n];
    for (int f = 0; f < n; f++) {
        arr[f] = new float[m];
        for (int s = 0; s < m; s++) {
            arr[f][s] = 0.0f;
        }
    }

}

template<int n, int m>
Matrix<n, m>::Matrix(float **list) {//shallow copy
arr = list;
}

template<int n, int m>
Matrix<n, m>::Matrix(const Matrix<n, m> &other) {
    arr = new float*[n];
    for (int f = 0; f < n; f++) {
        arr[f] = new float[m];
        for (int s = 0; s < m; s++) {
            arr[f][s] = other.arr[f][s];
        }
    }
}

template<int n, int m>
Matrix<n, m>::~Matrix() {

    for (int i = 0; i < n; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}

template<int n, int m>
Matrix<n, m> &Matrix<n, m>::operator=(const Matrix<n, m> &other) {
    if (this != &other) {
        for (int f = 0; f < n; f++) {
            for (int s = 0; s < m; s++) {
                arr[f][s] = other.arr[f][s];
            }
        }
    }
    return *this;
}

template<int n, int m>
template<int a>
Matrix<n, a> Matrix<n, m>::operator*(const Matrix<m, a> &other) const { 
    Matrix<n, a> result;
    for (int f = 0; f < n; f++) {
        for (int s = 0; s < a; s++) {
            float sum = 0.0f;
            for (int i = 0; i < m; i++) {
                sum += arr[f][i] * other.arr[i][s];
            }
            result.arr[f][s] = sum;
        }
    }
    return result;  


}

template<int n, int m>
Matrix<n, m> Matrix<n, m>::operator*(const float scalar) const { 
    Matrix<n, m> result;
    for (int f = 0; f < n; f++) {
        for (int s = 0; s < m; s++) {
            result.arr[f][s] = arr[f][s] * scalar;
        }
    }
    return result;


}

template<int n, int m>
Matrix<n, m> Matrix<n, m>::operator+(const Matrix<n, m> &other) const { 
    Matrix<n, m> result;
    for (int f = 0; f < n; f++) {
        for (int s = 0; s < m; s++) {
            result.arr[f][s] = arr[f][s] + other.arr[f][s];
        }
    }
    return result;
}

template<int n, int m>
Matrix<m, n> Matrix<n, m>::operator~() const { 
    
 }



template<int n, int m>
int Matrix<n, m>::getM() const { return m; }

template<int n, int m>
int Matrix<n, m>::getN() const { return n; }    

template<int n, int m>
float Matrix<n, m>::determinant() const { return 0.0f; }
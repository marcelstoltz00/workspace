#ifndef MATRIX_CPP
#define MATRIX_CPP
#include <initializer_list>
template <int n, int m>
Matrix<n - 1, m - 1> subDetMatrixCalculator(const Matrix<n, m> &matriks, int ry, int kol);


template <int n, int m>
struct detHelpinhHand {
    static float calculate(const Matrix<n, m>& mat) {

        if (n != m) {
            throw "Matrix is not square";
        }

      
        if (n == 2) {
             return (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);
        }


        float D = 0;
        for (int j = 0; j < n; j++) {
            float plus_neg = std::pow(-1, j);

            D += plus_neg * mat[0][j] * subDetMatrixCalculator(mat, 0, j).determinant();
        }
        return D;
    }
};


template <int m>
struct detHelpinhHand<1, m> {
    static float calculate(const Matrix<1, m>& mat) {
        return mat[0][0];
    }
};



template <int n, int m> Matrix<n, m>::Matrix() {
  arr = new float *[n];
  for (int f = 0; f < n; f++) {
    arr[f] = new float[m];

    for (int s = 0; s < m; s++) {
      arr[f][s] = 0;
    }
  }
}

template <int n, int m> Matrix<n, m>::Matrix(float **list) { 
  arr = list;
}

template <int n, int m> Matrix<n, m>::Matrix(const Matrix<n, m> &other) {
  arr = new float *[n];
  for (int f = 0; f < n; f++) {
    arr[f] = new float[m];
    for (int s = 0; s < m; s++) {
     
      arr[f][s] = other[f][s];
    }
  }
}

template <int n, int m> Matrix<n, m>::~Matrix() {
  for (int i = 0; i < n; i++) {
    delete[] arr[i];
  }
  delete[] arr;
}

template <int n, int m>
Matrix<n, m> &Matrix<n, m>::operator=(const Matrix<n, m> &other) {
  if (this != &other) {
    for (int f = 0; f < n; f++) {
      for (int s = 0; s < m; s++) {
        arr[f][s] = other[f][s];
      }
    }
  }
  return *this;
}

template <int n, int m>
template <int a>
Matrix<n, a> Matrix<n, m>::operator*(const Matrix<m, a> other) const {
  Matrix<n, a> answer;
  for (int f = 0; f < n; f++) {
    for (int s = 0; s < a; s++) {
      float total = 0;
      for (int i = 0; i < m; i++) {
        total += arr[f][i] * other[i][s];
      }
      answer[f][s] = total;
    }
  }
  return answer;
}

template <int n, int m>
Matrix<n, m> Matrix<n, m>::operator*(const float scalar) const {
  Matrix<n, m> answer;
  for (int f = 0; f < n; f++) {
    for (int s = 0; s < m; s++) {
      answer[f][s] = arr[f][s] * scalar;
    }
  }
  return answer;
}

template <int n, int m>
Matrix<n, m> Matrix<n, m>::operator+(const Matrix<n, m> other) const {
  Matrix<n, m> answer;
  for (int f = 0; f < n; f++) {
    for (int s = 0; s < m; s++) {
      answer[f][s] = arr[f][s] + other[f][s];
    }
  }
  return answer;
}

template <int n, int m> Matrix<m, n> Matrix<n, m>::operator~() const {
  Matrix<m, n> answer;
  for (int f = 0; f < n; f++) {
    for (int s = 0; s < m; s++) {
      answer[s][f] = arr[f][s];
    }
  }
  return answer;
}

template <int n, int m> int Matrix<n, m>::getM() const { return m; }

template <int n, int m> int Matrix<n, m>::getN() const { return n; }


template <int n, int m> float Matrix<n, m>::determinant() const {
    return detHelpinhHand<n, m>::calculate(*this);
}

template <int n, int m>
Matrix<n - 1, m - 1> subDetMatrixCalculator(const Matrix<n, m> &matriks, int ry,
                                            int kol) {
  Matrix<n - 1, m - 1> answer;
  int k = 0;
  for (int i = 0; i < n; i++) {
    if (i == ry)
      continue;
    int Cv = 0;

    for (int j = 0; j < m; j++) {
      if (j == kol)
        continue;
      Cv++;
      answer[k][Cv - 1] = matriks[i][j];
    }
    k++;
  }
  return answer;
}

#endif
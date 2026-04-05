#ifndef CIRCLE_CPP
#define CIRCLE_CPP
#define CIRCLE_CPP_COMPILATION_UNIT
#include "Circle.h"
#include <cmath>
#include <iostream>

template <int n>
Circle<n>::Circle(const Vector<n> &center, float radius, int vertexCount) {
  this->center = center;
  this->radius = radius;
  this->vertexCount = vertexCount;

  if (this->radius < 0.0f) {
    this->radius = -this->radius;
  }
  if (this->vertexCount < 3) {
    this->vertexCount = 3;
  }
}

template <int n>
Circle<n>::Circle(const Circle<n> &other) {
  this->center = other.center;
  this->radius = other.radius;
  this->vertexCount = other.vertexCount;
}

template <int n>
Circle<n> &Circle<n>::operator*=(const Matrix<n, n> &other) {
  Vector<n> oldCenter = center;
  Vector<n> oldEdge = center;
  oldEdge[0] += radius;

  center = Vector<n>(other * static_cast<Matrix<n, 1>>(oldCenter));
  Vector<n> newEdge = Vector<n>(other * static_cast<Matrix<n, 1>>(oldEdge));

  float sum = 0.0f;
  for (int i = 0; i < n; i++) {
    float d = newEdge[i] - center[i];
    sum += d * d;
  }
  radius = std::sqrt(sum);

  return *this;
}

template <int n>
Circle<n> *Circle<n>::operator*(const Matrix<n, n> &other) const {
  Circle<n> *Circle_2 = new Circle<n>(*this);
  *Circle_2 *= other;
  return Circle_2;
}

template <int n>
float *Circle<n>::getPoints() const {
  float *points = new float[vertexCount * n];
  const float twoPi = 6.28318530717958647692f;

  for (int i = 0; i < vertexCount; i++) {
    float angle = (twoPi * static_cast<float>(i)) / static_cast<float>(vertexCount);

    Vector<n> p = center;
    p[0] = center[0] + radius * std::cos(angle);
    if (n >= 2) {
      p[1] = center[1] + radius * std::sin(angle);
    }

    for (int d = 0; d < n; d++) {
      points[i * n + d] = p[d];
    }
  }

  return points;
}

template <int n>
int Circle<n>::getNumPoints() const {
  return vertexCount;
}

template <int n>
void Circle<n>::print() const {
  std::cout << "_ CENTER _ " << std::endl;
  center.print();
  std::cout << "Radius: " << radius << std::endl;
  std::cout << "Vertices: " << vertexCount << std::endl;
}

#endif


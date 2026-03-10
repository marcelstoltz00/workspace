// Circle.cpp
// Responsibility:
// - Implements Circle construction and vertex generation.
// - Converts center/radius/vertex-count into drawable point data.
// - Implements matrix transform behavior declared in Circle.h.

#ifndef CIRCLE_CPP
#define CIRCLE_CPP

#define CIRCLE_CPP_COMPILATION_UNIT

#include "Circle.h"
#include <cmath>
#include <iostream>

template <int n>
Circle<n>::Circle(const Vector<n>& center, float radius, int vertexCount)
	: center(center), radius(radius), vertexCount(vertexCount)
{
	if (this->radius < 0.0f)
	{
		this->radius = -this->radius;
	}
	if (this->vertexCount < 3)
	{
		this->vertexCount = 3;
	}
}

template <int n>
Circle<n>::Circle(const Circle<n>& other)
	: center(other.center), radius(other.radius), vertexCount(other.vertexCount)
{
}

template <int n>
Circle<n>& Circle<n>::operator*=(const Matrix<n, n>& matrix)
{
	Vector<n> oldCenter = center;
	Vector<n> oldEdge = center;
	oldEdge[0] += radius;

	center = Vector<n>(matrix * static_cast<Matrix<n, 1>>(oldCenter));
	Vector<n> newEdge = Vector<n>(matrix * static_cast<Matrix<n, 1>>(oldEdge));

	float sum = 0.0f;
	for (int i = 0; i < n; i++)
	{
		float d = newEdge[i] - center[i];
		sum += d * d;
	}
	radius = std::sqrt(sum);

	return *this;
}

template <int n>
Circle<n>* Circle<n>::operator*(const Matrix<n, n>& matrix) const
{
	Circle<n>* c = new Circle<n>(*this);
	*c *= matrix;
	return c;
}

template <int n>
float* Circle<n>::getPoints() const
{
	float* points = new float[vertexCount * n];
	const float twoPi = 6.28318530717958647692f;

	for (int i = 0; i < vertexCount; i++)
	{
		float angle = (twoPi * static_cast<float>(i)) / static_cast<float>(vertexCount);

		Vector<n> p = center;
		p[0] = center[0] + radius * std::cos(angle);
		if (n >= 2)
		{
			p[1] = center[1] + radius * std::sin(angle);
		}

		for (int d = 0; d < n; d++)
		{
			points[i * n + d] = p[d];
		}
	}

	return points;
}

template <int n>
int Circle<n>::getNumPoints() const
{
	return vertexCount;
}

template <int n>
void Circle<n>::print() const
{
	std::cout << "Circle center:" << std::endl;
	center.print();
	std::cout << "Radius: " << radius << std::endl;
	std::cout << "Vertices: " << vertexCount << std::endl;
}

#endif /*CIRCLE_CPP*/


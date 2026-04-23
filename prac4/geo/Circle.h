#ifndef CIRCLE_H
#define CIRCLE_H

#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "Shape.h"

template <int n>
class Circle: public Shape<n> {
	private:
		Vector<n> center;
		float radius;
		int vertexCount;
	public:
		Circle(const Vector<n>& center, float radius, int vertexCount);
		Circle(const Circle<n>&);
		virtual Circle<n>& operator*=(const Matrix<n,n>&);
		virtual Circle<n>* operator*(const Matrix<n,n>&) const;
		virtual float* getPoints() const;
		virtual int getNumPoints() const;
		virtual void print() const;

		   virtual ~Circle() {}
};

#ifndef CIRCLE_CPP_COMPILATION_UNIT
#include "Circle.cpp"
#endif

#endif /*CIRCLE_H*/


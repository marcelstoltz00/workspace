#ifndef SHAPE_H
#define SHAPE_H

#include "../math/Matrix.h"

#include <iostream>

template <int n>
class Shape{
    public:
        virtual Shape& operator*=(const Matrix<n,n>&) =0;
        virtual Shape* operator*(const Matrix<n,n>&) const =0;
        virtual float* getPoints() const =0;
        virtual int getNumPoints() const =0;
        virtual void print() const =0;
    virtual ~Shape() {}
     
};

#endif /*SHAPE_H*/
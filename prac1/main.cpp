#include <iostream>
#include "Matrix.h" 
#include "Vector.h" 
#include "Triangle.h"
#include "Square.h"
using namespace std;

void test2() {

    Matrix<3, 3> m1;
    

    int count = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m1[i][j] = count++;
        }
    }

    m1.print();
}

void test3() {

    Matrix<2, 2> A;
    A[0][0] = 1; A[0][1] = 2;
    A[1][0] = 3; A[1][1] = 4;

    Matrix<2, 2> B;
    B[0][0] = 5; B[0][1] = 6;
    B[1][0] = 7; B[1][1] = 8;


    (A + B).print();


    (A * 2.0f).print();


    (A * B).print();
    
   
    (~A).print();
}

void test4() {

    Matrix<1, 1> single;
    single[0][0] = 42;
    cout <<  single.determinant() << endl;


    Matrix<2, 2> two;
    two[0][0] = 4; two[0][1] = 6;
    two[1][0] = 3; two[1][1] = 8;

    cout  << two.determinant() << endl;

   
    Matrix<3, 3> three;
    three[0][0] = 6; three[0][1] = 1; three[0][2] = 1;
    three[1][0] = 4; three[1][1] = -2; three[1][2] = 5;
    three[2][0] = 2; three[2][1] = 8; three[2][2] = 7;

    cout <<  three.determinant() << endl;
}

void test5() {

    Vector<3> v1 = {1.0, 2.0, 3.0};
    Vector<3> v2 = {4.0, 5.0, 6.0};

 v1.print();
    v2.print();

(v1 + v2).print();
    cout<< (v1 * v2) << endl;
    

    v1.crossProduct(v2).print();

    cout  << v1.magnitude() << endl;
}





void test6() {
  


    Vector<3> p1 = {0, 0, 0};
    Vector<3> p2 = {1, 0, 0};
    Vector<3> p3 = {0, 1, 0};
    
    Triangle<3> t1(p1, p2, p3);

    t1.print();



    float* points = t1.getPoints();
    int numPoints = t1.getNumPoints();

    for(int i = 0; i < numPoints * 3; i++) {
        cout << points[i] << " ";
    }
    cout << endl;
    delete[] points; 



    Matrix<3, 3> scaleMat;
    scaleMat[0][0] = 2; scaleMat[1][1] = 2; scaleMat[2][2] = 2; 
    
   
    Triangle<3>* t2 = t1 * scaleMat; 

    t2->print();
    delete t2;

  
 
    t1 *= scaleMat;
    t1.print();
}

void test1() {



    Vector<2> tl = {0, 1};
    Vector<2> tr = {1, 1};
    Vector<2> br = {1, 0};
    Vector<2> bl = {0, 0};

    Square<2> sq1(tl, tr, br, bl);

    sq1.print();


    Vector<2> center = {0, 0};
  
    Square<2> sq2(center, 2.0f, 2.0f); 
    sq2.print(); 

    Matrix<2, 2> rotMat;
    rotMat[0][0] = 0; rotMat[0][1] = -1;
    rotMat[1][0] = 1; rotMat[1][1] = 0;


    sq1 *= rotMat;
    sq1.print();
    
  
    float* sqPoints = sq1.getPoints();

    for(int i = 0; i < 4 * 2; i++) { 
        cout << sqPoints[i] << " ";
    }
    cout << endl;
    delete[] sqPoints;
}

int main() {
    try {
        test2();
        test3();
        test4();
        test5();
		test6();
		test1();
    } catch (const char* e) {
		
	} catch (...) {
		
	}; 

    return 0;
}
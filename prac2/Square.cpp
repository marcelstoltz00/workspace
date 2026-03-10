#ifndef SQUARE_CPP
#define SQUARE_CPP

template <int n>
Square<n>::Square(const Vector<n>& center, float height, float width) {
    tl = center;tr = center; br = center;bl = center;

        tr[0] = center[0] + width / 2;tl[0] = center[0] - width / 2;
       br[0] = center[0] + width / 2; bl[0] = center[0] - width / 2;



    if (n >= 2) {
           tr[1] = center[1] + height / 2;         bl[1] = center[1] - height / 2;  br[1] = center[1] - height / 2;  tl[1] = center[1] + height / 2;
      

  
    }
}

template <int n>
Square<n>::Square(const Vector<n>& tl, const Vector<n>& tr, const Vector<n>& br, const Vector<n>& bl) {
      this->tr = tr;  this->br = br; this->bl = bl; this->tl = tl;
    
  

}

template <int n>
Square<n>::Square(const Square<n>& other) {
    this->br = other.br;  this->tr = other.tr;this->bl = other.bl;this->tl = other.tl;
  
    
    
}

template <int n>
Square<n>& Square<n>::operator*=(const Matrix<n, n>& other) {
         bl = Vector<n>(other * static_cast<Matrix<n, 1>>(bl)); br = Vector<n>(other * static_cast<Matrix<n, 1>>(br)); tr = Vector<n>(other * static_cast<Matrix<n, 1>>(tr)); tl = Vector<n>(other * static_cast<Matrix<n, 1>>(tl));
  

  
    return *this;
}

template <int n>
Square<n>* Square<n>::operator*(const Matrix<n, n>& other) const {
    Square<n>* square = new Square<n>(*this);
    *square *= other;
    return square;
}

template <int n>
float* Square<n>::getPoints() const {
    float* punte = new float[4 * n];
    for (int i = 0; i < n; i++) {
        punte[i] = tl[i];
        punte[i + n] = tr[i];
        punte[i + 2 * n] = br[i];
        punte[i + 3 * n] = bl[i];
    }
    return punte;
}

template <int n>
int Square<n>::getNumPoints() const {
    return 4;
}

#endif
/*
  Matrix.hpp
  "overturner"

  Created by Renaud Dufays on 04/04/17
  Copyright © 2017. All rights reserved.
*/

#ifndef MATRIXHEADERDEF
#define MATRIXHEADERDEF

#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

class Matrix
{
    private:
        double ** mData; // entries of matrix
        int mDim1, mDim2; // dimensions
    public:
        Matrix(const Matrix& otherMatrix);
        Matrix(int dim1, int dim2);
        ~Matrix();
        void Print(std::string filename, bool binary=false) const;
        void Print(std::ofstream& f) const;
        void PrintBinary(std::ofstream& f) const;
        void Display() const;
        int GetDim1() const;
        int GetDim2() const;
        double& operator()(int i, int j); //0-based indexing
        Matrix& operator=(const Matrix& f);
        Matrix& operator*=(const double a);
        Matrix& operator/=(const double a);
};

Matrix operator*(const Matrix& f, const double a);
Matrix operator*(const double a, const Matrix& f);
Matrix operator/(const Matrix& f, const double a);

#endif
/*
  Matrix.cpp
  "sde2D"

  Created by Renaud Dufays on 04/04/17
  Copyright © 2017. All rights reserved.
*/
#include "Matrix.hpp"

// Overwritten copy constructor
// Allocate memory for new matrix, and copy
// entries into this matrix
Matrix::Matrix(const Matrix& m)
{
    mDim1 = m.mDim1;
    mDim2 = m.mDim2;
    mData = new double * [mDim1];
    for (int i=0; i<mDim1; i++)
    {
        mData[i] = new double [mDim2];
    }
    for (int i=0; i<mDim1; i++)
    {
        for (int j=0; j<mDim2; j++)
        {
            mData[i][j] = m.mData[i][j];
        }
    }
}
// Constructor for matrix of a given length
// Allocates memory, and initialises entries
// to zero
Matrix::Matrix(int dim1, int dim2)
{
    assert(dim1 > 0);
    assert(dim2 > 0);
    mDim1 = dim1;
    mDim2 = dim2;
    mData = new double * [mDim1];
    for (int i=0; i<mDim1; i++)
    {
        mData[i] = new double [mDim2];
    }
    for (int i=0; i<mDim1; i++)
    {
        for (int j=0; j<mDim2; j++)
        {
            mData[i][j] = 0.0;
        }
    }
}

// Overwritten destructor to correctly free memory
Matrix::~Matrix()
{
    for (int i=0; i<mDim1; i++)
    {
        delete[] mData[i];
    }
    delete[] mData;
}

void Matrix::Print(std::string filename, bool binary) const
{
    std::ofstream myfile;
    if (binary){
        myfile.open(filename.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
        for (int i=0; i<mDim1; i++)
        {
            myfile.write((char*) &mData[i][0], mDim2*sizeof(double));
        } 
    }
    else{  
        myfile.open(filename.c_str(), std::ios::out | std::ios::trunc);
        if (myfile.is_open())
        {
            myfile.setf(std::ios::scientific);
            myfile.precision(10);
            for(int j=0; j<mDim2; j++)
            {
                for(int i=0; i<mDim1; i++)
                {
                    myfile << mData[i][j] << " ";
                }
                myfile << "\n";
            }
            myfile.close();
        }
        else
        {
            std::cerr << "Unable to open file " << filename << "\n";
            abort();
        }
    }
}

void Matrix::Print(std::ofstream& f) const
{
    for(int i=0; i<mDim1; i++)
    {
        for(int j=0; j<mDim2; j++)
        {
            f << mData[i][j] << " ";
        }
        f << "\n";
    }
}

void Matrix::PrintBinary(std::ofstream& f) const
{
    for(int i=0; i<mDim1; i++)
    {
        f.write((char*) &mData[i][0], mDim2*sizeof(double));
    }
}

void Matrix::Display() const
{
    for(int j=0; j<mDim2; j++)
    {
        for(int i=0; i<mDim1; i++)
        {
            std::cout << mData[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Method to get number of rows of matrix
int Matrix::GetDim1() const
{
    return mDim1;
}
// Method to get number of columns of matrix
int Matrix::GetDim2() const
{
    return mDim2;
}

double Matrix::Get(int i, int j) const
{
    assert(i > -1);
    assert(i < mDim1);
    assert(j > -1);
    assert(j < mDim2);
    return mData[i][j];
}
// Overloading the round brackets
// Note that this uses 'zero-based’ indexing,
// and a check on the validity of the index
double& Matrix::operator()(int i, int j)
{
    // assert(i > -1);
    // assert(i < mDim1);
    // assert(j > -1);
    // assert(j < mDim2);
    if (i < 0 || i >= mDim1 || j < 0 || j >= mDim2)
    {
        std::cerr << "(i,j) = (" << i << ", " << j << ") out of dim. mDim1 = " << mDim1 << ", mDim2 = " << mDim2 << std::endl;
        abort();
    }
    return mData[i][j];
}

Matrix& Matrix::operator=(const Matrix& m)
{
    if (this != &m) // self-assignment check
    {
        if (m.mDim1 != mDim1 || m.mDim2 != mDim2) 
        { // storage cannot be reused : delete and realloc
            std::cerr << "HEY ! Not supposed to reallocated space for the matrix..." << std::endl;
            for (int i=0; i<mDim1; i++)
            {
                delete[] mData[i];
            }
            delete[] mData;

            mDim1 = m.mDim1;
            mDim2 = m.mDim2;
    
            mData = new double * [mDim1];
            for (int i=0; i<mDim1; i++)
            {
                mData[i] = new double [mDim2];
            }
        }
        for (int i=0; i<mDim1; i++)
        {
            for (int j=0; j<mDim2; j++)
            {
                mData[i][j] = m.mData[i][j];
            }
        }
    }
    return *this;
}

// Overloading the unary + operator
Matrix Matrix::operator+() const
{
    Matrix mout(mDim1,mDim2);
    for (int i=0; i<mDim1; i++){
        for (int j=0; j<mDim2; j++){
            mout(i,j) = mData[i][j];
        }
    }
    return mout;
}
// Overloading the unary - operator
Matrix Matrix::operator-() const
{
    Matrix mout(mDim1,mDim2);
    for (int i=0; i<mDim1; i++){
        for (int j=0; j<mDim2; j++){
            mout(i,j) = -mData[i][j];
        }
    }
    return mout;
}
// Overloading the binary + operator
Matrix Matrix::operator+(const Matrix& m) const
{
    assert(mDim1 == m.mDim1);
    assert(mDim2 == m.mDim2);
    Matrix mout(mDim1, mDim2);
    for (int i=0; i<mDim1; i++){
        for (int j=0; j<mDim2; j++){
            mout(i,j) = mData[i][j] + m.mData[i][j];
        }
    }
    return mout;
}
// Overloading the binary - operator
Matrix Matrix::operator-(const Matrix& m) const
{
    assert(mDim1 == m.mDim1);
    assert(mDim2 == m.mDim2);
    Matrix mout(mDim1, mDim2);
    for (int i=0; i<mDim1; i++){
        for (int j=0; j<mDim2; j++){
            mout(i,j) = mData[i][j] - m.mData[i][j];
        }
    }
    return mout;
}

Matrix& Matrix::operator+=(const Matrix& m) // binary +=
{
    *this = *this + m;
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& m) // binary -=
{
    *this = *this - m;
    return *this;
}

Matrix& Matrix::operator+=(const double a)
{
    *this = *this+a;
    return *this;
}

Matrix& Matrix::operator-=(const double a)
{
    *this = *this-a;
    return *this;
}

Matrix& Matrix::operator*=(const double a)
{
    *this = *this*a;
    return *this;
}

Matrix& Matrix::operator/=(const double a)
{
    *this = *this/a;
    return *this;
}

Matrix operator*(const Matrix& m, const double a)
{
    Matrix mout(m);
    for (int i=0; i<m.GetDim1(); i++)
    {
        for (int j=0; j<m.GetDim2(); j++)
        {
            mout(i,j) *= a;
        }
    }

    return mout;
}

Matrix operator*(const double a, const Matrix& m)
{
    return m*a;
}

Matrix operator/(const Matrix& m, const double a)
{
    return m*(1./a);
}

Matrix operator+(const Matrix& m, const double a)
{
    Matrix mout(m);
    for (int i=0; i<m.GetDim1(); i++){
        for (int j=0; j<m.GetDim2(); j++){
            mout(i,j) += a;
        }
    }
    return mout;
}

Matrix operator+(const double a, const Matrix& m)
{
    return m+a;
}

Matrix operator-(const Matrix& m, const double a)
{
    return m+(-a);
}

double Frobenius(const Matrix& m)
{
    double F = 0.;
    for (int i=0; i<m.GetDim1(); i++){
        for (int j=0; j<m.GetDim2(); j++){
            F += m.Get(i,j)*m.Get(i,j);
        }
    }
    return sqrt(F);
}
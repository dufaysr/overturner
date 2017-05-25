#include "Matrix.hpp"

// Overwritten copy constructor
// Allocate memory for new matrix, and copy
// entries into this matrix
Matrix::Matrix(const Matrix& otherMatrix)
{
    mDim1 = otherMatrix.mDim1;
    mDim2 = otherMatrix.mDim2;
    mData = new double * [mDim1];
    for (int i=0; i<mDim1; i++)
    {
        mData[i] = new double [mDim2];
    }
    for (int i=0; i<mDim1; i++)
    {
        for (int j=0; j<mDim2; j++)
        {
            mData[i][j] = otherMatrix.mData[i][j];
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
// Overloading the round brackets
// Note that this uses 'zero-based’ indexing,
// and a check on the validity of the index
double& Matrix::operator()(int i, int j)
{
    assert(i > -1);
    assert(i < mDim1);
    assert(j > -1);
    assert(j < mDim2);
    return mData[i][j];
}

Matrix& Matrix::operator=(const Matrix& f)
{
    if (this != &f) // self-assignment check
    {
        if (f.mDim1 != mDim1 || f.mDim2 != mDim2) 
        { // storage cannot be reused : delete and realloc
            for (int i=0; i<mDim1; i++)
            {
                delete[] mData[i];
            }
            delete[] mData;

            mDim1 = f.mDim1;
            mDim2 = f.mDim2;
    
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
                mData[i][j] = f.mData[i][j];
            }
        }
    }
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

Matrix operator*(const Matrix& f, const double a)
{
    Matrix fout(f);
    for (int i=0; i<f.GetDim1(); i++)
    {
        for (int j=0; j<f.GetDim2(); j++)
        {
            fout(i,j) *= a;
        }
    }

    return fout;
}

Matrix operator*(const double a, const Matrix& f)
{
    return f*a;
}

Matrix operator/(const Matrix& f, const double a)
{
    return f*(1./a);
}
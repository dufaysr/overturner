#include "Field.hpp"

// Overwritten copy constructor
// Allocate memory for new matrix, and copy
// entries into this matrix
Field::Field(const Field& otherField)
{
    mDim1 = otherField.mDim1;
    mDim2 = otherField.mDim2;
    mData = new double * [mDim1];
    for (int i=0; i<mDim1; i++)
    {
        mData[i] = new double [mDim2];
    }
    for (int i=0; i<mDim1; i++)
    {
        for (int j=0; j<mDim2; j++)
        {
            mData[i][j] = otherField.mData[i][j];
        }
    }
}
// Constructor for matrix of a given length
// Allocates memory, and initialises entries
// to zero
Field::Field(int dim1, int dim2)
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
Field::~Field()
{
    for (int i=0; i<mDim1; i++)
    {
        delete[] mData[i];
    }
    delete[] mData;
}

void Field::Print(std::string filename) const
{
    std::ofstream myfile (filename.c_str(), std::ios::out | std::ios::trunc);
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
        std::cout << "Unable to open file " << filename << "\n";
    }
}

void Field::Display() const
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
int Field::GetDim1() const
{
    return mDim1;
}
// Method to get number of columns of matrix
int Field::GetDim2() const
{
    return mDim2;
}
// Overloading the round brackets
// Note that this uses ‘one-based’ indexing,
// and a check on the validity of the index
double& Field::operator()(int i, int j)
{
    assert(i > -1);
    assert(i < mDim1);
    assert(j > -1);
    assert(j < mDim2);
    return mData[i][j];
}

Field& Field::operator=(const Field& f)
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

Field& Field::operator/=(const double a)
{
    *this = *this/a;
    return *this;
}

Field operator/(const Field& f, const double a)
{
    Field fout(f);
    for (int i=0; i<f.GetDim1(); i++)
    {
        for (int j=0; j<f.GetDim2(); j++)
        {
            fout(i,j) /= a;
        }
    }

    return fout;
}
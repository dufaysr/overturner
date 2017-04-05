/*
  Field.hpp
  "overturner"

  Created by Renaud Dufays on 04/04/17
  Copyright © 2017. All rights reserved.
*/

#ifndef FIELDHEADERDEF
#define FIELDHEADERDEF

#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

class Field
{
    private:
        double ** mData; // entries of matrix
        int mDim1, mDim2; // dimensions
    public:
        Field(const Field& otherField);
        Field(int dim1, int dim2);
        ~Field();
        void Print(std::string filename) const;
        void Display() const;
        int GetDim1() const;
        int GetDim2() const;
        double& operator()(int i, int j); //0-based indexing
        Field& operator=(const Field& f);
        Field& operator/=(const double a);
};

Field operator/(const Field& f, const double a);
#endif
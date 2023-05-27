//
// Created by Tomek on 25.05.2023.
//

#ifndef MN_MATRICES_CPP_VECTOR_H
#define MN_MATRICES_CPP_VECTOR_H

#endif //MN_MATRICES_CPP_VECTOR_H

class Matrix;

class Vector
{
private:


    int length;

    double* vector;

public:
    enum Creation_flags
    {
        ROW = 1,
        COLUMN = 2
        // = 4
    };

    Vector();

    Vector(int length, double* arr);

    Vector(int length, double initializing_value);

    Vector(const Matrix & matrix, int index, Creation_flags flag);

    ~Vector();

    double & operator[](int column) const;

    friend class Matrix;
};
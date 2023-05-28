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
    Vector();

    Vector(int length, double* arr);

    Vector(int length, double initializing_value);

    ~Vector();

    double &operator[](int column) const;

    friend class Matrix;
};
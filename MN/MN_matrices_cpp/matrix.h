#include <iostream>
#include "vector.h"

#ifndef MN_MATRICES_CPP_MATRIX_H
#define MN_MATRICES_CPP_MATRIX_H

#endif //MN_MATRICES_CPP_MATRIX_H

#define PRINT_MATRIX_OPERATIONS

/*
 *                  array of pointers               array of doubles
 *  Matrix => matrix ->    []    -> Vector => vector ->    []
 *                         []    ->                        []
 *                         []    ->                        []
 *                        ...                             ...
 */

class Matrix
{
private:
    Vector** matrix; // array of Vector pointers
    int rows, columns;

public:
//    enum Creation_flags
//    {
//        IDENTITY = 1,
//        BAND = 2,
//        // = 4
//        // ...
//    };

    int get_rows() const;

    int get_columns() const;

    Matrix(int rows, int columns, double initiliazing_value);

    Matrix(Matrix const &matrix_to_copy_from);

    Matrix(Vector const &vector_to_copy_from);

    Matrix(double (* func)(int), int rows, int columns);

    ~Matrix();

    static Matrix identity(int size);

    static Matrix band(int size, double* values, int values_length);

    Vector & operator[](int row) const;

    Matrix operator+(const Matrix& right_matrix) const;

    Matrix operator-(const Matrix& right_matrix) const;

    Matrix operator+(double scalar) const;

    Matrix operator-(double scalar) const;

    Matrix operator*(const Matrix& right_matrix) const;

    friend std::ostream& operator<< (std::ostream& out, const Matrix& matrix);
};
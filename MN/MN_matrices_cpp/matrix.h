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
    enum Creation_flags
    {
        IDENTITY = 1,
        // = 2
        // = 4
        // ...
    };
private:
    Vector** matrix; // array of Vector pointers
    int rows, columns;

public:

    int get_rows() const;

    int get_columns() const;

    Matrix(int rows, int columns, double initiliazing_value);

    Matrix(int rows, int columns, Creation_flags flags);

    Matrix(Matrix const &matrix_to_copy_from);

    Matrix(Vector const &vector_to_copy_from);

    ~Matrix();

    Vector & operator[](int row) const;

    Matrix operator+(const Matrix& right_matrix) const;

    Matrix operator-(const Matrix& right_matrix) const;

    Matrix operator+(double scalar) const;

    Matrix operator-(double scalar) const;

    Matrix operator*(const Matrix& right_matrix) const;

    friend std::ostream& operator<< (std::ostream& out, const Matrix& matrix);


};
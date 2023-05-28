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

    bool is_lower_triangular() const;

    bool is_upper_triangular() const;

    bool is_square() const;

    Matrix(int rows, int columns, double initiliazing_value);

    Matrix(Matrix const &matrix_to_copy_from);

    Matrix(int rows, int columns, double (* func)(int));

    Matrix(int rows, int columns, double** arr);

    ~Matrix();

    static Matrix identity(int size);

    static Matrix band(int size, double* values, int values_length);

    static Matrix jacobi_solve(const Matrix & matrix_A, const Matrix & vector_b, double max_error);

    static Matrix get_lower_triangular(const Matrix &matrix, bool with_diagonal);

    static Matrix get_upper_triangular(const Matrix &matrix, bool with_diagonal);

    static double norm(const Matrix & vector);

    Vector &operator[](int row) const;

    Matrix operator!() const; // transposition

    Matrix operator+(const Matrix &right_matrix) const;

    Matrix operator-(const Matrix &right_matrix) const;

    Matrix operator+(double scalar) const;

    Matrix operator-(double scalar) const;

    Matrix operator*(const Matrix &right_matrix) const;

    Matrix operator%(const Matrix &matrix_B) const; // forward, backward substitution

    Matrix operator-() const;

    friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);
};
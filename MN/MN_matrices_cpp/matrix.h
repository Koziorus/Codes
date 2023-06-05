#include <iostream>
#include "vector.h"

#ifndef MN_MATRICES_CPP_MATRIX_H
#define MN_MATRICES_CPP_MATRIX_H

#endif //MN_MATRICES_CPP_MATRIX_H

//#define PRINT_MATRIX_OPERATIONS // used to print whenever operators are used

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

    Matrix(int rows, int columns, double initializing_value);

    Matrix(Matrix const &matrix_to_copy_from);

    Matrix(int rows, int columns, double (* func)(int));

    Matrix(int rows, int columns, double** arr);

    ~Matrix();

    static Matrix band(int size, const double* values, int values_length);

    static Matrix identity(int size);

    static Matrix jacobi_solve(const Matrix &A, const Matrix &b, double max_error, std::string residual_out_path);

    static Matrix gauss_solve(const Matrix &A, const Matrix &b, double max_error, std::string residual_out_path);

    static void LU_decomposition(const Matrix &M, Matrix &L, Matrix &U);

    static Matrix LU_factorization_solve(const Matrix &A, const Matrix &b);

    static Matrix get_lower_triangular(const Matrix &matrix, bool with_diagonal);

    static Matrix get_upper_triangular(const Matrix &matrix, bool with_diagonal);

    static Matrix get_diagonal(const Matrix &matrix);

    static double norm(const Matrix &vector);

    Vector &operator[](int row) const;

    Matrix operator!() const; // transposition

    Matrix operator+(const Matrix &right_matrix) const;

    Matrix operator-(const Matrix &right_matrix) const;

    Matrix operator+(double scalar) const;

    Matrix operator-(double scalar) const;

    Matrix operator*(const Matrix &right_matrix) const;

    Matrix operator%(const Matrix &matrix_B) const; // forward, backward substitution

    Matrix operator-() const;

    Matrix& operator=(const Matrix &right_matrix);

    friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);
};
//
// Created by Tomek on 25.05.2023.
//

#include <iomanip>
#include "matrix.h"

int Matrix::get_rows() const
{
    return rows;
}

int Matrix::get_columns() const
{
    return columns;
}

Matrix::Matrix(int rows, int columns, double initiliazing_value)
{
    this->rows = rows;
    this->columns = columns;

    matrix = new Vector * [rows];
    for(int i = 0; i < rows; i++)
    {
        matrix[i] = new Vector(columns, initiliazing_value);
    }
}

Matrix::Matrix(Matrix const &matrix_to_copy_from)
{
    this->rows = matrix_to_copy_from.get_rows();
    this->columns = matrix_to_copy_from.get_columns();

    matrix = new Vector * [rows];
    for(int i = 0; i < rows; i++)
    {
        matrix[i] = new Vector(columns, 0);
    }

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            (*matrix[i])[j] = (*matrix_to_copy_from.matrix[i])[j];
        }
    }
}

Matrix::~Matrix()
{
    for(int i = 0; i < rows; i++)
    {
        delete matrix[i];
    }

    delete[] matrix;
}

Vector & Matrix::operator[] (int row) const
{
    return *(this->matrix[row]);
}

std::ostream& operator<< (std::ostream& out, const Matrix& matrix)
{
    for(int i = 0; i < matrix.rows; i++)
    {
        for(int j = 0; j < matrix.columns; j++)
        {
            double value = matrix[i][j];
            out<<std::setprecision(4)<<value<<" ";
        }

        out<<"\n";
    }

    return out;
}

Matrix Matrix::operator+ (const Matrix& right_matrix) const
{
    if(this->rows != right_matrix.rows || this->columns != right_matrix.columns)
    {
        throw std::exception();
    }

    Matrix result_matrix = Matrix(*this);

    for(int i = 0; i < this->rows; i++)
    {
        for(int j = 0; j < this->columns; j++)
        {
            result_matrix[i][j] += right_matrix[i][j];
        }
    }

#if defined(PRINT_MATRIX_OPERATIONS)
    std::cout<<"\n"<<*this<<"+\n"<<right_matrix<<"=\n"<<result_matrix<<"\n";
#endif

    return result_matrix;
}

Matrix Matrix::operator-(const Matrix& right_matrix) const
{
    if(this->rows != right_matrix.rows || this->columns != right_matrix.columns)
    {
        throw std::exception();
    }

    Matrix result_matrix = Matrix(*this);

    for(int i = 0; i < this->rows; i++)
    {
        for(int j = 0; j < this->columns; j++)
        {
            result_matrix[i][j] -= right_matrix[i][j];
        }
    }

#if defined(PRINT_MATRIX_OPERATIONS)
    std::cout<<"\n"<<*this<<"-\n"<<right_matrix<<"=\n"<<result_matrix<<"\n";
#endif

    return result_matrix;
}

Matrix Matrix::operator+ (double scalar) const
{
    Matrix result_matrix = Matrix(*this);

    for(int i = 0; i < this->rows; i++)
    {
        for(int j = 0; j < this->columns; j++)
        {
            result_matrix[i][j] += scalar;
        }
    }

#if defined(PRINT_MATRIX_OPERATIONS)
    std::cout<<"\n"<<*this<<"+\n"<<scalar<<"=\n"<<result_matrix<<"\n";
#endif

    return result_matrix;
}

Matrix Matrix::operator- (double scalar) const
{
    Matrix result_matrix = Matrix(*this);

    for(int i = 0; i < this->rows; i++)
    {
        for(int j = 0; j < this->columns; j++)
        {
            result_matrix[i][j] -= scalar;
        }
    }

#if defined(PRINT_MATRIX_OPERATIONS)
    std::cout<<"\n"<<*this<<"-\n"<<scalar<<"=\n"<<result_matrix<<"\n";
#endif

    return result_matrix;
}

Matrix::Matrix(const Vector &vector_to_copy_from)
{
    this->rows = 1;
    this->columns = vector_to_copy_from.length;

    this->matrix = new Vector * [1];

    for(int i = 0; i < vector_to_copy_from.length; i++)
    {
        (*this)[0][i] = vector_to_copy_from[i];
    }
}

Matrix Matrix::operator* (const Matrix &matrix_B) const
{
    if(this->columns != matrix_B.rows)
    {
        throw std::exception();
    }

    Matrix result_matrix = Matrix(this->rows, matrix_B.columns, 0);

    for(int row_A = 0; row_A < this->rows; row_A++)
    {
        for(int column_B = 0; column_B < matrix_B.columns; column_B++)
        {
            for(int i = 0; i < this->columns; i++)
            {
                result_matrix[row_A][column_B] += (*this)[row_A][i] * matrix_B[i][column_B];
            }
        }
    }

#if defined(PRINT_MATRIX_OPERATIONS)
    std::cout<<"\n"<<*this<<"*\n"<<matrix_B<<"=\n"<<result_matrix<<"\n";
#endif

    return result_matrix;
}

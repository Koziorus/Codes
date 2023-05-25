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

Vector & Matrix::operator[](int row) const
{
    return *(this->matrix[row]);
}

void Matrix::print()
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            double value = (*this)[i][j];
            std::cout<<std::setprecision(4)<<value<<" ";
        }

        std::cout<<"\n";
    }
}

Matrix Matrix::operator+(const Matrix& right_matrix) const
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

    return result_matrix;
}

Matrix Matrix::operator+(double scalar) const
{
    Matrix result_matrix = Matrix(*this);

    for(int i = 0; i < this->rows; i++)
    {
        for(int j = 0; j < this->columns; j++)
        {
            result_matrix[i][j] += scalar;
        }
    }

    return result_matrix;
}

Matrix Matrix::operator-(double scalar) const
{
    Matrix result_matrix = Matrix(*this);

    for(int i = 0; i < this->rows; i++)
    {
        for(int j = 0; j < this->columns; j++)
        {
            result_matrix[i][j] -= scalar;
        }
    }

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

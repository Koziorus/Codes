//
// Created by Tomek on 25.05.2023.
//

#include <iomanip>
#include "matrix.h"

Matrix::Matrix(int rows, int columns, double initiliazing_value)
{
    this->rows = rows;
    this->columns = columns;

    matrix = new Vector* [rows];
    for(int i = 0; i < rows; i++)
    {
        matrix[i] = new Vector(columns, initiliazing_value);
    }
}

Matrix::Matrix(Matrix const &matrix_to_copy_from)
{
    this->rows = matrix_to_copy_from.rows;
    this->columns = matrix_to_copy_from.columns;

    matrix = new Vector* [rows];
    for(int i = 0; i < rows; i++)
    {
        matrix[i] = new Vector(columns, 0.0);
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

Vector &Matrix::operator[](int row) const
{
    return *(this->matrix[row]);
}

std::ostream &operator<<(std::ostream &out, const Matrix &matrix)
{
    for(int i = 0; i < matrix.rows; i++)
    {
        for(int j = 0; j < matrix.columns; j++)
        {
            double value = matrix[i][j];
            out << std::setprecision(4) << value << " ";
        }

        out << "\n";
    }

    return out;
}

Matrix Matrix::operator+(const Matrix &right_matrix) const
{
    if(this->rows != right_matrix.rows || this->columns != right_matrix.columns)
    {
        throw std::runtime_error("Unmatched dimensions!");
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
    std::cout << "\n" << *this << "+\n" << right_matrix << "=\n" << result_matrix << "\n";
#endif

    return result_matrix;
}

Matrix Matrix::operator-(const Matrix &right_matrix) const
{
    if(this->rows != right_matrix.rows || this->columns != right_matrix.columns)
    {
        throw std::runtime_error("Unmatched dimensions!");
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
    std::cout << "\n" << *this << "-\n" << right_matrix << "=\n" << result_matrix << "\n";
#endif

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

#if defined(PRINT_MATRIX_OPERATIONS)
    std::cout << "\n" << *this << "+\n" << scalar << "=\n" << result_matrix << "\n";
#endif

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

#if defined(PRINT_MATRIX_OPERATIONS)
    std::cout << "\n" << *this << "-\n" << scalar << "=\n" << result_matrix << "\n";
#endif

    return result_matrix;
}

Matrix::Matrix(const Vector &vector_to_copy_from) : Matrix(1, vector_to_copy_from.length, 0.0)
{
    for(int i = 0; i < vector_to_copy_from.length; i++)
    {
        (*this)[0][i] = vector_to_copy_from[i];
    }
}

Matrix Matrix::operator*(const Matrix &matrix_B) const
{
    if(this->columns != matrix_B.rows)
    {
        throw std::runtime_error("Unmatched dimensions!");
    }

    Matrix result_matrix = Matrix(this->rows, matrix_B.columns, 0.0);

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
    std::cout << "\n" << *this << "*\n" << matrix_B << "=\n" << result_matrix << "\n";
#endif

    return result_matrix;
}

Matrix Matrix::identity(int size)
{
    Matrix result_matrix = Matrix(size, size, 0.0);

    for(int i = 0; i < size; i++)
    {
        result_matrix[i][i] = 1;
    }

    return result_matrix;
}

Matrix Matrix::band(int size, double* values, int values_length)
{
    Matrix band_matrix = Matrix(size, size, 0.0);

    for(int row = 0; row < size; row++)
    {
        for(int column = 0; column < size; column++)
        {
            int index = values_length / 2 + column - row;

            if(index >= 0 && index < values_length)
            {
                band_matrix[row][column] = values[index];
            }
        }
    }

    return band_matrix;
}

Matrix::Matrix(int rows, int columns, double (* func)(int)) : Matrix(rows, columns, 0.0)
{
    for(int row = 0; row < rows; row++)
    {
        for(int column = 0; column < columns; column++)
        {
            (*this)[row][column] = func(column);
        }
    }
}

bool Matrix::is_upper_triangular() const
{
    for(int row = 1; row < this->rows; row++)
    {
        for(int column = 0; column < row; column++)
        {
            if((*this)[row][column] != 0)
            {
                return false;
            }
        }
    }

    return true;
}

bool Matrix::is_lower_triangular() const
{
    for(int row = 0; row < this->rows; row++)
    {
        for(int column = row + 1; column < this->columns; column++)
        {
            if((*this)[row][column] != 0)
            {
                return false;
            }
        }
    }

    return true;
}

Vector Matrix::operator%(const Vector &vector_b) const
{
    if(!(this->rows > 0 && this->columns > 0 && this->rows == this->columns))
    {
        throw std::runtime_error("not a square matrix");
    }

    const Matrix* A = this;
    Vector x;

    if(this->is_lower_triangular())
    {
        x = Vector(this->rows, 0.0);

        int n = this->rows;

        x[0] = vector_b[0] / (*A)[0][0];

        for(int i = 1; i < n; i++)
        {
            double temp = 0;

            for(int k = 0; k < i; k++)
            {
                temp += x[k] * (*A)[i][k];
            }

            x[i] = (vector_b[i] - temp) / (*A)[i][i];
        }
    }
    else if(this->is_upper_triangular())
    {
        x = Vector(this->rows, 0.0);

        int n = this->rows;

        x[n - 1] = vector_b[n - 1] / (*A)[n - 1][n - 1];

        for(int i = (n - 1) - 1; i >= 0; i--)
        {
            double temp = 0;

            for(int k = i + 1; k < n; k++)
            {
                temp += x[k] * (*A)[i][k];
            }

            x[i] = (vector_b[i] - temp) / (*A)[i][i];
        }
    }
    else
    {
        throw std::runtime_error("not a triangular matrix");
    }

    return x;
}

Matrix::Matrix(int rows, int columns, double** arr) : Matrix(rows, columns, 0.0)
{
    for(int row = 0; row < rows; row++)
    {
        for(int column = 0; column < columns; column++)
        {
            (*this)[row][column] = arr[row][column];
        }
    }
}

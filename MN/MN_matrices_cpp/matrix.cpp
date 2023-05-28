#include <iomanip>
#include <valarray>
#include "matrix.h"

// TODO: do checks in every method/operator if the dimensions check

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

Matrix::Matrix(Matrix const &matrix_to_copy_from) : Matrix(matrix_to_copy_from.rows, matrix_to_copy_from.columns, 0.0)
{
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

Matrix Matrix::operator%(const Matrix &matrix_B) const
{
    const Matrix* matrix_A = this;

    if(matrix_A->rows != matrix_B.rows)
    {
        throw std::runtime_error("Unmatched dimensions!");
    }

    Matrix X = Matrix(matrix_A->rows, matrix_B.columns, 0.0);

    int n = matrix_A->rows;

    if(this->is_lower_triangular())
    {
        for(int column = 0; column < matrix_B.columns; ++column)
        {
            X[0][column] = matrix_B[0][column] / (*matrix_A)[0][0];

            for(int i = 1; i < n; i++)
            {
                double temp = 0;

                for(int k = 0; k < i; k++)
                {
                    temp += X[k][column] * (*matrix_A)[i][k];
                }

                X[i][column] = (matrix_B[i][column] - temp) / (*matrix_A)[i][i];
            }
        }
    }
    else if(this->is_upper_triangular())
    {
        for(int column = 0; column < matrix_B.columns; ++column)
        {
            X[n - 1][column] = matrix_B[n - 1][column] / (*matrix_A)[n - 1][n - 1];

            for(int i = (n - 1) - 1; i >= 0; i--)
            {
                double temp = 0;

                for(int k = i + 1; k < n; k++)
                {
                    temp += X[k][column] * (*matrix_A)[i][k];
                }

                X[i][column] = (matrix_B[i][column] - temp) / (*matrix_A)[i][i];
            }
        }
    }
    else
    {
        throw std::runtime_error("not a triangular matrix");
    }

#if defined(PRINT_MATRIX_OPERATIONS)
    std::cout << "\n" << *this << "%\n" << matrix_B << "=\n" << X << "\n";
#endif

    return X;
}

Matrix Matrix::operator-() const
{
    Matrix result_matrix = Matrix(*this);

    for(int row = 0; row < this->rows; ++row)
    {
        for(int column = 0; column < this->columns; ++column)
        {
            result_matrix[row][column] *= (result_matrix[row][column] == 0.0 ? 1 : -1);
        }
    }

#if defined(PRINT_MATRIX_OPERATIONS)
    std::cout << "\n-\n" << *this << "=\n" << result_matrix << "\n";
#endif

    return result_matrix;
}

Matrix Matrix::jacobi_solve(const Matrix &matrix_A, const Matrix &vector_b, double max_error)
{
    if(vector_b.columns != 1)
    {
        throw std::runtime_error("not a column vector!");
    }
}

Matrix Matrix::get_lower_triangular(const Matrix &matrix, bool with_diagonal)
{
    if(!matrix.is_square())
    {
        throw std::runtime_error("not a square matrix!");
    }

    Matrix lower_triangular_matrix = Matrix(matrix.rows, matrix.columns, 0.0);

    int offset = (with_diagonal ? 0 : 1);

    for(int row = 0 + offset; row < lower_triangular_matrix.rows; ++row)
    {
        for(int column = 0; column < row + 1 - offset; ++column)
        {
            lower_triangular_matrix[row][column] = matrix[row][column];
        }
    }

    return lower_triangular_matrix;
}

Matrix Matrix::get_upper_triangular(const Matrix &matrix, bool with_diagonal)
{
    if(!matrix.is_square())
    {
        throw std::runtime_error("not a square matrix!");
    }

    Matrix upper_triangular_matrix = Matrix(matrix.rows, matrix.columns, 0.0);

    int offset = (with_diagonal ? 0 : 1);

    for(int row = 0; row < upper_triangular_matrix.rows; ++row)
    {
        for(int column = row + offset; column < upper_triangular_matrix.columns; ++column)
        {
            upper_triangular_matrix[row][column] = matrix[row][column];
        }
    }

    return upper_triangular_matrix;
}

double Matrix::norm(const Matrix &vector)
{
    if(vector.columns != 1)
    {
        throw std::runtime_error("not a column vector!");
    }

    double norm = 0;

    for(int element = 0; element < vector.rows; ++element)
    {
        norm += vector[element][0] * vector[element][0];
    }

    norm = sqrt(norm);

    return norm;
}

Matrix Matrix::operator!() const
{
    Matrix transposed_matrix = Matrix(this->columns, this->rows, 0.0);

    for(int row = 0; row < transposed_matrix.rows; ++row)
    {
        for(int column = 0; column < transposed_matrix.columns; ++column)
        {
            transposed_matrix[row][column] = (*this)[column][row];
        }
    }

    return transposed_matrix;
}

bool Matrix::is_square() const
{
    return this->rows == this->columns;
}

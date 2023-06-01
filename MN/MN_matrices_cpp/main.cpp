#include <iostream>
#include <limits>
#include <math.h>

#include "matrix.h"

#define CONVERT_STATIC_TO_DYNAMIC(arr, rows, cols, dynamic_arr) \
    dynamic_arr = new double*[rows]; \
    for (int i = 0; i < rows; i++) { \
        dynamic_arr[i] = new double[cols]; \
        for (int j = 0; j < cols; j++) { \
            dynamic_arr[i][j] = arr[i][j]; \
        } \
    }

#define DELETE_DYNAMIC(dynamic_arr, rows) \
    for (int i = 0; i < rows; i++) { \
        delete[] dynamic_arr[i]; \
    } \
    delete[] dynamic_arr;

double sinf(int n)
{
    int f = 8; // 188682
    return sin(n * (f + 1));
}

int main()
{
//    double values[] = {3, 2, 1, 2, 3};
//
//    Matrix band_matrix = Matrix::band(10, values, 5);
//
//    std::cout<<band_matrix<<"\n";
//
//    Matrix b = Matrix(1, 10, sinf);
//
//    std::cout<<b;

    double** A_out;
    double A_in[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    CONVERT_STATIC_TO_DYNAMIC(A_in, 4, 4, A_out);
    Matrix D = Matrix(4, 4, A_out);

    //setbuf(stdout, 0);

    double** LU_out;
    double LU_in[4][4] = {{0, -0.4250, -0.2833, -0.4250}, {0, 0, -0.2833, -0.4250}, {-0.4250, -0.4250, 0, 0}, {-0.4250, 0, -0.2833, 0}};
    CONVERT_STATIC_TO_DYNAMIC(LU_in, 4, 4, LU_out);
    Matrix LU = Matrix(4, 4, LU_out);

    double** b_out;
    double b_in[4][1] = {{0.0375}, {0.0375}, {0.0375}, {0.0375}};
    CONVERT_STATIC_TO_DYNAMIC(b_in, 4, 1, b_out);
    Matrix b = Matrix(4, 1, b_out);

    Matrix X = (-D) % LU;
    Matrix Y = D % b;

    //std::cout << Y;

    std::cout<<Matrix::identity(10);

    DELETE_DYNAMIC(A_out, 4);
    DELETE_DYNAMIC(LU_out, 4);

    return 0;
}

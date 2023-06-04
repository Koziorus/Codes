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
    setbuf(stdout, 0);
//    double values[] = {3, 2, 1, 2, 3};
//
//    Matrix band_matrix = Matrix::band(10, values, 5);
//
//    std::cout<<band_matrix<<"\n";
//
//    Matrix b = Matrix(1, 10, sinf);
//
//    std::cout<<b;

    double** p_out;
    double p_in[4][4] = {{1.0000,-0.2833,-0.2833,-0.4250},{-0.4250,1.0000,-0.2833,-0.4250},{0,-0.2833,1.0000,0},{-0.4250,-0.2833,-0.2833,1.0000}};
    CONVERT_STATIC_TO_DYNAMIC(p_in, 4, 4, p_out);
    Matrix P = Matrix(4, 4, p_out);

    double** q_out;
    double q_in[4][1] = {{0.0375}, {0.0375}, {0.0375}, {0.0375}};
    CONVERT_STATIC_TO_DYNAMIC(q_in, 4, 1, q_out);
    Matrix q = Matrix(4, 1, q_out);

    std::cout << P << "\n" << q << "\n";

    std::cout << Matrix::gauss_solve(P, q, 1e-2);
    std::cout<<"\n";
    std::cout << Matrix::jacobi_solve(P, q, 1e-2);

    DELETE_DYNAMIC(p_out, 4);
    DELETE_DYNAMIC(q_out, 4);

    return 0;
}

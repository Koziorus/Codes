#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>

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

void out_array(double* arr, int n, std::string file_path)
{
    std::ofstream data_file(file_path);
    for(int i = 0; i < n; ++i)
    {
        data_file << arr[i] << "\n";
    }
    data_file.close();
}

void plot(std::string parameters)
{
    std::string command = "matlab -nodesktop -nosplash -noFigureWindows -r ";
    std::string system_command = command + "\"" + parameters + "quit;" + "\"";

    system(system_command.c_str());
}

void problem_A_B()
{
    int N = 982;
    double e = 6; // 188682
    double values[] = {-1, -1, 5 + e, -1, -1};

    Matrix A = Matrix::band(N, values, sizeof(values) / sizeof(values[0]));
    Matrix b = Matrix(N, 1, sinf);

    double max_error = 1e-9;

    clock_t jacobi_timer = clock();
    Matrix::jacobi_solve(A, b, max_error);
    double jacobi_time = 1000.0 * (clock() - jacobi_timer) / CLOCKS_PER_SEC;
    std::cout << "Jacobi time: " << jacobi_time << "ms\n";

    clock_t gauss_timer = clock();
    Matrix::gauss_solve(A, b, max_error);
    double gauss_time = 1000.0 * (clock() - gauss_timer) / CLOCKS_PER_SEC;
    std::cout << "Gauss time: " << gauss_time << "ms\n";

    double Y[2] = {jacobi_time, gauss_time};

    out_array(Y, 2, "times.txt");

    plot("\
\
Y = load('times.txt');\
plot(Y);\
xlabel('iksy');\
title('titl');\
saveas(gcf, 'plot.png');\
\
         ");
}

void problem_C()
{
    int N = 982;
    double values[] = {-1, -1, 3, -1, -1};

    Matrix A = Matrix::band(N, values, sizeof(values) / sizeof(values[0]));
    Matrix b = Matrix(N, 1, sinf);

    double max_error = 1e-9;

    clock_t jacobi_timer = clock();
    Matrix::jacobi_solve(A, b, max_error);
    double jacobi_time = 1000.0 * (clock() - jacobi_timer) / CLOCKS_PER_SEC;
    std::cout << "Jacobi time: " << jacobi_time << "ms\n";

    clock_t gauss_timer = clock();
    Matrix::gauss_solve(A, b, max_error);
    double gauss_time = 1000.0 * (clock() - gauss_timer) / CLOCKS_PER_SEC;
    std::cout << "Gauss time: " << gauss_time << "ms\n";
}

void problem_D()
{
    int N = 982;
    double values[] = {-1, -1, 3, -1, -1};

    Matrix A = Matrix::band(N, values, sizeof(values) / sizeof(values[0]));
    Matrix b = Matrix(N, 1, sinf);

    clock_t LU_timer = clock();
    Matrix x = Matrix::LU_factorization_solve(A, b);
    double LU_time = 1000.0 * (clock() - LU_timer) / CLOCKS_PER_SEC;
    std::cout << "LU time: " << LU_time << "ms\n";

    std::cout << "LU residual norm: " << Matrix::norm(A * x - b) << "\n";
}

void problem_E()
{
    int N[5] = {100, 500, 1000, 2000, 3000};
    double e = 6; // 188682
    double values[] = {-1, -1, 5 + e, -1, -1};

    double max_error = 1e-9;
    for(int i = 0; i < sizeof(N) / sizeof(N[0]); ++i)
    {
        Matrix A = Matrix::band(N[i], values, sizeof(values) / sizeof(values[0]));
        Matrix b = Matrix(N[i], 1, sinf);

        std::cout << "N: " << N[i] << "\n";
        clock_t jacobi_timer = clock();
        Matrix::jacobi_solve(A, b, max_error);
        double jacobi_time = 1000.0 * (clock() - jacobi_timer) / CLOCKS_PER_SEC;
        std::cout << "\tJacobi time: " << jacobi_time << "ms\n";

        clock_t gauss_timer = clock();
        Matrix::gauss_solve(A, b, max_error);
        double gauss_time = 1000.0 * (clock() - gauss_timer) / CLOCKS_PER_SEC;
        std::cout << "\tGauss time: " << gauss_time << "ms\n";
    }
}



int main()
{
    problem_A_B();

    //problem_C();

    //problem_D();

    //problem_E();

//    double** p_out;
//    double p_in[4][4] = {{1.0000, -0.2833, -0.2833, -0.4250}, {-0.4250, 1.0000, -0.2833, -0.4250}, {0, -0.2833, 1.0000, 0}, {-0.4250, -0.2833, -0.2833, 1.0000}};
//    CONVERT_STATIC_TO_DYNAMIC(p_in, 4, 4, p_out);
//    Matrix P = Matrix(4, 4, p_out);
//
//    double** q_out;
//    double q_in[4][1] = {{0.0375}, {0.0375}, {0.0375}, {0.0375}};
//    CONVERT_STATIC_TO_DYNAMIC(q_in, 4, 1, q_out);
//    Matrix q = Matrix(4, 1, q_out);
//
//    DELETE_DYNAMIC(p_out, 4);
//    DELETE_DYNAMIC(q_out, 4);

    return 0;
}

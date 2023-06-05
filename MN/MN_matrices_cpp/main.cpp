#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>

#include "matrix.h"
#include "plot_helper.h"

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

void problem_A_B()
{
    std::cout << "Problem A,B\n\n";
    int N = 982;
    double e = 6; // 188682
    double values[] = {-1, -1, 5 + e, -1, -1};

    Matrix A = Matrix::band(N, values, sizeof(values) / sizeof(values[0]));

    Matrix b = Matrix(N, 1, sinf);

    double max_error = 1e-9;

    clock_t jacobi_timer = clock();
    Matrix::jacobi_solve(A, b, max_error, "Bjacobi.txt");
    double jacobi_time = 1000.0 * (clock() - jacobi_timer) / CLOCKS_PER_SEC;
    std::cout << "Jacobi time: " << jacobi_time << "ms\n";

    clock_t gauss_timer = clock();
    Matrix::gauss_solve(A, b, max_error, "Bgauss.txt");
    double gauss_time = 1000.0 * (clock() - gauss_timer) / CLOCKS_PER_SEC;
    std::cout << "Gauss time: " << gauss_time << "ms\n";

//    double Y[2] = {jacobi_time, gauss_time};
//    out_array(Y, 2, "times.txt");
//    plot("\
//Y = load('times.txt');\
//plot(Y);\
//xlabel('iksy');\
//title('titl');\
//saveas(gcf, 'plot.png');\
//         ");

    std::cout << "\n";
}

void problem_C()
{
    std::cout << "Problem C\n\n";
    int N = 982;
    double values[] = {-1, -1, 3, -1, -1};

    Matrix A = Matrix::band(N, values, sizeof(values) / sizeof(values[0]));
    Matrix b = Matrix(N, 1, sinf);

    double max_error = 1e-9;

    bool is_error = false;

    try
    {
        clock_t jacobi_timer = clock();
        Matrix::jacobi_solve(A, b, max_error, "Cjacobi.txt");
        double jacobi_time = 1000.0 * (clock() - jacobi_timer) / CLOCKS_PER_SEC;
        std::cout << "Jacobi time: " << jacobi_time << "ms\n";
    }
    catch(std::runtime_error &error)
    {
        is_error = true;
        std::cout << "Jacobi: " << error.what() << "\n";
    }

    try
    {
        clock_t gauss_timer = clock();
        Matrix::gauss_solve(A, b, max_error, "Cgauss.txt");
        double gauss_time = 1000.0 * (clock() - gauss_timer) / CLOCKS_PER_SEC;
        std::cout << "Gauss time: " << gauss_time << "ms\n";
    }
    catch(std::runtime_error &error)
    {
        is_error = true;
        std::cout << "Gauss: " << error.what() << "\n";
    }

    std::cout << "\n";

    if(is_error)
    {
        throw std::runtime_error("methods are not converging");
    }
}

void problem_D()
{
    std::cout << "Problem D\n\n";
    int N = 982;
    double values[] = {-1, -1, 3, -1, -1};

    Matrix A = Matrix::band(N, values, sizeof(values) / sizeof(values[0]));
    Matrix b = Matrix(N, 1, sinf);

    clock_t LU_timer = clock();
    Matrix x = Matrix::LU_factorization_solve(A, b);
    double LU_time = 1000.0 * (clock() - LU_timer) / CLOCKS_PER_SEC;
    std::cout << "LU time: " << LU_time << "ms\n";

    std::cout << "LU residual norm: " << Matrix::norm(A * x - b) << "\n";

    std::cout << "\n";
}

void problem_E()
{
    std::cout << "Problem E\n\n";
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
        Matrix::jacobi_solve(A, b, max_error, "Ejacobi" + std::to_string(N[i]) + ".txt");
        double jacobi_time = 1000.0 * (clock() - jacobi_timer) / CLOCKS_PER_SEC;
        std::cout << "\tJacobi time: " << jacobi_time << "ms\n";

        clock_t gauss_timer = clock();
        Matrix::gauss_solve(A, b, max_error, "Egauss" + std::to_string(N[i]) + ".txt");
        double gauss_time = 1000.0 * (clock() - gauss_timer) / CLOCKS_PER_SEC;
        std::cout << "\tGauss time: " << gauss_time << "ms\n";

        clock_t LU_timer = clock();
        Matrix x = Matrix::LU_factorization_solve(A, b);
        double LU_time = 1000.0 * (clock() - LU_timer) / CLOCKS_PER_SEC;
        std::cout << "\tLU time: " << LU_time << "ms\n";
    }

    std::cout << "\n";
}

int main()
{
    problem_A_B();

    //problem_C();

    problem_D();

    problem_E();

    return 0;
}

/*
 * G = [48.7266
10.8165
2.40181
0.533351
0.11844
0.0263022
0.00584103
0.00129715
0.000288065
6.39726e-05
1.42069e-05
3.15503e-06
7.00663e-07
1.55602e-07
3.45559e-08
7.67413e-09
1.70426e-09
3.78481e-10];
J = [79.7107
28.9598
10.5244
3.82512
1.39034
0.505374
0.183705
0.066779
0.0242755
0.00882479
0.00320809
0.00116626
0.000423983
0.000154136
5.6036e-05
2.03719e-05
7.40627e-06
2.69259e-06
9.78912e-07
3.55893e-07
1.29389e-07
4.70412e-08
1.71025e-08
6.21791e-09
2.26063e-09
8.21897e-10];

figure
semilogy(G)
hold on
semilogy(J)
xlabel('Liczba iteracji')
ylabel('Wartość normy residuum')
legend("Gauss", "Jacobi")
title('Norma residuum')
hold off

N = [100, 500, 1000, 2000, 3000];
G_time = [11, 636, 5039, 44012, 160587];
J_time = [12, 637, 5030, 43987, 161015];
LU_time = [2, 319, 3100, 29838, 114126]

semilogy(N, G_time)
hold on
semilogy(N, J_time)
semilogy(N, LU_time)
xlabel('Wielkość macierzy')
ylabel('Czas wykonania [ms]')
legend("Gauss", "Jacobi", "LU")
hold off
title('Czas wykonania')

plot(N, J_time-G_time)
title('Różnica czasu wykonania Jacobi - Gauss')
xlabel('Wielkość macierzy')
ylabel('Różnica czasu [ms]')
axis([100 3000 -100 500])

G_iter = [17, 18, 18, 18, 18];
J_iter = [25, 26, 26, 27, 27];

plot(N, G_iter)
hold on
plot(N, J_iter)
legend("Gauss", "Jacobi")
hold off
xlabel('Wielkość macierzy')
ylabel('Liczba iteracji')
title('Liczba iteracji')

bar([18 26]);
title('Liczba iteracji (norma residuum = 10^{-9})')
ylabel('Liczba iteracji')
xticklabels({'Jacobi', 'Gauss'})

bar([4240 4212]);
title('Czas wykonania (norma residuum = 10^{-9})')
ylabel('Czas wykonania [ms]')
xticklabels({'Jacobi', 'Gauss'})

 */
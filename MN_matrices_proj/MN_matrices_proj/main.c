#include <stdio.h>
#include <math.h>

#define FULL -1

#define RAND_MAX 0x7fff

typedef struct Matrix
{
	double** mat;
	int n, m;
} Matrix;

void err(const char* message)
{
	fprintf(stderr, "!--- %s ---!\n", message);
}

Matrix* m_new(int n, int m, double value)
{
	Matrix* M = malloc(sizeof(Matrix));
	M->n = n;
	M->m = m;
	M->mat = malloc(n * sizeof(double*));
	if (M->mat == NULL)
	{
		err("Couldn't allocate a matrix");
	}

	for (int i = 0; i < n; i++)
	{
		M->mat[i] = malloc(m * sizeof(double));
	}


	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			M->mat[i][j] = value;
		}
	}

	return M;
}

Matrix* m_add(const Matrix* A, const Matrix* B)
{
	if (A->n != B->n || A->m != B->m)
	{
		err("Unmatched matrix dimension");
		return NULL;
	}

	Matrix* C = m_new(A->n, A->m, 0);

	for (int i = 0; i < A->n; i++)
	{
		for (int j = 0; j < A->m; j++)
		{
			C->mat[i][j] = A->mat[i][j] + B->mat[i][j];
		}
	}

	return C;
}

Matrix* m_sub(const Matrix* A, const Matrix* B)
{
	if (A->n != B->n || A->m != B->m)
	{
		err("Unmatched matrix dimension");
		return NULL;
	}

	Matrix* C = m_new(A->n, A->m, 0);

	for (int i = 0; i < A->n; i++)
	{
		for (int j = 0; j < A->m; j++)
		{
			C->mat[i][j] = A->mat[i][j] - B->mat[i][j];
		}
	}

	return C;
}

// maximum value from a matrix
double m_max(const Matrix* M)
{
	double max = 0.0;

	for (int i = 0; i < M->n; i++)
	{
		for (int j = 0; j < M->m; j++)
		{
			if (M->mat[i][j] > max)
			{
				max = M->mat[i][j];
			}
		}
	}

	return max;
}

// number of digits of an integer number
int integer_len(int x)
{
	int len;
	if (x == 0)
	{
		len = 1;
	}
	else
	{
		len = floor(log10(abs(x))) + 1;
	}

	return len;
}

void m_print(const Matrix* M, int precision)
{
	double max = m_max(M);

	int spaces = 3;

	int len = integer_len((int) max) + 1 + precision + spaces; // maksymalna dlugosc liczby wraz z kropka i przerwa

	printf("%dx%d\n", M->n, M->m);

	for (int i = 0; i < M->n; i++)
	{
		for (int j = 0; j < M->m; j++)
		{
			printf("%*.*f", len, precision, M->mat[i][j]);
		}
		printf("\n");
	}

	printf("\n");
}

void m_copy(Matrix* A, const Matrix* B)
{
	// copying from B to A

	if (A->n != B->n || A->m != B->m)
	{
		err("Unmatched matrix dimensions");
		return NULL;
	}

	for (int i = 0; i < A->n; i++)
	{
		for (int j = 0; j < A->m; j++)
		{
			A->mat[i][j] = B->mat[i][j];
		}
	}
}

// matrix with random values <0.0, limit> (d -> double)
Matrix* m_d_random(int n, int m, double limit)
{
	Matrix* R = m_new(n, m, 0);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			R->mat[i][j] = ((double) rand() / RAND_MAX) * limit;
		}
	}

	return R;
}

// element-wise multiply
Matrix* m_ew_mult(Matrix* A, double scalar)
{
	for (int i = 0; i < A->n; i++)
	{
		for (int j = 0; j < A->m; j++)
		{
			A->mat[i][j] *= scalar;
		}
	}
}

// element-wise add
Matrix* m_ew_add(Matrix* A, double scalar)
{
	for (int i = 0; i < A->n; i++)
	{
		for (int j = 0; j < A->m; j++)
		{
			A->mat[i][j] += scalar;
		}
	}
}

Matrix* m_get(const Matrix* A, int a, int b)
{
	Matrix* M;
	if (a == FULL)
	{
		if (b == FULL)
		{
			// copy of a matrix
			M = m_new(A->n, A->m, 0);
			m_copy(M, A);
		}
		else
		{
			// column matrix
			M = m_new(A->n, 1, 0.0);
			for (int i = 0; i < A->n; i++)
			{
				M->mat[i][0] = A->mat[i][b];
			}
		}
	}
	else if (b == FULL)
	{
		M = m_new(1, A->m, 0);
		for (int j = 0; j < A->n; j++)
		{
			M->mat[0][j] = A->mat[a][j];
		}
	}
	else
	{
		M = m_new(1, 1, 0);
		M->mat[0][0] = A->mat[a][b];
	}

	return M;
}

double m_to_scalar(const Matrix* M)
{
	if (!(M->n == 1 && M->m == 1))
	{
		err("Can't convert matrix to a scalar");
		return NAN;
	}

	return M->mat[0][0];
}

Matrix* m_identity(int n)
{
	Matrix* M = m_new(n, n, 0.0);
	for (int i = 0; i < n; i++)
	{
		M->mat[i][i] = 1.0;
	}

	return M;
}

// n x n diaognal 
Matrix* m_eye(int n, double diag_value)
{
	Matrix* M = m_new(n, n, 0.0);

	for (int i = 0; i < n; i++)
	{
		M->mat[i][i] = diag_value;
	}

	return M;
}

Matrix* m_mult(Matrix* A, Matrix* B)
{
	Matrix* C = m_new(A->n, B->m, 0);

	for (int i = 0; i < A->n; i++)
	{
		for (int j = 0; j < B->m; j++)
		{
			for (int k = 0; k < A->m; k++)
			{
				C->mat[i][j] += A->mat[i][k] * B->mat[k][j];
			}
		}
	}

	return C;
}

Matrix* m_res(Matrix* A, Matrix* x, Matrix* b)
{
	Matrix* A_times_x = m_mult(A, x);
	Matrix* res = m_sub(A_times_x, b);
	return res;
}

double m_norm(Matrix* x)
{
	double norm = 0;
	for (int i = 0; i < x->n; i++)
	{
		norm += (x->mat[i][0]) * (x->mat[i][0]);
	}

	norm = sqrt(norm);

	return norm;
}

int m_jacobi(Matrix* A, Matrix* b, Matrix* x, double theta, int max_iter) 
{
	int N = A->n;

	double sum;

	Matrix* x_old = m_new(x->n, x->m, 0);

	int k;

	for (k = 0; k < max_iter; k++) 
	{
		m_copy(x_old, x);

		for (int i = 0; i < N; i++) 
		{
			sum = b->mat[i][0];
			for (int j = 0; j < N; j++) 
			{
				if (j != i) {
					sum -= A->mat[i][j] * x_old->mat[j][0];
				}
			}
			x->mat[i][0] = sum / A->mat[i][i];
		}

		if (m_norm(m_res(A, x, b)) < theta)
		{
			break;
		}
	}

	free(x_old);

	return k;
}

int m_gauss_seidel(Matrix* A, Matrix* b, Matrix* x, double theta, int max_iter) 
{
	int N = A->n;

	Matrix* x_old = m_new(x->n, x->m, 0);

	int k;

	for (k = 0; k < max_iter; k++)
	{
		m_copy(x_old, x);

		for (int i = 0; i < N; i++) 
		{
			double sum = 0;
			for (int j = 0; j < N; j++) 
			{
				if (j != i) {
					sum += A->mat[i][j] * x->mat[j][0];
				}
			}
			x->mat[i][0] = (b->mat[i][0] - sum) / A->mat[i][i];
		}

		if (m_norm(m_res(A, x, b)) < theta)
		{
			break;
		}
	}

	return k;
}



Matrix* m_from_array(int n, int m, double* arr) {
	Matrix* M = m_new(n, m, 0);

	for (int i = 0; i < n; i++) 
	{
		for (int j = 0; j < m; j++) 
		{
			M->mat[i][j] +=  *(arr + i * m + j);
		}
	}

	return M;
}

Matrix* m_diags(int n, int m, double* arr, int d)
{
	Matrix* M = m_new(n, m, 0);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			M->mat[i][j] = (d / 2 - i + j >= 0 && d / 2 - i + j < d ? arr[d / 2 - i + j] : 0);
		}
	}

	return M;
}

// TODO:
// m_sum() -> sums by row / column / row and column

int main()
{
	srand(time(NULL));
	//double matrix_a[2][3] = { {1, 2, 3}, {4, 5, 6} };
	//Matrix* A = m_from_array(2, 3, matrix_a[0]);
	
	// -------------------- Zadanie A
	printf("\n-------------------- Zadanie A\n");

	int e = 6; // 1 8 8 6 8 2
	int f = 8;
	double a_A[] = { -1, -1, 5+e, -1, -1 };
	int N = 982; // 1 8 8 6 8 2
	Matrix* A_A = m_diags(N, N, a_A, 5);

	Matrix* b = m_new(N, 1, 0);
	for (int i = 0; i < N; i++)
	{
		b->mat[i][0] = sin(i * (f + 1));
	}

	// -------------------- Zadanie B
	printf("\n-------------------- Zadanie B\n");

	Matrix* x_jacobi_A = m_new(N, 1, 0);
	int jacobi_iterations_A = m_jacobi(A_A, b, x_jacobi_A, 1e-9, 1000);

	Matrix* x_gauss_A = m_new(N, 1, 0);
	int gauss_iterations_A = m_gauss_seidel(A_A, b, x_gauss_A, 1e-9, 1000);
	
	printf("Number of iterations:\n");
	printf("Jacobi: %d\n", jacobi_iterations_A);
	printf("Gauss-Seidel: %d\n", gauss_iterations_A);

	// -------------------- Zadanie C
	printf("\n-------------------- Zadanie C\n");

	double a_C[] = { -1, -1, 3, -1, -1 };

	Matrix* A_C = m_diags(N, N, a_A, 5);

	Matrix* x_jacobi_C = m_new(N, 1, 0);
	int jacobi_iterations_C = m_jacobi(A_A, b, x_jacobi_C, 1e-9, 1000);

	Matrix* x_gauss_C = m_new(N, 1, 0);
	int gauss_iterations_C = m_gauss_seidel(A_A, b, x_gauss_C, 1e-9, 1000);

	printf("Number of iterations:\n");
	printf("Jacobi: %d\n", jacobi_iterations_C);
	printf("Gauss-Seidel: %d\n", gauss_iterations_C);

	return 0;
}
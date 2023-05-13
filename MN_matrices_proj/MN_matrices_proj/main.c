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
Matrix* m_diag(int n, double diag_value)
{
	Matrix* M = m_new(n, n, 0.0);

	for (int i = 0; i < n; i++)
	{
		M->mat[i][j] = diag_value;
	}

	return M;
}

// TODO:
// m_sum() -> sums by row / column / row and column

int main()
{
	srand(time(NULL));
	Matrix* A = m_new(5, 5, 3.0);
	Matrix* B = m_new(5, 5, 4.0);

	Matrix* C = m_add(A, B);

	m_print(C, 2);

	//A->mat[2][3] = 7543.4;
	//B->mat[4][2] = 90.3;

	Matrix* R = m_d_random(5, 5, 10.0);

	m_print(R, 2);

	Matrix* column = m_get(R, FULL, 3);

	m_print(column, 2);

	Matrix* row = m_get(R, 2, FULL);

	m_print(row, 2);

	Matrix* full = m_get(R, FULL, FULL);

	m_print(full, 2);

	Matrix* single = m_get(R, 2, 4);

	m_print(single, 2);

	return 0;
}
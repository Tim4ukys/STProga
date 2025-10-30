#include "matrix.h"
#include <memory.h>
#include <assert.h>
#include <math.h>
#include <Windows.h> // ZeroMemory & FillMemory

#define GET_EL(matrix, i, j) (matrix).A[(matrix).cols * (i) + (j)]

static int cmp_double(double a, double b) {
    const double eps = 1e-7;

    if (!a && !b) {
        return 0;
    }
    if (fabs(a - b) < eps)
        return 0;

    return fabs(a - b) / fabs(a + b) < eps ? 0 : (b > a ? 1 : -1);
}

static void fill_double(void* dest, double value, size_t sz) {
    for (size_t i = 0; i < sz; ++i) {
        memcpy((double*)dest + i, &value, sizeof(double));
    }
}

Matrix create_empty() {
    Matrix t;
    ZeroMemory(&t, sizeof(Matrix));
    return t;
}
void destroy(Matrix* mat) {
    if (!mat)
        return;
    if (mat->A)
        free(mat->A);
    ZeroMemory(mat, sizeof(Matrix));
}

Matrix create(size_t rows, size_t cols) {
    if (!rows || !cols)
        return create_empty();
    Matrix m = {malloc(sizeof(double) * cols * rows), cols, rows};
    assert(m.A && "couldn't allocate memory");
    ZeroMemory(m.A, sizeof(double) * cols * rows);
    return m;
}

Matrix create_copy(Matrix* mat) {
    if (!is_valid(mat))
        return create_empty();
    Matrix r = create(mat->rows, mat->cols);
    memcpy(r.A, mat->A, sizeof(double) * mat->cols * mat->rows);
    return r;
}

Matrix create_identity(size_t rows, size_t cols) {
    Matrix m = create(rows, cols);
    for (size_t i = 0; i < rows && i < cols; i++) {
        GET_EL(m, i, i) = 1.0;
    }
    return m;
}

Matrix create_vector(size_t cols) {
    return create(1, cols);
}

Matrix create_zero(size_t rows, size_t cols) {
    Matrix m = create(rows, cols);
    ZeroMemory(m.A, sizeof(double) * cols * rows);
    return m;
}

Matrix create_constants(size_t rows, size_t cols, double value) {
    if (value != value)
        return create_empty();
    Matrix m = create(rows, cols);
    fill_double(m.A, value, cols * rows);
    return m;
}

bool is_valid(Matrix* mat) {
    return mat && mat->A && mat->cols && mat->rows;
}

void set_element(Matrix* mat, size_t rowIdx, size_t colIdx, double value) {
    if (!is_valid(mat) || rowIdx > mat->rows || colIdx > mat->cols)
        return;
    rowIdx -= 1;
    colIdx -= 1;
    GET_EL(*mat, rowIdx, colIdx) = value;
}
double get_element(Matrix* mat, size_t rowIdx, size_t colIdx) {
    if (!is_valid(mat) || rowIdx > mat->rows || colIdx > mat->cols)
        return NAN;
    rowIdx -= 1;
    colIdx -= 1;
    return GET_EL(*mat, rowIdx, colIdx);
}

Matrix multiply(Matrix* mat1, Matrix* mat2) {
    Matrix r = create_empty();
    if (!is_valid(mat1) || !is_valid(mat2) || mat1->cols != mat2->rows) {
        return r;
    }
    r = create_zero(mat1->rows, mat2->cols);

    for (size_t i = 0; i < r.rows; i++) {
        for (size_t j = 0; j < r.cols; j++) {
            for (size_t k = 0; k < mat1->cols; ++k) {
                GET_EL(r, i, j) += GET_EL(*mat1, i, k) * GET_EL(*mat2, k, j);
            }
        }
    }

    return r;
}

Matrix add(Matrix* mat1, Matrix* mat2) {
    Matrix r = create_empty();
    if (!is_valid(mat1) || !is_valid(mat2) || mat1->cols != mat2->cols || mat1->rows != mat2->rows)
        return r;

    r = create_zero(mat1->rows, mat1->cols);
    for (size_t i = 0; i < mat1->cols * mat1->rows; i++) {
        r.A[i] = mat1->A[i] + mat2->A[i];
    }
    return r;
}

Matrix multiply_scalar(Matrix* mat, double value) {
    Matrix r = create_empty();
    if (!is_valid(mat))
        return r;

    r = create_zero(mat->rows, mat->cols);
    for (size_t i = 0; i < mat->cols * mat->rows; i++) {
        r.A[i] = mat->A[i] * value;
    }
    return r;
}

Matrix divide_scalar(Matrix* mat, double value) {
    Matrix r = create_empty();
    if (!is_valid(mat) || value == 0.0)
        return r;

    r = create_zero(mat->rows, mat->cols);
    for (size_t i = 0; i < mat->cols * mat->rows; i++) {
        r.A[i] = mat->A[i] / value;
    }
    return r;
}

Matrix subtract(Matrix* mat1, Matrix* mat2) {
    Matrix r = create_empty();
    if (!is_valid(mat1) || !is_valid(mat2) || mat1->cols != mat2->cols || mat1->rows != mat2->rows)
        return r;

    r = create_zero(mat1->rows, mat1->cols);
    for (size_t i = 0; i < mat1->cols * mat1->rows; i++) {
        r.A[i] = mat1->A[i] - mat2->A[i];
    }
    return r;
}

void resize(Matrix* mat, size_t rows, size_t cols) {
    if (!is_valid(mat) && (!rows || !cols) && rows != cols)
        return;
    else if (!rows && !cols) {
        destroy(mat);
        return;
    }

    Matrix m = {.A = mat->A, .cols = cols, .rows = rows};
    if (mat->cols < cols) {
        mat->A = m.A = realloc(mat->A, sizeof(double) * rows * cols);
        assert(m.A && "couldn't allocate memory");
        for (size_t i = min(rows, mat->rows) - 1; i; i--) {
            for (size_t j = mat->cols; j; j--) {
                GET_EL(m, i, j - 1) = GET_EL(*mat, i, j - 1);
            }
        }
    } else {
        for (size_t i = 1; i < rows && i < mat->rows; i++) {
            for (size_t j = 0; j < cols && j < mat->cols; j++) {
                GET_EL(m, i, j) = GET_EL(*mat, i, j);
            }
        }
        m.A = realloc(mat->A, sizeof(double) * rows * cols);
        assert(m.A && "couldn't allocate memory");
    }

    for (size_t i = mat->rows; i < rows; ++i) {
        for (size_t j = 0; j < cols; j++) {
            GET_EL(m, i, j) = 0.0;
        }
    }
    for (size_t j = mat->cols; j < cols; ++j) {
        for (size_t i = 0; i < min(rows, mat->rows); i++) {
            GET_EL(m, i, j) = 0.0;
        }
    }
    memcpy(mat, &m, sizeof(Matrix));
}

void set_identity(Matrix* mat) {
    if (!is_valid(mat))
        return;

    ZeroMemory(mat->A, sizeof(double) * mat->rows * mat->cols);
    for (size_t i = 0; i < mat->rows && i < mat->cols; i++) {
        GET_EL(*mat, i, i) = 1.0;
    }
}

void set_zero(Matrix* mat) {
    if (!is_valid(mat))
        return;

    ZeroMemory(mat->A, sizeof(double) * mat->rows * mat->cols);
}

void set_constants(Matrix* mat, double value) {
    if (!is_valid(mat))
        return;

    fill_double(mat->A, value, mat->cols * mat->rows);
}

void set_identity_resize(Matrix* mat, size_t rows, size_t cols) {
    if (!is_valid(mat) && (!rows || !cols) && rows != cols)
        return;
    else if (!rows && !cols) {
        destroy(mat);
        return;
    }

    mat->A = realloc(mat->A, sizeof(double) * rows * cols);
    assert(mat->A && "couldn't allocate memory");
    mat->cols = cols;
    mat->rows = rows;
    set_identity(mat);
}

void set_zero_resize(Matrix* mat, size_t rows, size_t cols) {
    if (!is_valid(mat) && (!rows || !cols) && rows != cols)
        return;
    else if (!rows && !cols) {
        destroy(mat);
        return;
    }

    mat->A = realloc(mat->A, sizeof(double) * rows * cols);
    assert(mat->A && "couldn't allocate memory");
    mat->cols = cols;
    mat->rows = rows;
    set_zero(mat);
}

void set_constants_resize(Matrix* mat, size_t rows, size_t cols, double value) {
    if (!is_valid(mat) && (!rows || !cols) && rows != cols)
        return;
    else if (!rows && !cols) {
        destroy(mat);
        return;
    }

    mat->A = realloc(mat->A, sizeof(double) * rows * cols);
    assert(mat->A && "couldn't allocate memory");
    mat->cols = cols;
    mat->rows = rows;
    set_constants(mat, value);
}

Matrix transpose(Matrix* mat) {
    if (!is_valid(mat))
        return create_empty();

    Matrix t = create(mat->cols, mat->rows);
    for (size_t i = 0; i < mat->cols; i++) {
        for (size_t j = 0; j < mat->rows; j++) {
            GET_EL(t, j, i) = GET_EL(*mat, i, j);
        }
    }
    return t;
}

/**
 * \param i_a строка из который будем вычитать
 * \param i_b строка которой будем вычитать
 * \param start_index с какого столбца начинать шаманство
 */
static void gs_row_sub_mul(Matrix* mat, size_t i_a, size_t i_b, double k, size_t start_index) {
    for (size_t j = start_index; j < mat->cols; ++j) {
        GET_EL(*mat, i_a, j) -= GET_EL(*mat, i_b, j) * k;
    }
}

static void gs_row_div_scalar(Matrix* mat, size_t i, double k, size_t start_index) {
    for (size_t j = start_index; j < mat->cols; ++j) {
        GET_EL(*mat, i, j) /= k;
    }
}

double determinant(Matrix* mat) {
    if (!is_valid(mat))
        return NAN;
    else if (mat->rows != mat->cols)
        return 0.0;
    else if (mat->rows == 1)
        return mat->A[0];

    double det = 1.0;
    Matrix m = create_copy(mat);
    for (size_t j = 0; j < mat->cols; ++j) {
        const double a = GET_EL(m, j, j);
        if (!cmp_double(a, 0.0)) {
            det = 0.0;
            break;
        }
        det *= a;
        for (size_t i = j + 1; i < mat->rows; ++i) {
            if (!cmp_double(GET_EL(m, i, j), 0.0))
                continue;
            gs_row_sub_mul(&m, i, j, GET_EL(m, i, j) / a, j);
        }
    }

    destroy(&m);
    return det;
}

Matrix inverse(Matrix* mat) {
    if (!is_valid(mat))
        return create_empty();
    else if (mat->rows != mat->cols)
        return create_empty();
    else if (mat->rows == 1)
        return create_constants(1, 1, 1.0 / mat->A[0]);

    Matrix t = create_copy(mat);
    Matrix r = create_identity(mat->rows, mat->cols);

    for (size_t j = 0; j < mat->cols; j++) {
        double a = GET_EL(t, j, j);
        if (!cmp_double(a, 0.0)) {
            destroy(&r);
            destroy(&t);
            return r;
        } else if (cmp_double(a, 1.0)) {
            gs_row_div_scalar(&t, j, a, j);
            gs_row_div_scalar(&r, j, a, 0);
        }
        for (size_t i = j + 1; i < mat->rows; ++i) {
            a = GET_EL(t, i, j);
            if (!cmp_double(a, 0.0))
                continue;
            gs_row_sub_mul(&t, i, j, a, j);
            gs_row_sub_mul(&r, i, j, a, 0);
        }
    }
    for (size_t j = mat->cols - 1; j > 0; j--) {
        for (size_t i = j; i > 0; i--) {
            const double a = GET_EL(t, i - 1, j);
            // gs_row_sub_mul(&t, i - 1, j, a, j);
            gs_row_sub_mul(&r, i - 1, j, a, 0);
        }
    }

    destroy(&t);
    return r;
}

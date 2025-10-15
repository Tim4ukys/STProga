#ifndef __ST_DATA_MATRIX_H__
#define __ST_DATA_MATRIX_H__

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    double* A;
	size_t cols, rows;
} Matrix;

Matrix create_empty();
Matrix create_vector(size_t cols);
Matrix create(size_t rows, size_t cols);
Matrix create_identity(size_t rows, size_t cols);
Matrix create_zero(size_t rows, size_t cols);
Matrix create_constants(size_t rows, size_t cols, double value);
Matrix create_copy(Matrix* mat);
void   destroy(Matrix* mat);

Matrix multiply(Matrix* mat1, Matrix* mat2);
Matrix subtract(Matrix* mat1, Matrix* mat2);
Matrix add(Matrix* mat1, Matrix* mat2);
Matrix multiply_scalar(Matrix* mat, double value);
Matrix divide_scalar(Matrix* mat, double value);

bool is_valid(Matrix* mat);
void resize(Matrix* mat, size_t rows, size_t cols);

double get_element(Matrix* mat, size_t rowIdx, size_t colIdx);
void   set_element(Matrix* mat, size_t rowIdx, size_t colIdx, double value);

void set_identity(Matrix* mat);
void set_zero(Matrix* mat);
void set_constants(Matrix* mat, double value);

void set_identity_resize(Matrix* mat, size_t rows, size_t cols);
void set_zero_resize(Matrix* mat, size_t rows, size_t cols);
void set_constants_resize(Matrix* mat, size_t rows, size_t cols, double value);

Matrix transpose(Matrix* mat);
Matrix inverse(Matrix* mat);
double determinant(Matrix* mat);

#endif

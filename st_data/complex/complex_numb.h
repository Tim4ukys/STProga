#ifndef __ST_DATA_COMPLEX_NUMB_H__
#define __ST_DATA_COMPLEX_NUMB_H__

#include <stdbool.h>

typedef struct {
    double re, im;
} Complex;

typedef struct {
    double r, phi;
} PolarComplex;

void complex_add(const Complex* a, const Complex* b, Complex* out);
void complex_sub(const Complex* a, const Complex* b, Complex* out);
void complex_mul(const Complex* a, const Complex* b, Complex* out);
void complex_div(const Complex* a, const Complex* b, Complex* out);

double complex_abs(const Complex* a);

void complex_alg_to_polar(const Complex* a, PolarComplex* out);
void complex_polar_to_alg(const PolarComplex* a, Complex* out);

void complex_pow(const Complex* a, double degree, Complex* out);

void complex_conj(const Complex* a, Complex* out);

#endif

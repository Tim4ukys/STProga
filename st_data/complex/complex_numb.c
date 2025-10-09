#include "complex_numb.h"
#include <math.h>
#include <memory.h>

#define NAN_COMPLEX(a) (a->re != a->re || a->im != a->im)
#define NAN_POLAR(a)   (a->r != a->r || a->phi != a->phi)

#define INVALID_ARGS(a, b, out) (!a || !b || !out || NAN_COMPLEX(a) || NAN_COMPLEX(b))

void complex_add(const Complex* a, const Complex* b, Complex* out) {
    if (INVALID_ARGS(a, b, out)) {
        if (out)
            memset(out, NULL, sizeof(Complex));
        return;
    }
    out->re = a->re + b->re;
    out->im = a->im + b->im;
}

void complex_sub(const Complex* a, const Complex* b, Complex* out) {
    if (INVALID_ARGS(a, b, out)) {
        if (out)
            memset(out, NULL, sizeof(Complex));
        return;
    }
    out->re = a->re - b->re;
    out->im = a->im - b->im;
}

void complex_mul(const Complex* a, const Complex* b, Complex* out) {
    if (INVALID_ARGS(a, b, out)) {
        if (out)
            memset(out, NULL, sizeof(Complex));
        return;
    }
    out->re = a->re * b->re - a->im * b->im;
    out->im = a->re * b->im + a->im * b->re;
}

void complex_div(const Complex* a, const Complex* b, Complex* out) {
    if (INVALID_ARGS(a, b, out)) {
        if (out)
            memset(out, NULL, sizeof(Complex));
        return;
    }
    complex_mul(a, b, out);
    double zn = b->re * b->re - b->im * b->im;
    out->re /= zn;
    out->im /= zn;
}

double complex_abs(const Complex* a) {
    if (!a || NAN_COMPLEX(a))
        return NAN;
    return sqrt(a->im * a->im + a->re * a->re);
}

void complex_alg_to_polar(const Complex* a, PolarComplex* out) {
    if (!a || NAN_COMPLEX(a) || !out) {
        memset(out, NULL, sizeof(Complex));
        return;
    }
    out->r = complex_abs(a);
    out->phi = atan2(a->im, a->re); // pi/2
}

void complex_polar_to_alg(const PolarComplex* a, Complex* out) {
    if (!a || !out || NAN_POLAR(a)) {
        memset(out, NULL, sizeof(Complex));
        return;
    }
    out->re = a->r * cos(a->phi);
    out->im = a->r * sin(a->phi);
}

void complex_pow(const Complex* a, double degree, Complex* out) {
    if (!a || !out || NAN_COMPLEX(a)) {
        memset(out, NULL, sizeof(Complex));
        return;
    }
    PolarComplex tmp;
    complex_alg_to_polar(a, &tmp);
    tmp.r = pow(tmp.r, degree);
    tmp.phi *= degree;
    complex_polar_to_alg(&tmp, out);
}

void complex_conj(const Complex* a, Complex* out) {
    if (!a || !out || NAN_COMPLEX(a)) {
        memset(out, NULL, sizeof(Complex));
        return;
    }
    out->re = a->re;
    out->im = -a->im;
}

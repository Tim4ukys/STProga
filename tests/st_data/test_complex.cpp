#include <gtest/gtest.h>
#include <utility>
#include <complex>
#define _USE_MATH_DEFINES 
#include <math.h>
#include <random>

extern "C" {
#include <complex_numb.h>
}

template<typename T>
inline T random(const T& min, const T& max) {
    static std::random_device              rd;
    static std::default_random_engine      e1(rd());
    
    if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>) {
        std::uniform_real_distribution<T> uniform_dist(min, max);
        return uniform_dist(e1);
    } else {
        std::uniform_int<T> uniform_dist(min, max);
        return uniform_dist(e1);
    }
}

static int __cdecl cmp_double(const double& a, const double& b) {
    constexpr double eps = 1e-7;

    if (!a && !b) {
        return 0;
    } else if (a == std::numeric_limits<double>::infinity() && b == std::numeric_limits<double>::infinity() ||
               a == -std::numeric_limits<double>::infinity() && b == -std::numeric_limits<double>::infinity()) {
        return 0;
    }

    return fabs(a - b) / fabs(b + a) < eps ? 0 : (b > a ? 1 : -1);
}

TEST(StData_ComplexNumbs, NoVallidArgs) {
    Complex a, b, c_null;
    memset(&a, 0xFF, sizeof(Complex));
    memset(&b, 0xFF, sizeof(Complex));
    memset(&c_null, 0x00, sizeof(Complex));

    for (auto& f : {complex_add, complex_sub, complex_mul, complex_div}) {
        f(NULL, &a, &b);
        EXPECT_FALSE(memcmp(&c_null, &b, sizeof(Complex)));
        memset(&b, 0xFF, sizeof(Complex));
        f(&a, NULL, &b);
        EXPECT_FALSE(memcmp(&c_null, &b, sizeof(Complex)));
        memset(&b, 0xFF, sizeof(Complex));
        f(&a, &b, NULL);
    }

    complex_abs(NULL);

    PolarComplex p_a, p_null;
    memset(&p_null, 0x00, sizeof(PolarComplex));
    memset(&p_a, 0xFF, sizeof(PolarComplex));
    complex_alg_to_polar(NULL, &p_null);
    EXPECT_FALSE(memcmp(&p_a, &p_null, sizeof(PolarComplex)));
    complex_alg_to_polar(NULL, NULL);
    complex_alg_to_polar(&a, NULL);

    complex_polar_to_alg(NULL, &b);
    EXPECT_FALSE(memcmp(&c_null, &b, sizeof(Complex)));
    memset(&b, 0xFF, sizeof(Complex));
    complex_polar_to_alg(&p_a, NULL);
    complex_polar_to_alg(NULL, NULL);

    complex_pow(NULL, 0, NULL);
    complex_pow(&a, 0, NULL);
    complex_pow(NULL, 0, &b);
    EXPECT_FALSE(memcmp(&c_null, &b, sizeof(Complex)));
    memset(&b, 0xFF, sizeof(Complex));

    complex_conj(NULL, NULL);
    complex_conj(&a, NULL);
    complex_conj(NULL, &b);
    EXPECT_FALSE(memcmp(&c_null, &b, sizeof(Complex)));
}

TEST(StData_ComplexNumbs, Abs) {
    Complex a;
    std::complex<double> p_a;

    for (size_t i{}; i < 100; ++i) {
        p_a._Val[0] = a.re = random<double>(-1000, 1000);
        p_a._Val[1] = a.im = random<double>(-1000, 1000);
        EXPECT_FALSE(cmp_double(complex_abs(&a), std::abs(p_a)));     
    }
}

TEST(StData_ComplexNumbs, PolarCust_Conj) {
    Complex      a{0, -100}, b;
    PolarComplex p_a, p_b;
    complex_alg_to_polar(&a, &p_a);
    EXPECT_FALSE(cmp_double(p_a.phi, -M_PI / 2));
    a.re = 100;
    complex_alg_to_polar(&a, &p_a);
    EXPECT_FALSE(cmp_double(p_a.phi, -M_PI / 4));
    EXPECT_FALSE(cmp_double(p_a.r, 100.0 * std::sqrt(2)));

    a.im = random<double>(-1000.0, 1000.0);
    a.re = random<double>(-1000.0, 1000.0);
    complex_alg_to_polar(&a, &p_a);
    complex_conj(&a, &b);
    complex_alg_to_polar(&b, &p_b);
    EXPECT_FALSE(cmp_double(p_b.phi, -p_a.phi));
}

TEST(StData_ComplexNumbs, Arithmetic) {
    Complex a, b, c;
    
    a.im = random<double>(-1000.0, 1000.0);
    a.re = random<double>(-1000.0, 1000.0);

    b.im = random<double>(1, 1000.0);
    b.re = random<double>(1, 1000.0);
    complex_add(&a, &b, &c);

    std::complex<double> c_a{a.re, a.im}, c_b{b.re, b.im},
        c_c = c_b + c_a;
    EXPECT_TRUE(!cmp_double(c.im, c_c.imag()) && !cmp_double(c.re, c_c.real()));

    complex_mul(&a, &b, &c);
    c_c = c_b * c_a;
    EXPECT_TRUE(!cmp_double(c.im, c_c.imag()) && !cmp_double(c.re, c_c.real()));

    complex_div(&a, &b, &c);
    c_c = c_a / c_b;
    EXPECT_TRUE(!cmp_double(c.im, c_c.imag()) && !cmp_double(c.re, c_c.real()));
    
    complex_sub(&a, &b, &c);
    c_c = c_a - c_b;
    EXPECT_TRUE(!cmp_double(c.im, c_c.imag()) && !cmp_double(c.re, c_c.real()));
}

TEST(StData_ComplexNumbs, Pow) {
    Complex              a, b;
    std::complex<double> p_a;

    for (size_t i{}; i < 100; ++i) {
        p_a._Val[0] = a.re = random<double>(-1000, 1000);
        p_a._Val[1] = a.im = random<double>(-1000, 1000);
        const double degree = random<double>(-1000, 1000);
        complex_pow(&a, degree, &b);
        p_a = std::pow(p_a, degree);
        
        EXPECT_TRUE(!cmp_double(b.im, p_a.imag()) && !cmp_double(b.re, p_a.real()));
    }
}
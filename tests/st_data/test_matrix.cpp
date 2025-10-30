#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <gtest/gtest.h>
#include <utility>
#include <random>
#include <armadillo>

namespace mat {
    extern "C" {
#include <matrix.h>
    }
} // namespace mat

class MemoryLeakCheck {
#ifndef NDEBUG
    _CrtMemState s1, s2{}, s3{};

public:
    explicit MemoryLeakCheck() {
        _CrtMemCheckpoint(&s1);
    }
    ~MemoryLeakCheck() {
        _CrtMemCheckpoint(&s2);
        assert(!_CrtMemDifference(&s3, &s1, &s2) && "Memory leak =((");
    }
#else
public:
    explicit MemoryLeakCheck() = default;
    ~MemoryLeakCheck() = default;
#endif
};

inline int cmp_double(const double& a, const double& b) {
    constexpr double eps = 1e-9;
    if (!a && !b) {
        return 0;
    }
    if (fabs(a - b) < eps)
        return 0;

    return fabs(a - b) / fabs(a + b) < eps ? 0 : (b > a ? 1 : -1);
}

template<typename T>
inline T random(const T& min, const T& max) {
    static std::random_device         rd;
    static std::default_random_engine e1(rd());

    if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>) {
        std::uniform_real_distribution<T> uniform_dist(min, max);
        return uniform_dist(e1);
    } else {
        std::uniform_int_distribution<T> uniform_dist(min, max);
        return uniform_dist(e1);
    }
}

/////////////////////////////////

class CMatrix {
    mutable mat::Matrix m_mat;

public:
    CMatrix(CMatrix& a) {
        m_mat = mat::create_copy(&a.m_mat);
    }
    CMatrix(const mat::Matrix& mat)
        : m_mat(mat) {
    }
    CMatrix(const std::vector<double>& numbs, const std::pair<size_t, size_t>& sz) {
        m_mat = mat::create(sz.first, sz.second);
        memcpy(m_mat.A, numbs.data(), numbs.size() * sizeof(double));
    };
    ~CMatrix() {
        mat::destroy(&m_mat);
    }

    inline mat::Matrix* operator&() const noexcept {
        return &m_mat;
    }
    inline mat::Matrix* operator->() const noexcept {
        return &m_mat;
    }
};

TEST(StData_Matrix, Resize) {
    auto    mt = std::make_unique<MemoryLeakCheck>();
    CMatrix a = mat::create(3, 3);
    for (size_t j = 1; j <= a->cols; j++) {
        for (size_t i = 1; i <= a->rows; i++) {
            auto d = random<double>(-100, 100);
            mat::set_element(&a, i, j, d);
        }
    }

    struct {
        CMatrix                   m_CMat;
        std::pair<size_t, size_t> m_Size;
    } b[] = {{mat::create_copy(&a), {4, 2}},
             {mat::create_copy(&a), {2, 4}},
             {mat::create_copy(&a), {2, 2}},
             {mat::create_copy(&a), {6, 6}}};

    for (auto& i : b) {
        mat::resize(&i.m_CMat, i.m_Size.first, i.m_Size.second);
        auto& m = i.m_CMat;
        for (size_t j = 1; j <= m->cols; j++) {
            for (size_t i = 1; i <= m->rows; i++) {
                EXPECT_TRUE(mat::get_element(&m, i, j) == ((j <= a->cols && i <= a->rows) ? mat::get_element(&a, i, j) : 0.0));
            }
        }
    }
}

TEST(StData_Matrix, SetGet) {
    auto mt = std::make_unique<MemoryLeakCheck>();

    CMatrix a = mat::create_zero(3, 4);
    for (size_t j = 1; j <= a->cols; j++) {
        for (size_t i = 1; i <= a->rows; i++) {
            auto d = random<double>(-100, 100);
            mat::set_element(&a, i, j, d);
            EXPECT_TRUE(d == mat::get_element(&a, i, j));
        }
    }
}

TEST(StData_Matrix, Algebra) {
    auto mt = std::make_unique<MemoryLeakCheck>();

    auto empty = mat::create_empty();
    for (auto& f : {mat::multiply, mat::subtract, mat::add}) {
        auto a = f(&empty, 0);
        EXPECT_FALSE(memcmp(&a, &empty, sizeof(mat::Matrix)));
        a = f(0, &empty);
        EXPECT_FALSE(memcmp(&a, &empty, sizeof(mat::Matrix)));
        a = f(&empty, &empty);
        EXPECT_FALSE(memcmp(&a, &empty, sizeof(mat::Matrix)));
    }
    for (auto& f : {mat::multiply_scalar, mat::divide_scalar}) {
        auto a = f(&empty, 0);
        EXPECT_FALSE(memcmp(&a, &empty, sizeof(mat::Matrix)));
        a = f(0, 0);
        EXPECT_FALSE(memcmp(&a, &empty, sizeof(mat::Matrix)));
    }

    std::vector<double> numbs;
    numbs.resize(4 * 4);
    for (size_t j = 0; j < 4 * 4; j++) {
        numbs[j] = random<double>(-1000.0, 1000.0);
    }
    auto a = CMatrix(numbs, {4, 4});
    auto b = CMatrix(mat::add(&CMatrix(mat::add(&a, &a)), &a));
    auto eq = [](const double& a, const double& b) {
        return !cmp_double(a, b);
    };
    EXPECT_TRUE(std::equal(b->A, b->A + b->cols * b->rows, CMatrix(mat::multiply_scalar(&a, 3.0))->A, eq));

    auto c = CMatrix(mat::divide_scalar(&CMatrix(mat::multiply_scalar(&a, 5)), 5));
    EXPECT_TRUE(std::equal(a->A, a->A + a->cols * a->rows, c->A, eq));
}

TEST(StData_Matrix, Create) {
    auto mt = std::make_unique<MemoryLeakCheck>();

    auto a = mat::create_empty();
    auto zero = std::vector<uint8_t>(sizeof(mat::Matrix));
    EXPECT_FALSE(memcmp(&a, zero.data(), sizeof(mat::Matrix)));
    a = mat::create_vector(0);
    EXPECT_FALSE(memcmp(&a, zero.data(), sizeof(mat::Matrix)));
    a = mat::create(0, 0);
    EXPECT_FALSE(memcmp(&a, zero.data(), sizeof(mat::Matrix)));
    a = mat::create_identity(0, 0);
    EXPECT_FALSE(memcmp(&a, zero.data(), sizeof(mat::Matrix)));
    a = mat::create_zero(0, 0);
    EXPECT_FALSE(memcmp(&a, zero.data(), sizeof(mat::Matrix)));
    a = mat::create_constants(0, 0, 0);
    EXPECT_FALSE(memcmp(&a, zero.data(), sizeof(mat::Matrix)));
    a = mat::create_copy(0);
    EXPECT_FALSE(memcmp(&a, zero.data(), sizeof(mat::Matrix)));

    a = mat::create_identity(3, 4);
    for (size_t i = 1; i <= a.rows; ++i) {
        for (size_t j = 1; j <= a.cols; ++j) {
            EXPECT_FALSE(cmp_double(mat::get_element(&a, i, j), i == j ? 1.0 : 0.0));
        }
    }

    mat::destroy(&a);
    a = mat::create_zero(3, 4);
    EXPECT_FALSE(memcmp(a.A, std::vector<double>(size_t(3 * 4), 0).data(), 3 * 4 * sizeof(double)));
    mat::destroy(&a);

    auto d = random<double>(1.0, 1e5);
    a = mat::create_constants(3, 4, d);
    EXPECT_FALSE(memcmp(a.A, std::vector<double>(size_t(3 * 4), d).data(), 3 * 4 * sizeof(double)));
    mat::destroy(&a);
}

TEST(StData_Matrix, Transposition) {
    auto mt = std::make_unique<MemoryLeakCheck>();

    for (size_t i = 0; i < 100; i++) {
        std::vector<double> numbs;
        numbs.resize(4 * 4);
        for (size_t j = 0; j < 4 * 4; j++) {
            numbs[j] = random<double>(-1000.0, 1000.0);
        }
        auto a = CMatrix(numbs, {4, 4});
        auto b = CMatrix(mat::transpose(&CMatrix(mat::transpose(&a))));
        auto eq = [](const double& a, const double& b) {
            return !cmp_double(a, b);
        };
        EXPECT_TRUE(std::equal(a->A, a->A + a->cols * a->rows, b->A, eq));
    }
}

TEST(StData_Matrix, Det) {
    auto mt = std::make_unique<MemoryLeakCheck>();

    for (size_t l = 0; l < 100; l++) {
        std::vector<double> numbs;
        auto                sz = std::pair(size_t(random<int>(1, 5)), size_t(random<int>(1, 5)));
        numbs.resize(sz.second * sz.first);
        for (size_t j = 0; j < sz.second * sz.first; j++) {
            numbs[j] = random<double>(-1000.0, 1000.0);
        }

        auto a = CMatrix(numbs, sz);
        auto mt = arma::mat(numbs.data(), sz.first, sz.second);
        if (sz.second != sz.first) {
            EXPECT_FALSE(cmp_double(mat::determinant(&a), 0.0));
            continue;
        }

        EXPECT_FALSE(cmp_double(arma::det(mt), mat::determinant(&a)));
    }
}

TEST(StData_Matrix, Inverse) {
    auto mt = std::make_unique<MemoryLeakCheck>();

    for (size_t l = 0; l < 100; l++) {
        std::vector<double> numbs;
        auto                sz = std::pair(size_t(random<int>(1, 5)), size_t(random<int>(1, 5)));
        numbs.resize(sz.second * sz.first);
        for (size_t j = 0; j < sz.second * sz.first; j++) {
            numbs[j] = random<double>(-1000.0, 1000.0);
        }

        CMatrix a = CMatrix(numbs, sz),
                b = mat::inverse(&a),
                E = mat::multiply(&a, &b);

        if (!mat::is_valid(&E)) {
            EXPECT_FALSE(cmp_double(mat::determinant(&a), 0.0));
            continue;
        }

        for (size_t i = 1; i <= b->rows; i++) {
            for (size_t j = 1; j <= b->cols; j++) {
                EXPECT_FALSE(cmp_double(mat::get_element(&E, i, j), i == j ? 1.0 : 0.0));
            }
        }
    }
}

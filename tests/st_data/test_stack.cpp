#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <gtest/gtest.h>
#include <random>

extern "C" {
#include <stack.h>
}

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

TEST(StData_Stack, NoValidArgs) {
    auto mem_leak = std::make_unique<MemoryLeakCheck>();

    Stack st{};
    stack_create(0);
    stack_create(&st);
    EXPECT_FALSE(st.m_arr);

    stack_destroy(0);
    stack_push(0, 0);
    stack_push(&st, 0);
    EXPECT_FALSE(stack_size(&st));
    EXPECT_FALSE(stack_pop(0));
    EXPECT_FALSE(stack_peek(0));
}

TEST(StData_Stack, create) {
    auto mem_leak = std::make_unique<MemoryLeakCheck>();

    constexpr size_t    N_NUMBS = 1000;
    std::vector<double> m_numbs;
    m_numbs.resize(N_NUMBS);
    Stack               stack{};
    stack.m_szElement = sizeof(double);
    stack_create(&stack);

    for (size_t i{}; i < N_NUMBS; i++) {
        auto n = random<double>(-1000, 1000);
        EXPECT_TRUE(stack_size(&stack) == i);
        stack_push(&stack, &n);
        m_numbs[i] = n;
    }
    for (size_t i{}; i < N_NUMBS; ++i) {
        auto a = *(double*)stack_peek(&stack);
        auto b = *(double*)stack_pop(&stack);
        EXPECT_TRUE(a == b);
        EXPECT_TRUE(m_numbs[N_NUMBS - i - 1] == a);
    }
    EXPECT_TRUE(stack_size(&stack) == 0);

    stack_destroy(&stack);
}

#include <gtest/gtest.h>
#include <string_view>
#include <vector>

extern "C" {
#include <rem_comments.h>
}

TEST(ZVeZda_RemCom, SimpleTests) {
    std::vector<std::pair<std::string_view, RC_STATE>> test{
        {R"(// /* \
/*
)", RC_STATE_OK }, 
{R"(/* /\*/)", RC_STATE_OK},
        {R"(/* 
// \
*/)",
         RC_STATE_OK},
        {R"(/* *\/)", RC_STATE_BALANCE_ERR}
    };
    for (auto& [str, state] : test) {
        stRCCheck ch;
        rc_reset(&ch);
        rc_next(str.data(), str.size(), &ch, 0, 0);
        EXPECT_TRUE(rc_check(&ch) == state);
    }
}
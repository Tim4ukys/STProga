#include <gtest/gtest.h>
#include <string_view>
#include <vector>

extern "C" {
#include <rem_comments.h>
}

TEST(ZVeZda_RemCom, Rem) {
    std::vector<std::pair<std::string_view, std::string_view>> test{
        {R"(goida = /*country 404
zrada 
peremoga*/ SVO
//gg
ZOV)", R"(goida =  SVO
ZOV)"},
        {R"(//
//
//
//
//

test)",
         R"(
test)"}
    };

    for (auto& [in, et] : test) {
        std::string out;
        out.resize(in.size());
        size_t out_ln;
        stRCCheck ch;
        rc_reset(&ch);
        rc_next(in.data(), in.size(), &ch, out.data(), &out_ln);
        EXPECT_TRUE(!strcmp(et.data(), out.data()) && out_ln == et.size());
    }
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
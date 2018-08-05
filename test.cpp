#include "gtest/gtest.h"
#include "xt_quick_cast.h"
#include "xt_hash_helper.h"
#include <string_view>
#include <iostream>
#include <optional>
#include <any>
#include <variant>



TEST(XTUTILS_GOOGLE_TEST, QUICK_CAST_TEST)
{
    EXPECT_EQ(QCast<int>(QCast<std::string>(1)), 1);
    EXPECT_EQ(QCastFromVec<std::string>(std::vector<int>{1, 2, 3, 4, 5}, ";"), "1;2;3;4;5");
    {
        auto vec = std::vector<int>{1, 2, 3, 4, 5};
        EXPECT_EQ(QCastToVec<int>("  1  ;2   ;3 ;4 ;5 ", ";"), vec);
    }
}

TEST(XTUTILS_GOOGLE_TEST, HASH_HELPER_TEST)
{
    constexpr auto target_value = XTSTATIC_HASH_STR("std::vector<int>");
    auto current_value1 = XTSTATIC_HASH(std::vector<int>);
    EXPECT_EQ(current_value1, target_value);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

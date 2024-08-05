#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <any>


#include "../lib/any.h"


TEST(Create, Empty) {
    Any any;
    ASSERT_TRUE(any.empty());
}

TEST(Create, TrivialType) {
    Any my_any;
    std::any real_any;
    int x = 10;
    double y = 123.045;

    my_any = x;
    real_any = x;
    ASSERT_EQ(any_cast<int>(my_any), std::any_cast<int>(real_any));

    my_any = y;
    real_any = y;
    ASSERT_EQ(any_cast<double>(my_any), std::any_cast<double>(real_any));
}

TEST(Create, NotTrivialType) {
    Any my_any;
    std::any real_any;
    std::string x = "asdflkdf";
    std::vector y{1, 2, 3, 4, 5, 6, 7, 8, 9};

    my_any = x;
    real_any = x;
    ASSERT_EQ(any_cast<std::string>(my_any), std::any_cast<std::string>(real_any));

    my_any = y;
    real_any = y;
    ASSERT_EQ(any_cast<std::vector<int>>(my_any), std::any_cast<std::vector<int>>(real_any));
}

TEST(Create, Move) {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7};
    auto copy_arr = arr;
    Any my_any;
    my_any = std::move(arr);
    ASSERT_TRUE(arr.empty());
    ASSERT_EQ(any_cast<std::vector<int>>(my_any), copy_arr);
}


TEST(Copy, TrivialType) {
    int x = 5;
    Any any = x;
    Any copy_any = any;
    ASSERT_EQ(any_cast<int>(any), any_cast<int>(copy_any));
    double y = 1.546;
    any = y;
    copy_any = any;
    ASSERT_EQ(any_cast<double>(any), any_cast<double>(copy_any));
}

TEST(Copy, NotTrivialType) {
    Any any;
    Any copy_any;
    std::string x = "asdflkdf";
    std::vector y{1, 2, 3, 4, 5, 6, 7, 8, 9};
    any = x;
    copy_any = any;
    ASSERT_EQ(any_cast<std::string>(any), any_cast<std::string>(copy_any));

    any = y;
    copy_any = any;
    ASSERT_EQ(any_cast<std::vector<int>>(any), any_cast<std::vector<int>>(copy_any));
}

TEST(Copy, Move) {
    Any any;
    Any copy_any;
    std::string x = "asdflkdf";
    any = x;
    copy_any = std::move(any);
    ASSERT_TRUE(any.empty());
    ASSERT_EQ(any_cast<std::string>(copy_any), x);
}

TEST(Cast, NotConst) {
    Any any;
    std::string x = "asdfasdf";
    any = x;
    ASSERT_EQ(any_cast<std::string>(any), x);
}

TEST(Cast, Const) {
    std::string x;
    const Any any = x = "asdfasdf";
    ASSERT_EQ(any_cast<std::string>(any), x);
}

TEST(Cast, Rvalue) {
    std::string x = "sdfsdf";
    Any any = x;
    any = x;
    ASSERT_EQ(any_cast<std::string>(std::move(any)), x);
}

TEST(Cast, BadCast) {
    Any any = std::string("asdf");
    ASSERT_THROW(any_cast<int>(any), bad_any_cast);
}





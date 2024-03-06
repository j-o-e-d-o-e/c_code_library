#include "unity.h"
#include "unity_fixture.h"
#include "code-library.h"

TEST_GROUP(utils);

TEST_SETUP(utils) {
}

TEST_TEAR_DOWN(utils) {
}

TEST(utils, lower) {
    char res[4];
    lower("Abc", res);
    TEST_ASSERT_EQUAL_STRING("abc", res);
}

TEST(utils, lowerAndTrim) {
    char res[4];
    lower_and_trim("  Abc  ", res);
    TEST_ASSERT_EQUAL_STRING("abc", res);
}

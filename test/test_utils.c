#include <string.h>
#include "unity.h"
#include "unity_fixture.h"
#include "code_library.h"

TEST_GROUP(utils);

TEST_SETUP(utils) {
}

TEST_TEAR_DOWN(utils) {
}

TEST(utils, trim_singleWord_noWhitespace) {
    char dest[TITLE_LEN];
    char *src = "data";
    trim(dest, src);
    TEST_ASSERT_EQUAL_STRING("data", dest);
}

TEST(utils, trim_twoWords_noWhitespace) {
    char dest[TITLE_LEN];
    char *src = "data struct";
    trim(dest, src);
    TEST_ASSERT_EQUAL_STRING("data struct", dest);
}

TEST(utils, trim_twoWords_withWhitespace) {
    char dest[TITLE_LEN];
    char *src = "   data structure   ";
    trim(dest, src);
    TEST_ASSERT_EQUAL_STRING("data structure", dest);
}

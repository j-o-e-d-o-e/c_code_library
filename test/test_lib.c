#include <malloc.h>
#include "unity.h"
#include "unity_fixture.h"
#include "code-library.h"

TEST_GROUP(lib);

TEST_SETUP(lib) {
}

TEST_TEAR_DOWN(lib) {
}

TEST(lib, createLib) {
    Library *res = setup_lib();
    struct entry *entries = res->entries;
    TEST_ASSERT_GREATER_THAN_INT(0, res->len);
    for (int i = 0; i < res->len; i++) {
        struct entry e = entries[i];
        TEST_ASSERT_EQUAL_INT(i + 1, e.index);
    }
    free(res);
}

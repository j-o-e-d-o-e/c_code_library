#include <string.h>
#include <malloc.h>
#include "unity.h"
#include "unity_fixture.h"
#include "code-library.h"

TEST_GROUP(lib);

TEST_SETUP(lib) {
}

TEST_TEAR_DOWN(lib) {
}

TEST(lib, create) {
    Library *lib = create_lib();
    TEST_ASSERT_GREATER_THAN_UINT8(0, lib->len);
    struct entry *entries = lib->entries;
    for (unsigned char i = 0; i < lib->len; i++) {
        struct entry entry = entries[i];
        TEST_ASSERT_TRUE(strlen(entry.title) > 0);
        TEST_ASSERT_EQUAL_UINT8(i + 1, entry.index);
    }
    free(lib);
}

TEST(lib, search) {
    Library *lib = create_lib();
    char *search_term = "data struct";
    Library *lib_search = search_lib(lib, search_term);
    free(lib);
    TEST_ASSERT_GREATER_THAN_UINT8(0, lib_search->len);
    struct entry *entries = lib_search->entries;
    for (unsigned char i = 0; i < lib_search->len; i++) TEST_ASSERT_TRUE(strstr(entries[i].tags, search_term));
    free(lib_search);
}

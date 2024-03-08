#include "unity_fixture.h"

TEST_GROUP_RUNNER(lib) {
    RUN_TEST_CASE(lib, create);
    RUN_TEST_CASE(lib, search);
}

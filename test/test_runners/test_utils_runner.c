#include "unity_fixture.h"

TEST_GROUP_RUNNER(utils) {
    RUN_TEST_CASE(utils, lower);
    RUN_TEST_CASE(utils, lowerAndTrim);
}

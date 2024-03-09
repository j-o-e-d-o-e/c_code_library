#include "unity_fixture.h"

TEST_GROUP_RUNNER(utils) {
    RUN_TEST_CASE(utils, trim_singleWord_noWhitespace);
    RUN_TEST_CASE(utils, trim_twoWords_noWhitespace);
    RUN_TEST_CASE(utils, trim_twoWords_withWhitespace);
}

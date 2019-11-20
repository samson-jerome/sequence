#include "sequence.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


TEST_CASE("first test", "test1") {
  REQUIRE(sequence::give_me_one() == 1);
}
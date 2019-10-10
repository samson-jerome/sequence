#include "gtest/gtest.h"
#include "seqparser.h"

TEST (give_me_one, correct_return) { 
    EXPECT_EQ (1, sequence::give_me_one ());
}

TEST (give_me_one, incorrest) { 
    ASSERT_EQ (1, sequence::give_me_one ());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
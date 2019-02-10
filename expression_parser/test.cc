#include "utils.hh"

#include <gtest/gtest.h>

namespace ExpressionParser {
namespace Tests {

std::vector<Expression> expressions;

TEST(a, b) { EXPECT_EQ(2 + 2, 4); }

} // namespace Tests
} // namespace ExpressionParser

int main(int argc, char **argv) {

  ExpressionParser::Tests::expressions =
      ExpressionParser::loadFromFile("expressions.txt");

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

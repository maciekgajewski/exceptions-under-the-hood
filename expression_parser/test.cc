#include "evaluate_exceptions.hh"
#include "utils.hh"

#include <gtest/gtest.h>

namespace ExpressionParser {
namespace Tests {

static std::vector<Expression> expressions;

TEST(ExceptionsEvaluatorTest, EmptyIsError) {

  EXPECT_ANY_THROW(evaluateExceptions({}));
}

TEST(ExceptionsEvaluatorTest, ValidCases) {

  for (const Expression &expression : expressions) {
    int value = evaluateExceptions(expression.expression);

    EXPECT_EQ(expression.result, value);
  }
}

} // namespace Tests
} // namespace ExpressionParser

int main(int argc, char **argv) {

  ExpressionParser::Tests::expressions =
      ExpressionParser::loadFromFile("expressions.txt");

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

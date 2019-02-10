#pragma once
#include <string>

namespace ExpressionParser {

namespace Detail {}

template <typename It> int evaluateExceptions(It first, It last) {
  // TODO
  return 0;
}

// evaluates expression, throws on error
int evaluateExceptions(const std::string &expression) {
  return evaluateExceptions(expression.begin(), expression.end());
}

} // namespace ExpressionParser

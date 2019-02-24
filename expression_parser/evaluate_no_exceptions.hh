#pragma once
#include <cassert>
#include <stdexcept>
#include <string>

namespace ExpressionParser {

extern std::string _error;

// evaluates expression, return true if ok. Value returned by out param, error
// in global _error;
bool evaluateNoExceptions(const std::string &expression, int &result);

} // namespace ExpressionParser

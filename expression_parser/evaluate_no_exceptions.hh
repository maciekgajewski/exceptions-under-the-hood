#pragma once
#include <cassert>
#include <stdexcept>
#include <string>

namespace ExpressionParser {

extern std::string _error;

namespace DetailNoExceptions {

template <typename It>
bool evaluateParenthesis(It &current, const It last, int &result);
template <typename It> int evaluateNumber(It &current, const It last);

template <typename It> bool evaluate(It &current, const It last, int &result) {
  int value = 0;
  int mply = 1;
  while (current != last) {
    // parse value

    if (*current == '(') {
      int res;
      if (!evaluateParenthesis(current, last, res))
        return false;
      value += mply * res;
    } else if (*current >= '0' && *current <= '9') {
      value += mply * evaluateNumber(current, last);
    } else {
      _error = "Error parsing value";
      return false;
    }

    // maybe parse operator
    if (current == last || *current == ')')
      break;

    if (*current == '+')
      mply = 1;
    else if (*current == '-')
      mply = -1;
    else {
      _error = "Error parsing operator";
      return false;
    }
    ++current;
  }

  result = value;
  return true;
}

template <typename It>
bool evaluateParenthesis(It &current, const It last, int &result) {

  assert(*current == '(');
  ++current;
  if (!evaluate(current, last, result))
    return false;
  if (current == last || *current != ')') {
    _error = "Missing closing parenthesis";
    return false;
  }
  ++current;
  return true;
}

template <typename It> int evaluateNumber(It &current, const It last) {

  assert(*current >= '0' && *current <= '9');

  int result = 0;
  while (true) {
    result += *current - '0';
    ++current;
    if (current == last)
      return result;
    if (*current < '0' || *current > '9')
      return result;
    result *= 10;
  }
}

} // namespace DetailNoExceptions

template <typename It>
int evaluateNoExceptions(It first, const It last, int &result) {

  if (first == last) {
    _error = "Empty expression";
    return false;
  }
  bool res = DetailNoExceptions::evaluate(first, last, result);
  if (res) {
    if (first != last) {
      _error = "Unmatched ) at the end of expression";
      return false;
    }
  }
  return res;
}

// evaluates expression, return true if ok. Value returned by out param, error
// in global _error;
inline bool evaluateNoExceptions(const std::string &expression, int &result) {
  return evaluateNoExceptions(expression.begin(), expression.end(), result);
}

} // namespace ExpressionParser

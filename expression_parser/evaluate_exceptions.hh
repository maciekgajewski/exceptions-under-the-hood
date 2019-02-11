#pragma once
#include <cassert>
#include <stdexcept>
#include <string>

namespace ExpressionParser {

namespace DetailExceptions {

template <typename It> int evaluateParenthesis(It &current, const It last);
template <typename It> int evaluateNumber(It &current, const It last);

template <typename It> int evaluate(It &current, const It last) {
  int value = 0;
  int mply = 1;
  while (current != last) {
    // parse value

    if (*current == '(') {
      value += mply * evaluateParenthesis(current, last);
    } else if (*current >= '0' && *current <= '9') {
      value += mply * evaluateNumber(current, last);
    } else {
      throw std::runtime_error("Error parsing value");
    }

    // maybe parse operator
    if (current == last || *current == ')')
      break;

    if (*current == '+')
      mply = 1;
    else if (*current == '-')
      mply = -1;
    else
      throw std::runtime_error("Error parsing operator");
    ++current;
  }

  return value;
}

template <typename It> int evaluateParenthesis(It &current, const It last) {

  assert(*current == '(');
  ++current;
  int value = evaluate(current, last);
  if (current == last || *current != ')')
    throw std::runtime_error("Missing closing parenthesis");
  ++current;
  return value;
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

} // namespace DetailExceptions

template <typename It> int evaluateExceptions(It first, const It last) {

  if (first == last)
    throw std::runtime_error("Empty expression");
  int value = DetailExceptions::evaluate(first, last);
  if (first != last)
    throw std::runtime_error("Unmatched ) at the end of expression");
  return value;
}

// evaluates expression, throws on error
inline int evaluateExceptions(const std::string &expression) {
  return evaluateExceptions(expression.begin(), expression.end());
}

} // namespace ExpressionParser

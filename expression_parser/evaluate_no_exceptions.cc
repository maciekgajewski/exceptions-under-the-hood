#include "evaluate_no_exceptions.hh"

namespace ExpressionParser {
std::string _error;
namespace DetailNoExceptions {

static bool evaluateParenthesis(const char *&current, const char *last,
                                int &result);
static int evaluateNumber(const char *&current, const char *last);

static bool evaluate(const char *&current, const char *last, int &result) {
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

static bool evaluateParenthesis(const char *&current, const char *last,
                                int &result) {

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

static int evaluateNumber(const char *&current, const char *last) {

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

static int evaluateNoExceptions(const char *first, const char *last,
                                int &result) {

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
bool evaluateNoExceptions(const std::string &expression, int &result) {
  return evaluateNoExceptions(expression.data(),
                              expression.data() + expression.length(), result);
}
} // namespace ExpressionParser

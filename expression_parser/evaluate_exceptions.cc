#include "evaluate_exceptions.hh"
namespace ExpressionParser {
namespace DetailExceptions {

static int evaluateParenthesis(const char *&current, const char *last);
static int evaluateNumber(const char *&current, const char *last);

static int evaluate(const char *&current, const char *last) {
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

static int evaluateParenthesis(const char *&current, const char *last) {

  assert(*current == '(');
  ++current;
  int value = evaluate(current, last);
  if (current == last || *current != ')')
    throw std::runtime_error("Missing closing parenthesis");
  ++current;
  return value;
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

} // namespace DetailExceptions

static int evaluateExceptions(const char *first, const char *last) {

  if (first == last)
    throw std::runtime_error("Empty expression");
  int value = DetailExceptions::evaluate(first, last);
  if (first != last)
    throw std::runtime_error("Unmatched ) at the end of expression");
  return value;
}

// evaluates expression, throws on error
int evaluateExceptions(const std::string &expression) {
  return evaluateExceptions(expression.data(),
                            expression.data() + expression.length());
}
} // namespace ExpressionParser

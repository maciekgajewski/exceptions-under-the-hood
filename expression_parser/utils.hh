#pragma once
#include <string>
#include <vector>

namespace ExpressionParser {

struct Expression {
  std::string expression;
  int result;
};

// Loads expressions from file gneerated by generate.py
std::vector<Expression> loadFromFile(const std::string &path);

} // namespace ExpressionParser

#include "utils.hh"

#include <fstream>

namespace ExpressionParser {

std::vector<Expression> loadFromFile(const std::string &path) {

  std::vector<Expression> result;

  std::ifstream file(path);
  if (!file.good())
    throw std::runtime_error("loadFromFile: Failed ot open expressions file: " +
                             path);

  for (std::string line; std::getline(file, line);) {
    auto pos = line.find('=');
    if (pos == std::string::npos || pos == 0 || pos == line.size() - 1)
      throw std::runtime_error("loadFromFile: invalid line: " + line);

    std::string expression(line.begin(), line.begin() + pos);
    std::string value(line.begin() + pos + 1, line.end());

    result.push_back(Expression{expression, std::stoi(value)});
  }
  return result;
}

} // namespace ExpressionParser

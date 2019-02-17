#include <benchmark/benchmark.h>

#include "evaluate_exceptions.hh"
#include "evaluate_no_exceptions.hh"
#include "utils.hh"

namespace ExpressionParser {
namespace Benchmarks {

static std::vector<Expression> expressions_valid;
static std::vector<Expression> expressions_invalid_1;

static void NoExceptions(benchmark::State &state,
                         const std::vector<Expression> &expressions) {
  std::size_t idx = 0;
  for (auto _ : state) {
    int result;
    ::benchmark::DoNotOptimize(
        evaluateNoExceptions(expressions[idx].expression, result));
    ++idx;
    if (idx == expressions.size())
      idx = 0;
  }
}

static void Exceptions(benchmark::State &state,
                       const std::vector<Expression> &expressions) {
  std::size_t idx = 0;
  for (auto _ : state) {
    try {
      ::benchmark::DoNotOptimize(
          evaluateExceptions(expressions[idx].expression));
    } catch (...) {
    }
    ++idx;
    if (idx == expressions.size())
      idx = 0;
  }
}

BENCHMARK_CAPTURE(NoExceptions, valid, expressions_valid);
BENCHMARK_CAPTURE(Exceptions, valid, expressions_valid);

BENCHMARK_CAPTURE(NoExceptions, invalid1, expressions_invalid_1);
BENCHMARK_CAPTURE(Exceptions, invalid1, expressions_invalid_1);

} // namespace Benchmarks
} // namespace ExpressionParser

int main(int argc, char **argv) {

  ::benchmark::Initialize(&argc, argv);
  if (::benchmark::ReportUnrecognizedArguments(argc, argv))
    return 1;

  // load expressions
  ExpressionParser::Benchmarks::expressions_valid =
      ExpressionParser::loadFromFile("expressions.txt");

  // create a set with 1 invalid
  ExpressionParser::Benchmarks::expressions_invalid_1 =
      ExpressionParser::Benchmarks::expressions_valid;

  auto &expression = ExpressionParser::Benchmarks::expressions_invalid_1[7];
  expression.expression[expression.expression.length() / 2] =
      'x'; // spoil the expression

  ::benchmark::RunSpecifiedBenchmarks();
}

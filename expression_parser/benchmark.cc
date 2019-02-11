#include <benchmark/benchmark.h>

#include "evaluate_exceptions.hh"
#include "evaluate_no_exceptions.hh"
#include "utils.hh"

namespace ExpressionParser {
namespace Benchmarks {

static std::vector<Expression> expressions;

static void NoExceptionsValid(benchmark::State &state) {
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

static void ExceptionsValid(benchmark::State &state) {
  std::size_t idx = 0;
  for (auto _ : state) {
    ::benchmark::DoNotOptimize(evaluateExceptions(expressions[idx].expression));
    ++idx;
    if (idx == expressions.size())
      idx = 0;
  }
}

BENCHMARK(NoExceptionsValid);
BENCHMARK(ExceptionsValid);

} // namespace Benchmarks
} // namespace ExpressionParser

int main(int argc, char **argv) {

  ::benchmark::Initialize(&argc, argv);
  if (::benchmark::ReportUnrecognizedArguments(argc, argv))
    return 1;
  ExpressionParser::Benchmarks::expressions =
      ExpressionParser::loadFromFile("expressions.txt");
  ::benchmark::RunSpecifiedBenchmarks();
}

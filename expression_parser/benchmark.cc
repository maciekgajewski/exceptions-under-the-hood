
#include "evaluate_exceptions.hh"
#include "evaluate_no_exceptions.hh"
#include "utils.hh"

#include <benchmark/benchmark.h>

#include <iostream>

namespace ExpressionParser {
namespace Benchmarks {

static std::vector<Expression> expressions_valid;
static std::vector<Expression> expressions_invalid_1;

std::size_t global;
static void warmup(const std::vector<Expression> &expressions) {
  for (const Expression &e : expressions)
    for (char c : e.expression)
      global += c;
}

static void NullWholeSet(benchmark::State &state,
                         const std::vector<Expression> &expressions) {

  for (auto _ : state) {
    for (const Expression &e : expressions) {
      ::benchmark::DoNotOptimize(e);
    }
  }
}

static void NoExceptionsWholeSet(benchmark::State &state,
                                 const std::vector<Expression> &expressions) {
  std::size_t errors = 0;
  for (auto _ : state) {
    for (const Expression &e : expressions) {
      int result;
      bool success;
      ::benchmark::DoNotOptimize(
          success = evaluateNoExceptions(e.expression, result));
      if (!success)
        errors++;
    }
  }
  state.counters["errors"] = errors;
}

static void ExceptionsWholeSet(benchmark::State &state,
                               const std::vector<Expression> &expressions) {
  std::size_t errors = 0;
  for (auto _ : state) {
    for (const Expression &e : expressions) {
      try {
        ::benchmark::DoNotOptimize(evaluateExceptions(e.expression));
      } catch (...) {
        errors++;
      }
    }
  }
  state.counters["errors"] = errors;
}

static void NoExceptions(benchmark::State &state,
                         const std::vector<Expression> &expressions) {

  std::size_t idx = 0;
  std::size_t errors = 0;
  for (auto _ : state) {
    int result;
    bool success;
    ::benchmark::DoNotOptimize(
        success = evaluateNoExceptions(expressions[idx].expression, result));
    ++idx;
    if (idx == expressions.size())
      idx = 0;
    if (!success)
      errors++;
  }
  state.counters["errors"] = errors;
}

static void Exceptions(benchmark::State &state,
                       const std::vector<Expression> &expressions) {

  std::size_t errors = 0;
  std::size_t idx = 0;
  for (auto _ : state) {
    try {
      ::benchmark::DoNotOptimize(
          evaluateExceptions(expressions[idx].expression));
    } catch (...) {
      errors++;
    }
    ++idx;
    if (idx == expressions.size())
      idx = 0;
  }
  state.counters["errors"] = errors;
}

BENCHMARK_CAPTURE(NullWholeSet, valid, expressions_valid);
BENCHMARK_CAPTURE(NoExceptionsWholeSet, valid, expressions_valid);
BENCHMARK_CAPTURE(ExceptionsWholeSet, valid, expressions_valid);

BENCHMARK_CAPTURE(NoExceptionsWholeSet, invalid1, expressions_invalid_1);
BENCHMARK_CAPTURE(ExceptionsWholeSet, invalid1, expressions_invalid_1);

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

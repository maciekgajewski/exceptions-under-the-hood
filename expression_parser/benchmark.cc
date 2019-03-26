
#include "evaluate_exceptions.hh"
#include "evaluate_no_exceptions.hh"
#include "utils.hh"

#include <benchmark/benchmark.h>
#include <papipp.h>

#include <iostream>

//#define EPB_PAPI

namespace ExpressionParser {
namespace Benchmarks {

static std::vector<Expression> expressions_valid;
static std::vector<Expression> expressions_invalid_1;

using papi_event_set =
    papi::event_set<PAPI_TOT_INS, PAPI_TOT_CYC, PAPI_BR_MSP, PAPI_L1_DCM>;

static void printCounters(const papi_event_set &events,
                          std::uint64_t iterations) {
  std::cout << "==== counters for " << iterations << " iterations ==== \n";
  std::cout << events.get<PAPI_TOT_INS>().counter() /
                   double(events.get<PAPI_TOT_CYC>().counter())
            << " instr per cycle\n";
  std::cout << events.get<PAPI_TOT_INS>().counter() / double(iterations)
            << " instructions\n";
  std::cout << events.get<PAPI_L1_DCM>().counter() / double(iterations)
            << " l1 cache misses\n"
            << events.get<PAPI_BR_MSP>().counter() / double(iterations)
            << " branch misses\n"
            << std::endl;
}

std::size_t global;
static void warmup(const std::vector<Expression> &expressions) {
  for (const Expression &e : expressions)
    for (char c : e.expression)
      global += c;
}

static void NullWholeSet(benchmark::State &state,
                         const std::vector<Expression> &expressions) {
#ifdef EPB_PAPI
  papi_event_set events;
  events.start_counters();
  warmup(expressions);
  asm volatile("" ::"r,m"(expressions.data()) : "memory");
  events.reset_counters();
#endif

  for (auto _ : state) {
    for (const Expression &e : expressions) {
      ::benchmark::DoNotOptimize(e);
    }
  }

#ifdef EPB_PAPI
  events.stop_counters();
  if (state.iterations() > 10000000)
    printCounters(events, state.iterations());
#endif
}

static void NoExceptionsWholeSet(benchmark::State &state,
                                 const std::vector<Expression> &expressions) {
// papi - perf counters
#ifdef EPB_PAPI
  papi_event_set events;
  events.start_counters();
  warmup(expressions);
  asm volatile("" ::"r,m"(expressions.data()) : "memory");
  events.reset_counters();
#endif

  for (auto _ : state) {
    for (const Expression &e : expressions) {
      int result;
      ::benchmark::DoNotOptimize(evaluateNoExceptions(e.expression, result));
    }
  }

#ifdef EPB_PAPI
  events.stop_counters();
  if (state.iterations() > 10000)
    printCounters(events, state.iterations());
#endif
}

static void ExceptionsWholeSet(benchmark::State &state,
                               const std::vector<Expression> &expressions) {
// papi - perf counters
#ifdef EPB_PAPI
  papi_event_set events;
  events.start_counters();
  warmup(expressions);
  asm volatile("" ::"r,m"(expressions.data()) : "memory");
  events.reset_counters();
#endif

  for (auto _ : state) {
    for (const Expression &e : expressions) {
      try {
        ::benchmark::DoNotOptimize(evaluateExceptions(e.expression));
      } catch (...) {
      }
    }
  }

#ifdef EPB_PAPI
  events.stop_counters();
  if (state.iterations() > 10000)
    printCounters(events, state.iterations());
#endif
}

static void NoExceptions(benchmark::State &state,
                         const std::vector<Expression> &expressions) {
// papi - perf counters
#ifdef EPB_PAPI
  papi_event_set events;
  events.start_counters();
#endif

  std::size_t idx = 0;
  for (auto _ : state) {
    int result;
    ::benchmark::DoNotOptimize(
        evaluateNoExceptions(expressions[idx].expression, result));
    ++idx;
    if (idx == expressions.size())
      idx = 0;
  }

#ifdef EPB_PAPI
  events.stop_counters();
  if (state.iterations() > 1000000)
    printCounters(events, state.iterations());
#endif
}

static void Exceptions(benchmark::State &state,
                       const std::vector<Expression> &expressions) {
#ifdef EPB_PAPI
  papi_event_set events;
  events.start_counters();
#endif

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

#ifdef EPB_PAPI
  events.stop_counters();
  if (state.iterations() > 1000000)
    printCounters(events, state.iterations());
#endif
}

BENCHMARK_CAPTURE(NullWholeSet, valid, expressions_valid);
BENCHMARK_CAPTURE(NoExceptionsWholeSet, valid, expressions_valid);
BENCHMARK_CAPTURE(ExceptionsWholeSet, valid, expressions_valid);

// BENCHMARK_CAPTURE(NoExceptionsWholeSet, invalid1, expressions_invalid_1);
// BENCHMARK_CAPTURE(ExceptionsWholeSet, invalid1, expressions_invalid_1);

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

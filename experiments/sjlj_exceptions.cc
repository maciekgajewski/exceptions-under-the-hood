#include "sjlj_exceptions.hh"

#include <exception>
#include <iostream>

namespace sjlj {

thread_local ExceptionData *current_scope = nullptr;

void before_try(ExceptionData &ed) {
  ed.prev = current_scope;
  current_scope = &ed;
}

void end_catch(ExceptionData &ed) { current_scope = ed.prev; }

void _throw(int error_code) {
  if (!current_scope) {
    std::cerr << "_throw w/o catch" << std::endl;
    std::abort();
  }

  current_scope->error_code = error_code;
  std::longjmp(current_scope->ctx, 1);
}

void continue_unwinding(ExceptionData &ed) {
  current_scope = ed.prev;
  if (!current_scope) {
    std::cerr << "_throw w/o catch" << std::endl;
    std::abort();
  }

  current_scope->error_code = ed.error_code;
  std::longjmp(current_scope->ctx, 1);
}

} // namespace sjlj

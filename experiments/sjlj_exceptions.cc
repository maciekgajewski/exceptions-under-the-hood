#include "sjlj_exceptions.hh"

#include <exception>
#include <iostream>

namespace sjljexceptions {

static thread_local ExceptionData *current_scope = nullptr;

bool _try(ExceptionData &ed) {
  ed.prev = current_scope;
  if (setjmp(ed.ctx) == 0) {
    return true;
  } else {
    current_scope = ed.prev;
    return false;
  }
}

void _throw(int error_code) {
  if (!current_scope) {
    std::cerr << "_throw w/o catch" << std::endl;
    std::abort();
  }

  current_scope->error_code = error_code;
  std::longjmp(current_scope->ctx, 1);
}

} // namespace sjljexceptions

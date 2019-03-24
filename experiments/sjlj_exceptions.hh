#pragma once

#include <csetjmp>

namespace sjlj {

struct ExceptionData {
  std::jmp_buf ctx;
  int error_code;
  ExceptionData *prev;
};

extern thread_local ExceptionData *current_scope;

// starts try-block. ed should be a local variable
void before_try(ExceptionData &ed);

// end catch block
void end_catch(ExceptionData &ed);

// continues stack unwinding
void continue_unwinding(ExceptionData &ed);

void _throw(int error_code);
} // namespace sjlj

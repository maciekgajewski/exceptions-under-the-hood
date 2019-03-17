#pragma once

#include <csetjmp>

namespace sjljexceptions {

struct ExceptionData {
  std::jmp_buf ctx;
  int error_code;
  ExceptionData* prev;
};

// starts try-block. ed should be a local variable
// returns true on the first call, false on exception
bool _try(ExceptionData& ed);

void _throw(int error_code);
}

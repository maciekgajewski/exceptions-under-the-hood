#include "sjlj_exceptions.hh"
#include "widget.hh"

#include <iostream>
#include <string>

void thrower(int value) {
  if (value > 100) {
    int error_code = value;
    std::cout << "Throwing " << error_code << std::endl;
    sjlj::_throw(error_code);
  } else {
    std::cout << "not throwing" << std::endl;
  }
}

void bystander(int value) {
  Widget w("bystander");

  sjlj::ExceptionData exception;
  sjlj::before_try(exception);

  if (!setjmp(sjlj::current_scope->ctx)) {
    thrower(value);
  } else {
    w.~Widget();
    sjlj::continue_unwinding(exception);
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Param required" << std::endl;
    std::exit(1);
  }
  int value;
  try {
    value = std::stoi(argv[1]);
  } catch (...) {
    std::cerr << "Param must be a number" << std::endl;
    std::exit(2);
  }

  sjlj::ExceptionData exception;
  sjlj::before_try(exception);
  if (!setjmp(sjlj::current_scope->ctx)) {
    std::cout << "Calling bystander, value=" << value << std::endl;
    bystander(value);

    std::cout << "Bystander exited cleanly" << std::endl;
  } else {
    std::cout << "Exception caught. error code: " << exception.error_code
              << std::endl;

    sjlj::end_catch(exception);
  }

  std::cout << "The end" << std::endl;
}

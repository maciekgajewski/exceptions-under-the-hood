#include "sjlj_exceptions.hh"
#include "widget.hh"

#include <iostream>
#include <string>

void thrower(int value) {
  if (value > 100) {
    int error_code = value;
    std::cout << "Throwing " << error_code << std::endl;
    sjljexceptions::_throw(error_code);
  } else {
    std::cout << "not throwing" << std::endl;
  }
}

void bystander(int value) {
  Widget w("bystabder");
  thrower(value);
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

  sjljexceptions::ExceptionData exception;
  if (sjljexceptions::_try(exception)) {
    std::cout << "Calling bystander, value=" << value << std::endl;
    bystander(value);
    std::cout << "Bysrtabder exited cleanly" << std::endl;
  } else {
    std::cout << "Exception caught. error code: " << exception.error_code
              << std::endl;
  }

  std::cout << "The end" << std::endl;
}

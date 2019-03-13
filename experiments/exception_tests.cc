#include "exception_types.hh"

#include <iostream>

void catch_by_value() {

  try {
    std::cout << " # Will catch by value" << std::endl;
    throw_exception("catch by value");
  } catch (Exception e) {
    std::cout << "Exception caught, &e=" << &e << std::endl;
  }
}

void catch_by_ref() {
  try {
    std::cout << " # Will catch by reference" << std::endl;
    throw_exception("catch by ref");
  } catch (Exception &e) {
    std::cout << "Exception caught, &e=" << &e << std::endl;
  }
}

void catch_by_const_ref() {
  try {
    std::cout << " # Will catch by const reference" << std::endl;
    throw_exception("catch by const ref");
  } catch (const Exception &e) {
    std::cout << "Exception caught, &e=" << &e << std::endl;
  }
}

int main() {

  const std::type_info &ti = typeid(Exception);

  std::cout << "Startign tests, type info at " << &ti << std::endl;

  catch_by_value();
  catch_by_ref();
  catch_by_const_ref();
}

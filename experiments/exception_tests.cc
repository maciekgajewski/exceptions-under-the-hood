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
    int var = 5;
    e.report(&var);
  } catch (...) {
    std::cout << "... caught" << std::endl;
  }
}

void catch_by_const_ref() {
  try {
    std::cout << " # Will catch by const reference" << std::endl;
    throw_exception("catch by const ref");
  } catch (const Exception &e) {
    std::cout << "Exception caught, &e=" << &e << std::endl;
    int var = 5;
    e.report(&var);
  } catch (...) {
    std::cout << "... caught" << std::endl;
  }
}

void catch_derived_by_const_ref() {
  try {
    std::cout << " # Will catch derived by const reference" << std::endl;
    throw_derived_exception("derived by const ref");
  } catch (const Exception &e) {
    const std::type_info &ti = typeid(e);
    std::cout << "Exception caught, &e=" << &e << ", type_info=" << &ti
              << std::endl;
    int var = 6;
    e.report(&var);
  } catch (...) {
    std::cout << "... caught" << std::endl;
  }
}

void catch_derived_non_virtual_by_const_ref() {
  try {
    std::cout << " # Will catch non-virtual derived by const reference"
              << std::endl;
    throw_derived_non_virtual_exception("non virtual derived");
  } catch (const ExceptionNonVirtual &e) {
    const std::type_info &ti = typeid(e);
    std::cout << "ExceptionNonVirtual caught, &e=" << &e
              << ", type_info=" << &ti << std::endl;
  } catch (...) {
    std::cout << "... caught" << std::endl;
  }
}

void catch_std_exception() {
  try {

    std::cout << " # Will catch std exception" << std::endl;
    throw_std_exception("boo");
  } catch (const std::exception &e) {
    std::cout << "std::exception caught: " << e.what() << std::endl;
  }
}

void catch_int() {
  try {
    std::cout << " # Will catch int" << std::endl;
    throw_int(77);
  } /*catch (int x) {
    std::cout << "int caught: " << x << std::endl;
  }*/
  catch (...) {
    std::cout << "... caught" << std::endl;
  }
}

int main() {

  std::cout << "Starting tests" << std::endl;

  const std::type_info &ti = typeid(Exception);
  std::cout << " - type info of Exception at " << &ti << std::endl;
  const std::type_info &tinv = typeid(ExceptionNonVirtual);
  std::cout << " - type info of ExceptionNonVirtual at " << &tinv << std::endl;

  catch_by_value();
  catch_by_ref();
  catch_by_const_ref();
  catch_derived_by_const_ref();
  catch_derived_non_virtual_by_const_ref();
  catch_std_exception();
  catch_int();
}

#include <exception>
#include <iostream>

#ifndef _MSC_VER
#include <cxxabi.h>
#endif

void my_terminate() {
  std::cout << "My terminate" << std::endl;
  int ue = std::uncaught_exceptions();
  if (ue == 0) {
    std::cout << "Terminate w/o active exceptions" << std::endl;
    // std::abort();
  }
  std::cout << "Active exceptions : " << ue << std::endl;

  std::exception_ptr p = std::current_exception();
  if (p) {
    std::cout << "current_exception is not null" << std::endl;
  } else {
    std::cout << "current_exception is null" << std::endl;
  }

#ifndef _MSC_VER

  std::type_info *currentExceptionType =
      __cxxabiv1::__cxa_current_exception_type();

  if (currentExceptionType) {

    char *demangled = __cxxabiv1::__cxa_demangle(currentExceptionType->name(),
                                                 nullptr, nullptr, nullptr);

    std::cout << "Current exception type: " << demangled << std::endl;
    std::free(demangled);
  }

#endif

  std::abort();
}

void evil_terminate() {
  std::cout << "Evil terminate" << std::endl;
  my_terminate();
}

struct ExceptionDetector {

  ExceptionDetector() { exceptions_ = std::uncaught_exceptions(); }

  ~ExceptionDetector() noexcept(false) {
    if (std::uncaught_exceptions() == exceptions_) {
      std::cout << "Destroyed in normal process" << std::endl;
    } else {
      std::cout << "Destroyed because of exception" << std::endl;
      std::set_terminate(evil_terminate);
      // throw 7;
    }
  }

  int exceptions_;
};

void maybe_throw(bool t) {
  if (t)
    throw std::runtime_error("Yo");
}

void fun(bool t) {
  ExceptionDetector detector;
  maybe_throw(t);
}

int main() {

  std::set_terminate(my_terminate);

  try {

    std::cout << "calling function, not throwing" << std::endl;
    fun(false);

    std::cout << "calling function, throwing" << std::endl;
    fun(true);

  } catch (const std::exception &e) {
    std::cout << "Exception caught : " << e.what() << std::endl;
    throw;
  }
  std::cout << "Clean exit" << std::endl;
}

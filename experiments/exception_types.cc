#include "exception_types.hh"

#include <iostream>

Exception::Exception(const char *msg) : msg_(msg) {

  std::cout << "Exception(" << msg_ << ") created, this=" << this << std::endl;
}

Exception::Exception(const Exception &o) : msg_(o.msg_) {
  std::cout << "Exception(" << msg_ << ") copied, this=" << this
            << ", copied from=" << &o << std::endl;
}

Exception::~Exception() {
  std::cout << "Exception(" << msg_ << ") destroyed, this=" << this
            << std::endl;
}

void Exception::report(int *ptr) const {
  std::cout << "Exception::report, msg=" << msg_ << ", ptr=" << ptr
            << std::endl;
}

// derived exception type
class DerivedException : public Exception {
public:
  DerivedException(const char *msg) : Exception(msg) {
    std::cout << "DerivedException(" << msg_ << ") created, this=" << this
              << std::endl;
  }
  DerivedException(const DerivedException &o) : Exception(o) {
    std::cout << "DerivedException(" << msg_ << ") copied, this=" << this
              << ", copied from=" << &o << std::endl;
  }
  ~DerivedException() override {
    std::cout << "DerivedException(" << msg_ << ") destroyed, this=" << this
              << std::endl;
  }
  void report(int *ptr) const override {
    std::cout << "DerivedException::report, msg=" << msg_ << ", ptr=" << ptr
              << std::endl;
  }
};

void throw_exception(const char *msg) {

  const std::type_info &ti = typeid(Exception);
  std::cout << "Throwing Exception. Stack now is: " << &msg << ", type_info at "
            << &ti << std::endl;
  throw Exception(msg);
}

void throw_derived_exception(const char *msg) {

  const std::type_info &ti = typeid(DerivedException);
  std::cout << "Throwing DerivedException. Stack now is: " << &msg
            << ", type_info at " << &ti << std::endl;
  throw DerivedException(msg);
}

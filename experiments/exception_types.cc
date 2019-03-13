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

void throw_exception(const char *msg) {

  const std::type_info &ti = typeid(Exception);
  std::cout << "Throwing exception. Stack now is: " << &msg << ", type_info at "
            << &ti << std::endl;
  throw Exception(msg);
}

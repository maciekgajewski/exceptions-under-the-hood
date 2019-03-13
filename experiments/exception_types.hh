#pragma once

class Exception {
public:
  Exception(const char* msg);
  Exception(const Exception&);
  ~Exception();

private:

  const char* msg_;
};

void throw_exception(const char* msg);

#pragma once

class Exception {
public:
  Exception(const char *msg);
  Exception(const Exception &);

  virtual void report(int *ptr) const;

  virtual ~Exception();

protected:
  const char *msg_;
};

void throw_exception(const char *msg);
void throw_derived_exception(const char *msg);

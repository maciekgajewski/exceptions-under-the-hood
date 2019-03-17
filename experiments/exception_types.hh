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

class ExceptionNonVirtual {
public:
  ExceptionNonVirtual(const char *msg);
  ExceptionNonVirtual(const ExceptionNonVirtual &);

  ~ExceptionNonVirtual();

protected:
  const char *msg_;
};

void throw_exception(const char *msg);
void throw_derived_exception(const char *msg);
void throw_derived_non_virtual_exception(const char *msg);
void throw_std_exception(const char *msg);
void throw_int(int x);

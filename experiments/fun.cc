#include "fun.hh"

#include <iostream>

void foo(bool t) {
  std::cout << "foo" << std::endl;
  if (t)
    throw 1;
}
void bar(bool t) {
  std::cout << "bar" << std::endl;
  if (t)
    throw 2;
};

static void fn(bool t) noexcept { foo(t); }

void foo_noexcept(bool t) {
  std::cout << "foo_noexcept" << std::endl;
  if (t)
    fn(t);
};



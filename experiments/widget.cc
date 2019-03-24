#include "widget.hh"

#include <iostream>

Widget::Widget(const char *name) : name_(name) {
  std::cout << "Widget(" << name_ << ") created" << std::endl;
}

Widget::~Widget() {
  std::cout << "Widget(" << name_ << ") destroyed" << std::endl;
}

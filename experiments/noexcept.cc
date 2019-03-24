#include <iostream>

void bar(bool t);

int main(int argc, char **argv) {

  try {
    bar(argc > 0);
  } catch (int x) {
    std::cout << "caught: " << x << std::endl;
  }
}

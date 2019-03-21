#include <iostream>
#include <typeinfo>

int main() {
  std::cout << "int: " << typeid(int).name() << std::endl;
  std::cout << "int*: " << typeid(int *).name() << std::endl;
  std::cout << "int**: " << typeid(int **).name() << std::endl;
  std::cout << "const int: " << typeid(const int).name() << std::endl;
  std::cout << "int&: " << typeid(int &).name() << std::endl;
}

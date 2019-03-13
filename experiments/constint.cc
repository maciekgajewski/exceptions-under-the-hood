#include <iostream>

void fun() { throw(const int) 7; }

void catch_int_only() {
  try {
    std::cout << "will catch 'int' and '...'" << std::endl;
    fun();
  } catch (int) {
    std::cout << "int caught" << std::endl;
  } catch (...) {
    std::cout << "... caught" << std::endl;
  }
}

void catch_int_const() {
  try {
    std::cout << "will catch 'int', 'const int' and '...'" << std::endl;
    fun();
  } catch (int) {
    std::cout << "int caught" << std::endl;
  } catch (const int) {
    std::cout << "const int caught" << std::endl;
  } catch (...) {
    std::cout << "... caught" << std::endl;
  }
}

void catch_const_int() {
  try {
    std::cout << "will catch 'const int', 'int' and '...'" << std::endl;
    fun();
  } catch (const int) {
    std::cout << "const int caught" << std::endl;
  } catch (int) {
    std::cout << "int caught" << std::endl;
  } catch (...) {
    std::cout << "... caught" << std::endl;
  }
}

int main(int argc, char **argv) {
  std::cout << "constint" << std::endl;
  catch_int_only();
  catch_int_const();
  catch_const_int();

  const std::type_info &tii = typeid(int);
  const std::type_info &tici = typeid(const int);

  std::cout << "type_info of int: " << &tii << std::endl;
  std::cout << "type_info of const int: " << &tici << std::endl;
}

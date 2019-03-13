#include <iostream>

void fun()
{
    throw 7u;
}

void catch_int_only()
{
    try
    {
        std::cout << "will catch 'int' and '...'" << std::endl;
        fun();
    }
    catch (int)
    {
        std::cout << "int caught" << std::endl;
    }
    catch (...)
    {
        std::cout << "... caught" << std::endl;
    }
}

void catch_int_unsigned()
{
    try
    {
        std::cout << "will catch 'int' and '...'" << std::endl;
        fun();
    }
    catch (unsigned int)
    {
        std::cout << "unsigned int caught" << std::endl;
    }
    catch (int)
    {
        std::cout << "int caught" << std::endl;
    }
    catch (...)
    {
        std::cout << "... caught" << std::endl;
    }
}

int main(int argc, char **argv)
{
    std::cout << "int2uint" << std::endl;
    catch_int_only();
    catch_int_unsigned();
}
#include <iostream>

void fun()
{
    throw 7u;
}

int main(int argc, char **argv)
{
    try
    {
        fun();
    }
    catch (int)
    {
        std::cout << "int caught" << std::endl;
    } /*
    catch (unsigned)
    {
        std::cout << "unsigned caught" << std::endl;
    }*/
    catch (...)
    {
        std::cout << "... caught" << std::endl;
    }
}
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif // _WIN32
#include <cstddef>
#include <iostream>
#include <string>
#include "any.hpp";

int main(int argc, char* argv[])
{
    any ai = make_any<int>(1);
    int* i = any_cast<int>(&ai);
    std::string* s = any_cast<std::string>(&ai);
    if (i != NULL)
    {
        std::cout << *i << std::endl;
    }
    if (s != NULL)
    {
        std::cout << *s << std::endl;
    }
    return 0;
}

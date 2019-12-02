#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif // _WIN32
#include <cstddef>
#include <iostream>
#include <string>
#include "any.hpp";

int main(int argc, char* argv[])
{
    auto ai = make_any<int>(1);
    auto i = any_cast<int>(&ai);
    auto s = any_cast<std::string>(&ai);
    if (i != nullptr)
    {
        std::cout << *i << std::endl;
    }
    if (s != nullptr)
    {
        std::cout << *s << std::endl;
    }
    auto ai2(ai);
    auto i2 = any_cast<int>(&ai2);
    if (i2 != nullptr)
    {
        std::cout << *i2 << std::endl;
    }
    any a3;
    std::cout << a3.has_value() << std::endl;
    any a4(4);
    auto i4 = any_cast<int>(&a4);
    if (i4 != nullptr)
    {
        std::cout << *i4 << std::endl;
    }
    return 0;
}

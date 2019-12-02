#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif // _WIN32
#include <cstddef>
#include <iostream>
#include <string>
#include "any.hpp"

template<typename T>
void test(const std::string& str, const any& value)
{
    auto i = any_cast<T>(&value);
    if (i != nullptr)
    {
        std::cout << str << ": " << *i << std::endl;
    }
    else
    {
        std::cout << str << ": " << "null" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    auto a1 = make_any<int>(1);
    test<int>("a1", a1);

    test<std::string>("a1", a1);

    auto a2(a1);
    test<int>("a2", a2);

    any a3;
    test<int>("a3", a3);

    any a4(4);
    test<int>("a4", a4);

    any a5;
    a5 = 5;
    test<int>("a5", a5);

    any a6;
    a6 = a5;
    test<int>("a6", a6);

    any a7;
    a7 = 6;
    auto i7 = a7.emplace(7);
    test<int>("a7", a7);
    std::cout << "i7: " << i7 << std::endl;
    return 0;
}

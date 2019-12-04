#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif // _WIN32
#include <cstddef>
#include <iostream>
#include <string>
#include "any.hpp"

class Object
{
public:
    Object()
    {
    }

    Object(int t1)
    {
        data += t1;
    }

    Object(int t1, int t2)
    {
        data += t1;
        data += t2;
    }

    Object(int t1, int t2, int t3)
    {
        data += t1;
        data += t2;
        data += t3;
    }

    Object(int t1, int t2, int t3, int t4)
    {
        data += t1;
        data += t2;
        data += t3;
        data += t4;
    }

    Object(int t1, int t2, int t3, int t4, int t5)
    {
        data += t1;
        data += t2;
        data += t3;
        data += t4;
        data += t5;
    }

    Object(int t1, int t2, int t3, int t4, int t5, int t6)
    {
        data += t1;
        data += t2;
        data += t3;
        data += t4;
        data += t5;
        data += t6;
    }

    Object(int t1, int t2, int t3, int t4, int t5, int t6, int t7)
    {
        data += t1;
        data += t2;
        data += t3;
        data += t4;
        data += t5;
        data += t6;
        data += t7;
    }

    ~Object()
    {
    }

    friend std::ostream& operator<<(std::ostream& os, Object& obj);
    friend std::ostream& operator<<(std::ostream& os, const Object& obj);

private:
    int data = 0;
};

std::ostream& operator<<(std::ostream& os, Object& obj)
{
    return os << obj.data;
}

std::ostream& operator<<(std::ostream& os, const Object& obj)
{
    return os << obj.data;
}

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
    Object obj;
    std::cout << obj << std::endl;

    auto o1 = make_any<Object>(1);
    test<Object>("o1", o1);

    auto o2 = make_any<Object>(1, 1);
    test<Object>("o2", o2);
    o2.emplace<Object>(1, 2);
    test<Object>("o2", o2);

    auto o3 = make_any<Object>(1, 2);
    auto io3 = any_cast<Object>(o3);

    auto o4 = make_any<Object>(1, 1, 1);
    test<Object>("o4", o4);

    //auto o5 = make_any<Object>(1, 1, 1, 1);
    //test<Object>("o5", o5);

    auto o5 = make_any<Object>(1, 1, 1, 1, 1);
    test<Object>("o5", o5);

    auto o6 = make_any<Object>(1, 1, 1, 1, 1, 1);
    test<Object>("o6", o6);

    auto o7 = make_any<Object>(1, 1, 1, 1, 1, 1, 1);
    test<Object>("o7", o7);
    return 0;
}

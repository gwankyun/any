#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
//#define ANY_HAS_CXX_11 0
//#define ANY_HAS_CXX_14 0
//#define ANY_HAS_CXX_17 0
#include <any.hpp>
#include <initializer_list>

using namespace lite;

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

    Object(int t1, int t2, int t3, int t4, int t5, int t6, int t7, int t8)
    {
        data += t1;
        data += t2;
        data += t3;
        data += t4;
        data += t5;
        data += t6;
        data += t7;
        data += t8;
    }

    Object(int t1, int t2, int t3, int t4, int t5, int t6, int t7, int t8, int t9)
    {
        data += t1;
        data += t2;
        data += t3;
        data += t4;
        data += t5;
        data += t6;
        data += t7;
        data += t8;
        data += t9;
    }

    Object(std::initializer_list<int> il)
    {
        for (auto&& i : il)
        {
            data += i;
        }
    }

    ~Object()
    {
    }

    int data = 0;

private:
};

TEST_CASE("any_cast and make_any", "[make_any]")
{
    REQUIRE(any_cast<Object>(make_any<Object>()).data == 0);
    REQUIRE(any_cast<Object>(make_any<Object>(1)).data == 1);
    REQUIRE(any_cast<Object>(make_any<Object>(1, 1)).data == 2);
    REQUIRE(any_cast<Object>(make_any<Object>(1, 1, 1)).data == 3);
    REQUIRE(any_cast<Object>(make_any<Object>(1, 1, 1, 1)).data == 4);
    REQUIRE(any_cast<Object>(make_any<Object>(1, 1, 1, 1, 1)).data == 5);
    REQUIRE(any_cast<Object>(make_any<Object>(1, 1, 1, 1, 1, 1)).data == 6);
    REQUIRE(any_cast<Object>(make_any<Object>(1, 1, 1, 1, 1, 1, 1)).data == 7);
    REQUIRE(any_cast<Object>(make_any<Object>(1, 1, 1, 1, 1, 1, 1, 1)).data == 8);
    REQUIRE(any_cast<Object>(make_any<Object>(1, 1, 1, 1, 1, 1, 1, 1, 1)).data == 9);
}

TEST_CASE("any_cast and any::any in_place_type", "[any::any]")
{
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object))).data == 0);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), 1)).data == 1);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), 1, 1)).data == 2);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), 1, 1, 1)).data == 3);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), 1, 1, 1, 1)).data == 4);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), 1, 1, 1, 1, 1)).data == 5);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), 1, 1, 1, 1, 1, 1)).data == 6);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), 1, 1, 1, 1, 1, 1, 1)).data == 7);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), 1, 1, 1, 1, 1, 1, 1, 1)).data == 8);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), 1, 1, 1, 1, 1, 1, 1, 1, 1)).data == 9);
}

#if ANY_HAS_CXX_11
TEST_CASE("any_cast and any::any in_place_type initializer_list", "[any::any]")
{
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), { 1 })).data == 1);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), { 1, 1 })).data == 2);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), { 1, 1, 1 })).data == 3);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), { 1, 1, 1, 1 })).data == 4);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), { 1, 1, 1, 1, 1 })).data == 5);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), { 1, 1, 1, 1, 1, 1 })).data == 6);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), { 1, 1, 1, 1, 1, 1, 1 })).data == 7);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), { 1, 1, 1, 1, 1, 1, 1, 1 })).data == 8);
    REQUIRE(any_cast<Object>(any(IN_PLACE_TYPE(Object), { 1, 1, 1, 1, 1, 1, 1, 1, 1 })).data == 9);
}
#endif // ANY_HAS_CXX_11

TEST_CASE("any_cast and emplace", "[emplace]")
{
    any a = make_any<Object>(0);
    SECTION("emplace 0")
    {
        a.emplace<Object>();
        REQUIRE(any_cast<Object>(a).data == 0);
    }
    SECTION("emplace 1")
    {
        a.emplace<Object>(1);
        REQUIRE(any_cast<Object>(a).data == 1);
    }
    SECTION("emplace 2")
    {
        a.emplace<Object>(1, 1);
        REQUIRE(any_cast<Object>(a).data == 2);
    }
    SECTION("emplace 3")
    {
        a.emplace<Object>(1, 1, 1);
        REQUIRE(any_cast<Object>(a).data == 3);
    }
    SECTION("emplace 4")
    {
        a.emplace<Object>(1, 1, 1, 1);
        REQUIRE(any_cast<Object>(a).data == 4);
    }
    SECTION("emplace 5")
    {
        a.emplace<Object>(1, 1, 1, 1, 1);
        REQUIRE(any_cast<Object>(a).data == 5);
    }
    SECTION("emplace 6")
    {
        a.emplace<Object>(1, 1, 1, 1, 1, 1);
        REQUIRE(any_cast<Object>(a).data == 6);
    }
    SECTION("emplace 7")
    {
        a.emplace<Object>(1, 1, 1, 1, 1, 1, 1);
        REQUIRE(any_cast<Object>(a).data == 7);
    }
    SECTION("emplace 8")
    {
        a.emplace<Object>(1, 1, 1, 1, 1, 1, 1, 1);
        REQUIRE(any_cast<Object>(a).data == 8);
    }
    SECTION("emplace 9")
    {
        a.emplace<Object>(1, 1, 1, 1, 1, 1, 1, 1, 1);
        REQUIRE(any_cast<Object>(a).data == 9);
    }
}


#pragma once
#include <cstddef>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include "compiler_detection.h"

#ifndef ANY_ARG
#if defined(__cpp_rvalue_references)
#define ANY_ARG(T) T&&
#else
#define ANY_ARG(T) const T&
#endif // defined(__cpp_rvalue_references)
#endif // !ANY_ARG

#ifndef ANY_HAS_CXX_11
#if defined(__cpp_rvalue_references)
#define ANY_HAS_CXX_11 1
#else
#define ANY_HAS_CXX_11 0
#endif // defined(__cpp_rvalue_references)
#endif // ANY_HAS_CXX_11

#if !ANY_HAS_CXX_11
#define EXTEND1(arg_, fn_, s_) fn_(arg_, 1)
#define EXTEND2(arg_, fn_, s_) s_(fn_(arg_, 1)) fn_(arg_, 2)
#define EXTEND3(arg_, fn_, s_) s_(fn_(arg_, 1)) s_(fn_(arg_, 2)) fn_(arg_, 3)
#define EXTEND4(arg_, fn_, s_) s_(fn_(arg_, 1)) s_(fn_(arg_, 2)) s_(fn_(arg_, 3)) fn_(arg_, 4)
#define EXTEND5(arg_, fn_, s_) s_(fn_(arg_, 1)) s_(fn_(arg_, 2)) s_(fn_(arg_, 3)) s_(fn_(arg_, 4)) fn_(arg_, 5)
#define EXTEND6(arg_, fn_, s_) s_(fn_(arg_, 1)) s_(fn_(arg_, 2)) s_(fn_(arg_, 3)) s_(fn_(arg_, 4)) s_(fn_(arg_, 5)) fn_(arg_, 6)
#define EXTEND7(arg_, fn_, s_) s_(fn_(arg_, 1)) s_(fn_(arg_, 2)) s_(fn_(arg_, 3)) s_(fn_(arg_, 4)) s_(fn_(arg_, 5)) s_(fn_(arg_, 6)) fn_(arg_, 7)
#define EXTEND8(arg_, fn_, s_) s_(fn_(arg_, 1)) s_(fn_(arg_, 2)) s_(fn_(arg_, 3)) s_(fn_(arg_, 4)) s_(fn_(arg_, 5)) s_(fn_(arg_, 6)) s_(fn_(arg_, 7)) fn_(arg_, 8)
#define EXTEND9(arg_, fn_, s_) s_(fn_(arg_, 1)) s_(fn_(arg_, 2)) s_(fn_(arg_, 3)) s_(fn_(arg_, 4)) s_(fn_(arg_, 5)) s_(fn_(arg_, 6)) s_(fn_(arg_, 7)) s_(fn_(arg_, 8)) fn_(arg_, 9)

#define DEFINE(arg_) \
    arg_(1) \
    \
    arg_(2) \
    \
    arg_(3) \
    \
    arg_(4) \
    \
    arg_(5) \
    \
    arg_(6) \
    \
    arg_(7) \
    \
    arg_(8) \
    \
    arg_(9)

#define COMMA(x) x,
#define SEMICOLON(x) x;

#define EXTEND(arg_, fn_, n_, D) EXTEND##n_(arg_, fn_, D)

#define ARGUMENT(x, n) x##n

#define PARAMETER(x, n) ANY_ARG(x##n) _##x##n

#define TYPENAME(x, n) typename x##n

#define TYPENAME_N(a_, n_)  EXTEND(a_, TYPENAME, n_, COMMA)
#define PARAMETER_N(a_, n_) EXTEND(a_, PARAMETER, n_, COMMA)
#define ARGUMENT_N(a_, n_) EXTEND(a_, ARGUMENT, n_, COMMA)

#define MAKE_ANY(x) \
    template<typename T, TYPENAME_N(T, x)> \
    any make_any(PARAMETER_N(T, x)) FEATURE_NOEXCEPT \
    { \
        return any(T(ARGUMENT_N(_T, x))); \
    }

#define ANY_EMPLACE(x) \
    template<typename T, TYPENAME_N(T, x)> \
    T& emplace(PARAMETER_N(T, x)) \
    { \
        reset(); \
        *this = T(ARGUMENT_N(_T, x)); \
        return get_value<T>(); \
    }
#endif // !ANY_HAS_CXX_11

class bad_any_cast : public std::bad_cast
{
public:
    bad_any_cast() FEATURE_NOEXCEPT
    {
    }

    ~bad_any_cast()
    {
    }

    char const* what() const FEATURE_NOEXCEPT
    {
        return "bad_any_cast";
    }

private:

};

class any
{
public:

    FEATURE_CONSTEXPR any() FEATURE_NOEXCEPT : base(NULL)
    {
    }

    any(const any& other) : base(other.has_value() ? other.base->clone() : NULL)
    {
    }

#if ANY_HAS_CXX_11
    any(any&& other) FEATURE_NOEXCEPT : base(NULL)
    {
        swap(other);
    }

    template<typename T>
    explicit any(T&& value) : base(new Derived<T>(value))
    {
    }
#endif // ANY_HAS_CXX_11

    template<typename T>
    explicit any(const T& value) : base(new Derived<T>(value))
    {
    }

    any& operator=(const any& other)
    {
        if (this != &other)
        {
            reset();
            base = other.base->clone();
        }
        return *this;
    }

    template<typename T>
    any& operator=(const T& rhs)
    {
        reset();
        base = new Derived<T>(rhs);
        return *this;
    }

    ~any()
    {
        reset();
    }

    // 修改器

#if ANY_HAS_CXX_11
    template<typename T, typename ...Args>
    T& emplace(Args&& ...args)
    {
        reset();
        *this = T(std::forward<Args>(args)...);
        return get_value<T>();
    }
#else
    DEFINE(ANY_EMPLACE)
#endif // ANY_HAS_CXX_11

        void reset() FEATURE_NOEXCEPT
    {
        if (has_value())
        {
            delete base;
            base = NULL;
        }
    }

    void swap(any& other) FEATURE_NOEXCEPT
    {
        Base* temp = base;
        base = other.base;
        other.base = temp;
    }

    // 觀察器

    bool has_value() const FEATURE_NOEXCEPT
    {
        return base != NULL;
    }

    const std::type_info& type() const FEATURE_NOEXCEPT
    {
        if (has_value())
        {
            return base->type();
        }
        else
        {
            return typeid(void);
        }
    }

    template<typename T>
    friend T* any_cast(any* operand) FEATURE_NOEXCEPT;

private:
    class Base
    {
    public:
        Base() FEATURE_NOEXCEPT
        {
        }

        virtual ~Base()
        {
        }

        virtual const std::type_info& type() const FEATURE_NOEXCEPT = 0;

        virtual Base* clone() = 0;

    private:

    };

    template<typename T>
    class Derived : public Base
    {
    public:
        explicit Derived(const T& value_) : value(value_)
        {
        }

        ~Derived()
        {
        }

        const std::type_info& type() const FEATURE_NOEXCEPT FEATURE_OVERRIDE
        {
            return typeid(value);
        }

        Base* clone() FEATURE_OVERRIDE
        {
            return new Derived<T>(value);
        }

        T value;

    private:

    };

    template<typename T>
    T& get_value()
    {
        return dynamic_cast<Derived<T>*>(base)->value;
    }

    Base* base;
};

template<typename T>
T* any_cast(any* operand) FEATURE_NOEXCEPT
{
    any::Derived<T>* d = dynamic_cast<any::Derived<T>*>(operand->base);
    if (d != NULL)
    {
        return &(d->value);
    }
    else
    {
        return NULL;
    }
}

template<typename T>
const T* any_cast(const any* operand) FEATURE_NOEXCEPT
{
    return any_cast<T>(const_cast<any*>(operand));
}

template<typename T>
T any_cast(any& operand)
{
    T* d = any_cast<T>(&operand);
    if (d != NULL)
    {
        return *d;
    }
    else
    {
        throw bad_any_cast();
    }
}

template<typename T>
T any_cast(const any& operand)
{
    return any_cast<T>(const_cast<any&>(operand));
}

inline void swap(any& lhs, any& rhs) FEATURE_NOEXCEPT
{
    lhs.swap(rhs);
}

#if ANY_HAS_CXX_11
template<typename T, typename ...Args>
any make_any(Args&& ...args) FEATURE_NOEXCEPT
{
    return any(T(std::forward<Args>(args)...));
}
#else
DEFINE(MAKE_ANY)
#endif // ANY_HAS_CXX_11

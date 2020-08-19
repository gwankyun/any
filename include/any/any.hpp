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
    template<typename T>
    T& emplace(ANY_ARG(T) value)
    {
        reset();
        *this = value;
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2>
    T& emplace(ANY_ARG(T1) t1, ANY_ARG(T2) t2)
    {
        reset();
        *this = T(t1, t2);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3>
    T& emplace(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3)
    {
        reset();
        *this = T(t1, t2, t3);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4>
    T& emplace(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4)
    {
        reset();
        *this = T(t1, t2, t3, t4);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
    T& emplace(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4, ANY_ARG(T5) t5)
    {
        reset();
        *this = T(t1, t2, t3, t4, t5);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    T& emplace(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4, ANY_ARG(T5) t5, ANY_ARG(T6) t6)
    {
        reset();
        *this = T(t1, t2, t3, t4, t5, t6);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    T& emplace(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4, ANY_ARG(T5) t5, ANY_ARG(T6) t6, ANY_ARG(T7) t7)
    {
        reset();
        *this = T(t1, t2, t3, t4, t5, t6, t7);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    T& emplace(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4, ANY_ARG(T5) t5, ANY_ARG(T6) t6, ANY_ARG(T7) t7, ANY_ARG(T8) t8)
    {
        reset();
        *this = T(t1, t2, t3, t4, t5, t6, t7, t8);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    T& emplace(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4, ANY_ARG(T5) t5, ANY_ARG(T6) t6, ANY_ARG(T7) t7, ANY_ARG(T8) t8, ANY_ARG(T9) t9)
    {
        reset();
        *this = T(t1, t2, t3, t4, t5, t6, t7, t8, t9);
        return get_value<T>();
    }
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
template<typename T>
any make_any(ANY_ARG(T) value) FEATURE_NOEXCEPT
{
    return any(value);
}

template<typename T, typename T1, typename T2>
any make_any(ANY_ARG(T1) t1, ANY_ARG(T2) t2) FEATURE_NOEXCEPT
{
    return any(T(t1, t2));
}

template<typename T, typename T1, typename T2, typename T3>
any make_any(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3) FEATURE_NOEXCEPT
{
    return any(T(t1, t2, t3));
}

template<typename T, typename T1, typename T2, typename T3, typename T4>
any make_any(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4) FEATURE_NOEXCEPT
{
    return any(T(t1, t2, t3, t4));
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
any make_any(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4, ANY_ARG(T5) t5) FEATURE_NOEXCEPT
{
    return any(T(t1, t2, t3, t4, t5));
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
any make_any(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4, ANY_ARG(T5) t5, ANY_ARG(T6) t6) FEATURE_NOEXCEPT
{
    return any(T(t1, t2, t3, t4, t5, t6));
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
any make_any(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4, ANY_ARG(T5) t5, ANY_ARG(T6) t6, ANY_ARG(T7) t7) FEATURE_NOEXCEPT
{
    return any(T(t1, t2, t3, t4, t5, t6, t7));
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
any make_any(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4, ANY_ARG(T5) t5, ANY_ARG(T6) t6, ANY_ARG(T7) t7, ANY_ARG(T8) t8) FEATURE_NOEXCEPT
{
    return any(T(t1, t2, t3, t4, t5, t6, t7, t8));
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
any make_any(ANY_ARG(T1) t1, ANY_ARG(T2) t2, ANY_ARG(T3) t3, ANY_ARG(T4) t4, ANY_ARG(T5) t5, ANY_ARG(T6) t6, ANY_ARG(T7) t7, ANY_ARG(T8) t8, ANY_ARG(T9) t9) FEATURE_NOEXCEPT
{
    return any(T(t1, t2, t3, t4, t5, t6, t7, t8, t9));
}
#endif // ANY_HAS_CXX_11

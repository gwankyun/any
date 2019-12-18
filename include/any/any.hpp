#pragma once
#include <cstddef>
#include <typeinfo>
#include <type_traits>

#define NOEXCEPT

class bad_any_cast : public std::bad_cast
{
public:
    bad_any_cast()
    {
    }

    ~bad_any_cast()
    {
    }

    char const* what() const NOEXCEPT
    {
        return "bad_any_cast";
    }

private:

};

class any
{
public:

    any() NOEXCEPT : b(NULL)
    {
    }

    any(const any& other) : b(other.has_value() ? other.b->clone() : NULL)
    {
    }

    template<typename T>
    explicit any(const T& value) : b(new derived<T>(value))
    {
    }

    ~any()
    {
        reset();
    }

    void reset() NOEXCEPT
    {
        if (has_value())
        {
            delete b;
            b = NULL;
        }
    }

    bool has_value() const NOEXCEPT
    {
        return b != NULL;
    }

    any& operator=(const any& other)
    {
        if (this != &other)
        {
            reset();
            b = other.b->clone();
        }
        return *this;
    }

    template<typename T>
    any& operator=(const T& rhs)
    {
        reset();
        b = new derived<T>(rhs);
        return *this;
    }

    void swap(any& other)
    {
        base* temp = b;
        b = other.b;
        other.b = temp;
    }

    const std::type_info& type() const NOEXCEPT
    {
        if (has_value())
        {
            return b->type();
        }
        else
        {
            return typeid(void);
        }
    }

    template<typename T>
    friend T* any_cast(any* operand) NOEXCEPT;

#ifdef __cpp_variadic_templates
    template<typename T, typename ...Args>
    T& emplace(Args&& ...args)
    {
        reset();
        *this = T(std::forward<Args>(args)...);
        return get_value<T>();
    }
#else
    template<typename T>
    T& emplace(const T& value)
    {
        reset();
        *this = value;
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2>
    T& emplace(const T1& t1, const T2& t2)
    {
        reset();
        *this = T(t1, t2);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3>
    T& emplace(const T1& t1, const T2& t2, const T3& t3)
    {
        reset();
        *this = T(t1, t2, t3);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4>
    T& emplace(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
    {
        reset();
        *this = T(t1, t2, t3, t4);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
    T& emplace(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
    {
        reset();
        *this = T(t1, t2, t3, t4, t5);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    T& emplace(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6)
    {
        reset();
        *this = T(t1, t2, t3, t4, t5, t6);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    T& emplace(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7)
    {
        reset();
        *this = T(t1, t2, t3, t4, t5, t6, t7);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    T& emplace(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8)
    {
        reset();
        *this = T(t1, t2, t3, t4, t5, t6, t7, t8);
        return get_value<T>();
    }

    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    T& emplace(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9)
    {
        reset();
        *this = T(t1, t2, t3, t4, t5, t6, t7, t8, t9);
        return get_value<T>();
    }
#endif // __cpp_variadic_templates

private:
    class base
    {
    public:
        base() NOEXCEPT
        {
        }

        virtual ~base()
        {
        }

        virtual const std::type_info& type() const NOEXCEPT = 0;

        virtual base* clone() = 0;

    private:

    };

    template<typename T>
    class derived : public base
    {
    public:
        derived(const T& value_) : value(value_)
        {
        }

        ~derived()
        {
        }

        const std::type_info& type() const NOEXCEPT
        {
            return typeid(value);
        }

        base* clone()
        {
            return new derived<T>(value);
        }

        T value;

    private:

    };

    template<typename T>
    T& get_value()
    {
        return dynamic_cast<derived<T>*>(b)->value;
    }

    base* b;
};

template<typename T>
T* any_cast(any* operand) NOEXCEPT
{
    any::derived<T>* d = dynamic_cast<any::derived<T>*>(operand->b);
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
const T* any_cast(const any* operand) NOEXCEPT
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

inline void swap(any& lhs, any& rhs) NOEXCEPT
{
    lhs.swap(rhs);
}

#ifdef __cpp_variadic_templates
template<typename T, typename ...Args>
any make_any(Args&& ...args) NOEXCEPT
{
    return any(T(std::forward<Args>(args)...));
}
#else
template<typename T>
any make_any(const T& value) NOEXCEPT
{
    return any(value);
}

template<typename T, typename T1, typename T2>
any make_any(const T1& t1, const T2& t2) NOEXCEPT
{
    return any(T(t1, t2));
}

template<typename T, typename T1, typename T2, typename T3>
any make_any(const T1& t1, const T2& t2, const T3& t3) NOEXCEPT
{
    return any(T(t1, t2, t3));
}

template<typename T, typename T1, typename T2, typename T3, typename T4>
any make_any(const T1& t1, const T2& t2, const T3& t3, const T4& t4) NOEXCEPT
{
    return any(T(t1, t2, t3, t4));
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
any make_any(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5) NOEXCEPT
{
    return any(T(t1, t2, t3, t4, t5));
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
any make_any(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6) NOEXCEPT
{
    return any(T(t1, t2, t3, t4, t5, t6));
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
any make_any(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7) NOEXCEPT
{
    return any(T(t1, t2, t3, t4, t5, t6, t7));
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
any make_any(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8) NOEXCEPT
{
    return any(T(t1, t2, t3, t4, t5, t6, t7, t8));
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
any make_any(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9) NOEXCEPT
{
    return any(T(t1, t2, t3, t4, t5, t6, t7, t8, t9));
}
#endif // __cpp_variadic_templates

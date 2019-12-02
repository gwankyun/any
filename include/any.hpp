#pragma once
#include <cstddef>
#include <typeinfo>

#define NOEXCEPT

namespace Any
{
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
}

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

    any(const any& other) : b(other.b->clone())
    {
    }

    template<typename T>
    explicit any(const T& value) : b(new Any::derived<T>(value))
    {
    }

    ~any()
    {
        reset();
    }

    void reset() NOEXCEPT
    {
        if (b != NULL)
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
            b = other.b->clone();
        }
        return *this;
    }

    void swap(any& other)
    {
        auto temp = b;
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

    template<typename T>
    friend any make_any(const T& value) NOEXCEPT;

private:
    Any::base* b;
};

template<typename T>
any make_any(const T& value) NOEXCEPT
{
    Any::base* b = new Any::derived<T>(value);
    any a;
    a.b = b;
    return a;
}

template<typename T>
T* any_cast(any* operand) NOEXCEPT
{
    Any::derived<T>* d = dynamic_cast<Any::derived<T>*>(operand->b);
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
    Any::derived<T>* d = any_cast(&operand);
    if (d != NULL)
    {
        return d->value;
    }
    else
    {
        throw bad_any_cast();
    }
}

template<typename T>
T any_cast(const any& operand)
{
    return any_cast(const_cast<any&>(operand));
}

inline void swap(any& lhs, any& rhs) NOEXCEPT
{
    lhs.swap(rhs);
}


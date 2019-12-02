#pragma once
#include <cstddef>
#include <typeinfo>
#include <memory>
#include <utility>

#define NOEXCEPT noexcept

class any;

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
    any() NOEXCEPT
    {
    }

    any(const any& other) : b(other.b)
    {
    }

    any(std::shared_ptr<Any::base> b_) : b(b_)
    {
    }

    ~any()
    {
        reset();
    }

    void reset() NOEXCEPT
    {
        b.reset();
    }

    bool has_value() const NOEXCEPT
    {
        return bool(b);
    }

    any& operator=(any& other)
    {
        if (this != &other)
        {
            b = other.b;
            //other.b = NULL;
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

private:
    //Any::base* b;
    std::shared_ptr<Any::base> b;
};

template<typename T>
any make_any(const T& value) NOEXCEPT
{
    //Any::base* b = new Any::derived<T>(value);
    auto b = std::make_shared<Any::derived<T>>(value);
    return any(b);
}

template<typename T>
T* any_cast(any* operand) NOEXCEPT
{
    //Any::derived<T>* d = dynamic_cast<Any::derived<T>*>(operand->b);
    auto d = std::dynamic_pointer_cast<Any::derived<T>>(operand->b);
    if (d)
    {
        return &(d->value);
    }
    else
    {
        return nullptr;
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
    if (d != nullptr)
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


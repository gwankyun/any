#pragma once
#include <cstddef>
#include <typeinfo>
#include <iostream>
#include <utility> // std::in_place_type std::in_place_type_t
#include "any/marco.hpp"

#if ANY_HAS_CXX_11
#  include <type_traits>
#  include <initializer_list>
#endif // ANY_HAS_CXX_11

namespace lite
{
#if ANY_HAS_CXX_17
    template<typename T>
    using in_place_type_t = std::in_place_type_t<T>;
#else
    template<typename T>
    struct in_place_type_t
    {
        explicit in_place_type_t() {}
    };
#endif // ANY_HAS_CXX_17

    class bad_any_cast : public std::bad_cast
    {
    public:
        bad_any_cast() NOEXCEPT {}

        ~bad_any_cast() {}

        char const* what() const NOEXCEPT
        {
            return "bad_any_cast";
        }

    private:

    };

    class any
    {
    public:

        // std::any::any

        CONSTEXPR any() NOEXCEPT : base(NULLPTR) // 1
        {
        }

        any(const any& other) // 2
            : base(other.clone())
        {
        }

#if ANY_HAS_CXX_11
        any(any&& other) NOEXCEPT // 3
            : base(NULLPTR)
        {
            swap(other);
        }

        template<typename ValueType>
        explicit any(ValueType&& value) // 4
            : base(new Derived<DECAY_T(ValueType)>(value))
        {
        }
#else
        template<typename ValueType>
        explicit any(const ValueType& value)
            : base(new Derived<DECAY_T(ValueType)>(value))
        {
        }
#endif // ANY_HAS_CXX_11

#if ANY_HAS_CXX_11
        template<typename ValueType, typename... Args>
        explicit any(in_place_type_t<ValueType>, Args&&... args) // 5
            : base(new Derived<DECAY_T(ValueType)>(IN_PLACE_TYPE(DECAY_T(ValueType)), std::forward<Args>(args)...))
        {
        }
#else
        BOOST_PP_REPEAT(10, ANY_IN_PLACE_TYPE, _);
#endif // ANY_HAS_CXX_11

#if ANY_HAS_CXX_11
        template<typename ValueType, typename U, typename... Args>
        explicit any(in_place_type_t<ValueType>, std::initializer_list<U> il,
            Args&&... args) // 6
            : base(new Derived<DECAY_T(ValueType)>(il, std::forward<Args>(args)...))
        {
        }
#endif // ANY_HAS_CXX_11

        // std::any::operator=

        any& operator=(const any& other) // 1
        {
            if (this != &other)
            {
                reset();
                base = other.clone();
            }
            return *this;
        }

#if ANY_HAS_CXX_11
        any& operator=(any&& rhs) NOEXCEPT // 2
        {
            swap(rhs);
            return *this;
        }
#endif // ANY_HAS_CXX_11

        template<typename ValueType>
        any& operator=(const ValueType& rhs) // 3
        {
            reset();
            base = new Derived<DECAY_T(ValueType)>(IN_PLACE_TYPE(DECAY_T(ValueType)), rhs);
            return *this;
        }

        // std::any::~any

        ~any()
        {
            reset();
        }

        // std::any::emplace

#if ANY_HAS_CXX_11
        template<typename ValueType, typename ...Args>
        DECAY_T(ValueType)& emplace(Args&& ...args) // 1
        {
            reset();
            *this = DECAY_T(ValueType)(std::forward<Args>(args)...);
            return get_value<DECAY_T(ValueType)>();
        }

        template<typename ValueType, typename U, typename... Args >
        DECAY_T(ValueType)& emplace(std::initializer_list<U> il, Args&&... args) // 2
        {
            reset();
            *this = DECAY_T(ValueType)(il, std::forward<Args>(args)...);
            return get_value<DECAY_T(ValueType)>();
        }
#else
        BOOST_PP_REPEAT(10, ANY_EMPLACE, _); // 1
#endif // ANY_HAS_CXX_11

        // std::any::reset

        void reset() NOEXCEPT
        {
            if (has_value())
            {
                delete base;
                base = NULLPTR;
            }
        }

        // std::any::swap

        void swap(any& other) NOEXCEPT
        {
            Base* temp = base;
            base = other.base;
            other.base = temp;
        }

        // std::any::has_value

        bool has_value() const NOEXCEPT
        {
            return base != NULLPTR;
        }

        // std::any::type

        const std::type_info& type() const NOEXCEPT
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
        friend T* any_cast(any* operand) NOEXCEPT;

    private:
        class Base
        {
        public:
            Base() NOEXCEPT {}

            virtual ~Base() {}

            virtual const std::type_info& type() const NOEXCEPT = 0;

            virtual Base* clone() = 0;

        private:

        };

        template<typename T>
        class Derived : public Base
        {
        public:
            Derived() {}

#if ANY_HAS_CXX_11
            template<typename ValueType, typename... Args>
            Derived(in_place_type_t<ValueType>, Args&&... args)
                : value(std::forward<Args>(args)...)
            {
            }
#else
            BOOST_PP_REPEAT(10, ANY_DERIVED, _); // Derived(in_place_type_t<ValueType>); Derived(in_place_type_t<ValueType>, T0 T0_); ...
#endif // ANY_HAS_CXX_11

#if ANY_HAS_CXX_11
            template<typename U, typename... Args>
            Derived(std::initializer_list<U> il, Args&&... args)
                : value(il, std::forward<Args>(args)...)
            {
            }
#endif // ANY_HAS_CXX_11

            ~Derived() {}

            const std::type_info& type() const NOEXCEPT OVERRIDE
            {
                return typeid(value);
            }

            Base* clone() OVERRIDE
            {
                return new Derived<T>(IN_PLACE_TYPE(T), value);
            }

            T value;

        private:

        };

        template<typename T>
        T& get_value()
        {
            return dynamic_cast<Derived<T>*>(base)->value;
        }

        Base* clone() const
        {
            if (base == NULLPTR)
            {
                return NULLPTR;
            }
            return base->clone();
        }

        Base* base;
    };

    // std::swap(std::any)

    inline void swap(any& lhs, any& rhs) NOEXCEPT
    {
        lhs.swap(rhs);
    }

    // std::any_cast

    template<typename T>
    T* any_cast(any* operand) NOEXCEPT // 5
    {
        if (operand == NULLPTR)
        {
            return NULLPTR;
        }
        any::Derived<T>* d = dynamic_cast<any::Derived<T>*>(operand->base);
        if (d == NULLPTR)
        {
            return NULLPTR;
        }
        return &(d->value);
    }

    template<typename T>
    const T* any_cast(const any* operand) NOEXCEPT // 4
    {
        return any_cast<T>(const_cast<any*>(operand));
    }

    template<typename T>
    T any_cast(any& operand) // 2
    {
        T* d = any_cast<T>(&operand);
        if (d != NULLPTR)
        {
            return *d;
        }
        else
        {
            throw bad_any_cast();
        }
    }

    template<typename T>
    T any_cast(const any& operand) // 1
    {
        return any_cast<T>(const_cast<any&>(operand));
    }

#if ANY_HAS_CXX_11
    template<typename T>
    T any_cast(any&& operand) // 3
    {
        return any_cast<T>(operand);
    }
#endif // ANY_HAS_CXX_11

    // std::make_any

#if ANY_HAS_CXX_11
    template<typename T, typename ...Args>
    any make_any(Args&& ...args) NOEXCEPT // 1
    {
        return any(IN_PLACE_TYPE(T), std::forward<Args>(args)...);
    }

    template<typename T, typename U, typename... Args>
    any make_any(std::initializer_list<U> il, Args&&... args) // 2
    {
        return any(IN_PLACE_TYPE(T), il, std::forward<Args>(args)...);
    }
#else
    BOOST_PP_REPEAT(10, MAKE_ANY, _); // 1
#endif // ANY_HAS_CXX_11
}


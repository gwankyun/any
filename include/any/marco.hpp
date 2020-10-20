#pragma once
#include "any/compiler_detection/short.hpp"

#ifndef ANY_HAS_CXX_11
#  if __cplusplus >= 201103L || defined(__cpp_variadic_templates)
#    define ANY_HAS_CXX_11 1
#  else
#    define ANY_HAS_CXX_11 0
#  endif // __cplusplus >= 201103L || defined(__cpp_variadic_templates)
#endif // ANY_HAS_CXX_11

#ifndef ANY_HAS_CXX_14
#  if (__cplusplus >= 201402L || defined(__cpp_alias_templates)) && ANY_HAS_CXX_11
#    define ANY_HAS_CXX_14 1
#  else
#    define ANY_HAS_CXX_14 0
#  endif // (__cplusplus >= 201402L || defined(__cpp_alias_templates)) && ANY_HAS_CXX_11
#endif // ANY_HAS_CXX_14

#ifndef ANY_HAS_CXX_17
#  if (__cplusplus >= 201703L || defined(__cpp_inline_variables)) && ANY_HAS_CXX_14
#    define ANY_HAS_CXX_17 1
#  else
#    define ANY_HAS_CXX_17 0
#  endif // (__cplusplus >= 201703L || defined(__cpp_inline_variables)) && ANY_HAS_CXX_14
#endif // !ANY_HAS_CXX_17

#if !ANY_HAS_CXX_11
#  include "any/boost/type_traits/decay.hpp"
#  include "any/boost/preprocessor/comma_if.hpp"
#  include "any/boost/preprocessor/repeat.hpp"
#endif // !ANY_HAS_CXX_11

#ifndef DECAY_T
#  if ANY_HAS_CXX_14
#    define DECAY_T(T) std::decay_t<T>
#  elif ANY_HAS_CXX_11
#    define DECAY_T(T) typename std::decay<T>::type
#  else
#    define DECAY_T(T) typename boost::decay<T>::type
#  endif // ANY_HAS_CXX_14
#endif // !DECAY_T

#ifndef IN_PLACE_TYPE
#  if ANY_HAS_CXX_17
#    define IN_PLACE_TYPE(x) std::in_place_type<x>
#  else 
#    define IN_PLACE_TYPE(x) lite::in_place_type_t<x>()
#  endif // ANY_HAS_CXX_17
#endif // !IN_PLACE_TYPE

#ifndef ANY_ARG
#  if defined(__cpp_rvalue_references)
#    define ANY_ARG(T) T&&
#  else
#    define ANY_ARG(T) const T&
#  endif // defined(__cpp_rvalue_references)
#endif // !ANY_ARG

#if !ANY_HAS_CXX_11

#ifndef ANY_TYPENAME
#define ANY_TYPENAME(z, n, x) BOOST_PP_COMMA_IF(n) typename x##n
#endif // !ANY_TYPENAME

#ifndef ANY_PARAMETER
#define ANY_PARAMETER(z, n, x) BOOST_PP_COMMA_IF(n) ANY_ARG(x##n) x##n##_
#endif // !ANY_PARAMETER

#ifndef ANY_ARGUMENT
#define ANY_ARGUMENT(z, n, x) BOOST_PP_COMMA_IF(n) x##n##_
#endif // !ANY_ARGUMENT

#ifndef BOOST_PP_REPEAT_Z
#define BOOST_PP_REPEAT_Z(z) BOOST_PP_REPEAT_##z
#endif // !BOOST_PP_REPEAT_Z

#ifndef BOOST_PP_REPEAT_FROM_TO_Z
#define BOOST_PP_REPEAT_FROM_TO_Z(z) BOOST_PP_REPEAT_FROM_TO_##z
#endif // !BOOST_PP_REPEAT_FROM_TO_Z

#ifndef MAKE_ANY
#define MAKE_ANY(z, n, _) \
    template<typename T BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT_Z(z)(n, ANY_TYPENAME, T)> /* , typename T0, typename T1 ... */ \
    any make_any(BOOST_PP_REPEAT_Z(z)(n, ANY_PARAMETER, T)) NOEXCEPT /* , ANY_ARG(T0) T0_, ANY_ARG(T1) T1_ ... */ \
    { \
        return any(IN_PLACE_TYPE(T) BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT_Z(z)(n, ANY_ARGUMENT, T)); \
    }
#endif // !MAKE_ANY

#ifndef ANY_EMPLACE
#define ANY_EMPLACE(z, n, _) \
    template<typename ValueType BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT_Z(z)(n, ANY_TYPENAME, T)> /* , typename T0, typename T1 ... */ \
    DECAY_T(ValueType)& emplace(BOOST_PP_REPEAT_Z(z)(n, ANY_PARAMETER, T)) /* , ANY_ARG(T0) T0_, ANY_ARG(T1) T1_ ... */ \
    { \
        reset(); \
        *this = DECAY_T(ValueType)(BOOST_PP_REPEAT_Z(z)(n, ANY_ARGUMENT, T)); /* , T0_, T1_ ... */ \
        return get_value<ValueType>(); \
    }
#endif // !ANY_EMPLACE

#ifndef ANY_IN_PLACE_TYPE
#define ANY_IN_PLACE_TYPE(z, n, _) \
    template<typename ValueType BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT_Z(z)(n, ANY_TYPENAME, T)> \
    explicit any(in_place_type_t<ValueType> BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT_Z(z)(n, ANY_PARAMETER, T)) \
        : base(new Derived<ValueType>(IN_PLACE_TYPE(DECAY_T(ValueType)) BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT_Z(z)(n, ANY_ARGUMENT, T))) \
    { \
    }
#endif // !ANY_IN_PLACE_TYPE

#ifndef ANY_DERIVED
#define ANY_DERIVED(z, n, _) \
    template<typename ValueType BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT_Z(z)(n, ANY_TYPENAME, T)> \
    explicit Derived(in_place_type_t<ValueType> BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT_Z(z)(n, ANY_PARAMETER, T)) \
        : value(BOOST_PP_REPEAT_Z(z)(n, ANY_ARGUMENT, T)) \
    { \
    }
#endif // !ANY_DERIVED

#endif // !ANY_HAS_CXX_11


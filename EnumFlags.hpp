#pragma once

#include <type_traits>

#ifndef ENUM_TEMPLATE
#define ENUM_TEMPLATE \
template < \
    class Enum, \
    class U = typename std::underlying_type<Enum>::type \
    typename = std::enable_if<std::is_enum<Enum>::value> \
>
#endif


ENUM_TEMPLATE
inline Enum operator ~ (Enum a)
    { return static_cast<Enum>(~(static_cast<U>(a))); }
ENUM_TEMPLATE
inline Enum operator | (Enum a, Enum b)
    { return static_cast<Enum>(static_cast<U>(a) | static_cast<U>(b)); }
ENUM_TEMPLATE
inline Enum operator & (Enum a, Enum b)
    { return static_cast<Enum>(static_cast<U>(a) & static_cast<U>(b)); }
ENUM_TEMPLATE
inline Enum operator ^ (Enum a, Enum b)
    { return static_cast<Enum>(static_cast<U>(a) ^ static_cast<U>(b)); }
ENUM_TEMPLATE
inline Enum operator |= (Enum& a, Enum b)
    { return (a = (a | b)); }
ENUM_TEMPLATE
inline Enum operator &= (Enum& a, Enum b)
    { return (a = (a & b)); }
ENUM_TEMPLATE
inline Enum operator ^= (Enum& a, Enum b)
    { return (a = (a ^ b)); }

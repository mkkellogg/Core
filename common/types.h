#pragma once

#include <type_traits>
#include <iostream>

namespace Core {

  typedef bool Bool;

  typedef unsigned char Byte;
  typedef unsigned char UChar;
  typedef char Char;

  typedef double RealDouble;

#ifdef _Real_DoublePrecision_
  typedef double Real;
#else
  typedef float Real;
#endif

  typedef char Int8;
  typedef unsigned char UInt8;

  typedef short Int16;
  typedef unsigned short UInt16;

  typedef int Int32;
  typedef unsigned int UInt32;

  typedef long long Int64;
  typedef unsigned long long UInt64;

  template <typename T>
  struct is_numeric{
    static const Bool value = std::is_integral<T>::value || std::is_floating_point<T>::value;
  };

  template< bool B, class T = void >
  using enable_if_t = typename std::enable_if<B, T>::type;

}



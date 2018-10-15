/*=============================================================================
   Copyright (c) 2016-2018 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(COMMON_SUPPORT_APRIL_10_2016)
#define COMMON_SUPPORT_APRIL_10_2016

#include <cfloat>
#include <cstdint>
#include <cassert>
#include <chrono>

namespace cycfi
{
   ////////////////////////////////////////////////////////////////////////////
   // Time
   ////////////////////////////////////////////////////////////////////////////
   using duration       = std::chrono::duration<double>;
   using microseconds   = std::chrono::duration<double, std::micro>;
   using milliseconds   = std::chrono::duration<double, std::milli>;
   using seconds        = std::chrono::duration<double>;
   using minutes        = std::chrono::duration<double, std::ratio<60>>;
   using hours          = std::chrono::duration<double, std::ratio<60*60>>;
   using time_point     = std::chrono::time_point<std::chrono::steady_clock, duration>;

   ////////////////////////////////////////////////////////////////////////////
   // static int types
	////////////////////////////////////////////////////////////////////////////
   template <typename T, T value_>
   struct static_int
   {
      static constexpr T value =  value_;
   };

   template <int i>
   using int_ = static_int<int, i>;

   template <std::size_t i>
   using uint_ = static_int<std::size_t, i>;

   template <int8_t i>
   using int8_ = static_int<int8_t, i>;

   template <uint8_t i>
   using uint8_ = static_int<uint8_t, i>;

   template <int16_t i>
   using int16_ = static_int<int16_t, i>;

   template <uint16_t i>
   using uint16_ = static_int<uint16_t, i>;

   template <int32_t i>
   using int32_ = static_int<int32_t, i>;

   template <uint32_t i>
   using uint32_ = static_int<uint32_t, i>;

   template <int64_t i>
   using int64_ = static_int<int64_t, i>;

   template <uint64_t i>
   using uint64_ = static_int<uint64_t, i>;

   ////////////////////////////////////////////////////////////////////////////
   // Some constants
	////////////////////////////////////////////////////////////////////////////
   template <typename T>
   struct int_traits;

   template <>
   struct int_traits<uint32_t>
   {
      static constexpr uint32_t max = 4294967295;
      static constexpr uint32_t min = 0;
   };

   template <>
   struct int_traits<int32_t>
   {
      static constexpr int32_t max = 2147483647;
      static constexpr int32_t min = -2147483648;
   };

   template <>
   struct int_traits<uint16_t>
   {
      static constexpr uint16_t max = 65535;
      static constexpr uint16_t min = 0;
   };

   template <>
   struct int_traits<int16_t>
   {
      static constexpr int16_t max = 32767;
      static constexpr int16_t min = -32768;
   };

   template <>
   struct int_traits<uint8_t>
   {
      static constexpr uint8_t max = 255;
      static constexpr uint8_t min = 0;
   };

   template <>
   struct int_traits<int8_t>
   {
      static constexpr uint8_t max = 127;
      static constexpr uint8_t min = -128;
   };

   template <typename T>
   constexpr T int_max()
   {
      return int_traits<T>::max;
   }

   template <typename T>
   constexpr T int_min()
   {
      return int_traits<T>::min;
   }
}
#endif

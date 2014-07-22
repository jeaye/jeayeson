/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: traits.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <type_traits>

#include "defines.hpp"

namespace jeayeson
{
  namespace detail
  {
    using int_t = JEAYESON_INT;
    using float_t = JEAYESON_FLOAT;

    template <bool B, typename T = void>
    using enable_if = typename std::enable_if<B, T>::type;

    template <typename T, typename E = void>
    struct normalize_impl
    { using type = T; };
    template <typename T>
    struct normalize_impl<T, enable_if<std::is_integral<T>::value &&
                                       !std::is_same<T, bool>::value>>
    { using type = int_t; };
    template <typename T>
    struct normalize_impl<T, enable_if<std::is_same<T, bool>::value>>
    { using type = bool; };
    template <typename T>
    struct normalize_impl<T, enable_if<std::is_floating_point<T>::value>>
    { using type = float_t; };
    template <typename T>
    using normalize = typename normalize_impl<T>::type;
    template <typename T>
    bool constexpr should_normalize()
    {
      return !std::is_same<T, bool>::value &&
             (std::is_integral<T>::value ||
              std::is_floating_point<T>::value);
    }
  }
}

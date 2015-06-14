/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: detail/normalize.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <type_traits>

#include "config.hpp"

namespace jeayeson
{
  namespace detail
  {
    using int_t = config<config_tag>::int_t;
    using float_t = config<config_tag>::float_t;

    template <typename T, typename E = void>
    struct is_string_impl
    { static bool constexpr value{ false }; };
    template <typename T>
    struct is_string_impl
    <
      T,
      std::enable_if_t
      <
        std::is_same<std::decay_t<T>, std::string>::value ||
        std::is_same<std::decay_t<T>, char const*>::value ||
        std::is_same<std::decay_t<T>, char*>::value
      >
    >
    { static bool constexpr value{ true }; };
    template <typename T>
    bool constexpr is_string()
    { return is_string_impl<T>::value; }

    template <typename T, typename V, typename E = void>
    struct is_convertible_impl
    { static bool constexpr value{ false }; };
    template <typename T, typename V>
    struct is_convertible_impl
    <
      T, V,
      std::enable_if_t
      <
        std::is_same<std::decay_t<T>, typename V::null_t>::value ||
        std::is_same<std::decay_t<T>, std::nullptr_t>::value ||
        std::is_integral<std::decay_t<T>>::value ||
        std::is_floating_point<std::decay_t<T>>::value ||
        is_string<T>() ||
        std::is_same<std::decay_t<T>, typename V::map_t>::value ||
        std::is_same<std::decay_t<T>, typename V::array_t>::value
      >
    >
    { static bool constexpr value{ true }; };

    template <typename T, typename V>
    bool constexpr is_convertible()
    { return is_convertible_impl<T, V>::value; }

    template <typename T, typename E = void>
    struct normalize_impl
    { using type = T; };
    template <typename T>
    struct normalize_impl
    <
      T,
      std::enable_if_t
      <
        std::is_integral<std::decay_t<T>>::value &&
        !std::is_same<T, bool>::value
      >
    >
    { using type = int_t; };
    template <typename T>
    struct normalize_impl
    <
      T,
      std::enable_if_t
      <std::is_same<std::decay_t<T>, bool>::value>
    >
    { using type = bool; };
    template <typename T>
    struct normalize_impl
    <
      T,
      std::enable_if_t
      <std::is_floating_point<std::decay_t<T>>::value>
    >
    { using type = float_t; };
    template <typename T>
    struct normalize_impl
    <
      T,
      std::enable_if_t
      <
        std::is_same<std::decay_t<T>, char const*>::value ||
        std::is_same<std::decay_t<T>, char*>::value
      >
    >
    { using type = std::string; };
    template <typename T>
    using normalize = typename normalize_impl<T>::type;

    template <typename T>
    bool constexpr should_normalize()
    {
      using decayed = std::decay_t<T>;
      return
      (
        (!std::is_same<decayed, bool>::value) &&
        (
          std::is_integral<decayed>::value ||
          std::is_floating_point<decayed>::value ||
          std::is_same<decayed, char const*>::value ||
          std::is_same<decayed, char*>::value
        )
      );
    }
  }
}

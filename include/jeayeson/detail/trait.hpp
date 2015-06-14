/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: detail/trait.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

namespace jeayeson
{
  template <>
  struct value::to_type<value::type::null>
  { using type = value::null_t; };
  template <>
  struct value::to_type<value::type::integer>
  { using type = detail::int_t; };
  template <>
  struct value::to_type<value::type::real>
  { using type = detail::float_t; };
  template <>
  struct value::to_type<value::type::boolean>
  { using type = bool; };
  template <>
  struct value::to_type<value::type::string>
  { using type = std::string; };
  template <>
  struct value::to_type<value::type::map>
  { using type = map_t; };
  template <>
  struct value::to_type<value::type::array>
  { using type = array_t; };

  template <>
  struct value::to_value<value::null_t>
  { static type constexpr const value{ ::jeayeson::value::type::null }; };
  template <typename T>
  struct value::to_value
  <
    T,
    std::enable_if_t
    <
      std::is_integral<std::decay_t<T>>::value &&
      !std::is_same<std::decay_t<T>, bool>::value
    >
  >
  { static type constexpr const value{ ::jeayeson::value::type::integer }; };
  template <typename T>
  struct value::to_value
  <
    T,
    std::enable_if_t
    <
      std::is_floating_point<std::decay_t<T>>::value
    >
  >
  { static type constexpr const value{ ::jeayeson::value::type::real }; };
  template <>
  struct value::to_value<bool>
  { static type constexpr const value{ ::jeayeson::value::type::boolean }; };
  template <typename T>
  struct value::to_value
  <
    T,
    std::enable_if_t
    <
      detail::is_string<std::decay_t<T>>()
    >
  >
  { static type constexpr const value{ ::jeayeson::value::type::string }; };
  template <>
  struct value::to_value<map_t>
  { static type constexpr const value{ ::jeayeson::value::type::map }; };
  template <>
  struct value::to_value<array_t>
  { static type constexpr const value{ ::jeayeson::value::type::array }; };
}

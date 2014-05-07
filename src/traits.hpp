/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: traits.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

namespace jeayeson
{
  namespace detail
  {
    /* Enum -> Type */
    template <value::type_t T>
    struct to_type;
    template <>
    struct to_type<value::type_null>
    { using type = value::null_t; };
    template <>
    struct to_type<value::type_int8>
    { using type = int8_t; };
    template <>
    struct to_type<value::type_uint8>
    { using type = uint8_t; };
    template <>
    struct to_type<value::type_int16>
    { using type = int16_t; };
    template <>
    struct to_type<value::type_uint16>
    { using type = uint16_t; };
    template <>
    struct to_type<value::type_int32>
    { using type = int32_t; };
    template <>
    struct to_type<value::type_uint32>
    { using type = uint32_t; };
    template <>
    struct to_type<value::type_int64>
    { using type = int64_t; };
    template <>
    struct to_type<value::type_uint64>
    { using type = uint64_t; };
    template <>
    struct to_type<value::type_int>
    { using type = int; };
    template <>
    struct to_type<value::type_unsigned_int>
    { using type = unsigned int; };
    template <>
    struct to_type<value::type_float>
    { using type = float; };
    template <>
    struct to_type<value::type_double>
    { using type = double; };
    template <>
    struct to_type<value::type_bool>
    { using type = bool; };
    template <>
    struct to_type<value::type_string>
    { using type = std::string; };
    template <>
    struct to_type<value::type_map>
    { using type = json_map; };
    template <>
    struct to_type<value::type_array>
    { using type = json_array; };

    /* Type -> Enum */
    template <typename T>
    struct to_value;
    template <>
    struct to_value<value::null_t>
    { static value::type_t constexpr const value{ value::type_null }; };
    template <>
    struct to_value<int8_t>
    { static value::type_t constexpr const value{ value::type_int8 }; };
    template <>
    struct to_value<uint8_t>
    { static value::type_t constexpr const value{ value::type_uint8 }; };
    template <>
    struct to_value<int16_t>
    { static value::type_t constexpr const value{ value::type_int16 }; };
    template <>
    struct to_value<uint16_t>
    { static value::type_t constexpr const value{ value::type_uint16 }; };
    template <>
    struct to_value<int32_t>
    { static value::type_t constexpr const value{ value::type_int32 }; };
    template <>
    struct to_value<uint32_t>
    { static value::type_t constexpr const value{ value::type_uint32 }; };
    template <>
    struct to_value<int64_t>
    { static value::type_t constexpr const value{ value::type_int64 }; };
    template <>
    struct to_value<uint64_t>
    { static value::type_t constexpr const value{ value::type_uint64 }; };
    //template <>
    //struct to_value<int>
    //{ static value::type_t constexpr const value{ value::type_int }; };
    //template <>
    //struct to_value<unsigned int>
    //{ static value::type_t constexpr const value{ value::type_unsigned_int }; };
    template <>
    struct to_value<float>
    { static value::type_t constexpr const value{ value::type_float }; };
    template <>
    struct to_value<double>
    { static value::type_t constexpr const value{ value::type_double }; };
    template <>
    struct to_value<bool>
    { static value::type_t constexpr const value{ value::type_bool }; };
    template <>
    struct to_value<std::string>
    { static value::type_t constexpr const value{ value::type_string }; };
    template <>
    struct to_value<json_map>
    { static value::type_t constexpr const value{ value::type_map }; };
    template <>
    struct to_value<json_array>
    { static value::type_t constexpr const value{ value::type_array }; };
  }
}

/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: value.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <boost/variant.hpp>

#include "map.hpp"
#include "array.hpp"
#include "parser.hpp"

namespace jeayeson
{
  template <bool B, typename T = void>
  using enable_if = typename std::enable_if<B, T>::type;

  class value
  {
    public:
      /* Maps to the variant 1:1. */
      enum type_t
      {
        type_null,
        type_int64, 
        type_double,
        type_bool,
        type_string,
        type_map,
        type_array
      };

      using map_t = map<value, parser>;
      using array_t = array<value, parser>;
      struct null_t
      {
        bool operator ==(null_t const &) const
        { return true; }
        bool operator !=(null_t const &) const
        { return false; }
      };
      using int_t = int64_t;
      using float_t = double;

      using variant_t = boost::variant
      <
        null_t,
        int64_t,
        double,
        bool,
        std::string,
        map_t,
        array_t
      >;
      using cstr_t = char const * const;

      /* Enum -> Type */
      template <type_t T>
      struct to_type;
      /* Type -> Enum */
      template <typename T>
      struct to_value;

      template <typename T, typename E = void>
      struct normalize_impl
      { using type = T; };
      template <typename T>
      using normalize = typename normalize_impl<T>::type;
      template <typename T>
      static constexpr bool should_normalize()
      { return std::is_integral<T>::value; }

      value() : value_(null_t())
      { }
      template <typename T>
      explicit value(T const &val) : value_{ null_t() }
      { set(val); }
      explicit value(cstr_t const str) : value_{ std::string(str) }
      { }
      value(value const &copy) : value_{ copy.value_ }
      { }

      template <typename T>
      normalize<T>& get()
      { return boost::get<normalize<T>&>(value_); }
      template <typename T>
      normalize<T> const& get() const
      { return boost::get<normalize<T> const&>(value_); }

      template <typename T>
      normalize<T>& as()
      { return boost::get<normalize<T>&>(value_); }
      template <typename T>
      normalize<T> const& as() const
      { return boost::get<normalize<T> const&>(value_); }

      template <typename T>
      operator T() 
      { return as<T&>(); }
      template <typename T>
      operator T() const
      { return as<T const&>(); }

      type_t get_type() const
      { return static_cast<type_t>(value_.which()); }
      bool is(type_t const type) const
      { return get_type() == type; }

      friend bool operator ==(value const &jv, value const &val);
      template <typename T>
      friend bool operator ==(value const &jv, T const &val);
      template <typename T>
      friend bool operator ==(T const &val, value const &jv);
      friend bool operator ==(value const &jv, cstr_t const val);
      friend bool operator ==(cstr_t const val, value const &jv);

      friend bool operator !=(value const &jv, value const &val);
      template <typename T>
      friend bool operator !=(value const &jv, T const &val);
      template <typename T>
      friend bool operator !=(T const &val, value const &jv);
      friend bool operator !=(value const &jv, cstr_t const val);
      friend bool operator !=(cstr_t const val, value const &jv);
      friend std::ostream& operator <<(std::ostream &stream, value const &val);

      template <typename T>
      enable_if<!should_normalize<T>()> set(T const &val)
      { value_ = val; }
      template <typename T>
      enable_if<should_normalize<T>()> set(T const &val)
      { value_ = normalize<T>(val); }

      /* Treat string literals as standard strings. */
      void set(cstr_t const val)
      { value_ = static_cast<std::string>(val); }

      /* Shortcut add for arrays. */
      template <typename T>
      void add(T const &val)
      { as<array_t>().add(val); }

      /* Shortcut add for maps. */
      template <typename T>
      void add(std::string const &key, T const &val)
      { as<map_t>().set(key, val); }

      template <typename T>
      variant_t& operator =(T const &val)
      { set(val); return value_; }
      variant_t& operator =(cstr_t const val)
      { return (value_ = std::string{ val }); }

    private:
      variant_t value_;
  };

  using map_t = map<value, parser>;
  using array_t = array<value, parser>;

  inline std::ostream& operator <<(std::ostream &stream, value const &val)
  {
    switch(val.value_.which())
    {
      case value::type_string:
        return (stream << "\"" << val.value_ << "\"");
      case value::type_bool:
        return (stream << (val.as<bool>() ? "true" : "false"));
      default:
        return (stream << val.value_);
    }
  }

  template <typename Iter>
  inline void streamjoin(Iter begin, Iter const end, std::ostream &stream,
                         std::string const &sep = ",")
  {
    if(begin != end)
    { stream << *begin++; }
    while(begin != end)
    { stream << sep << *begin++; }
  }

  inline std::ostream& operator <<(std::ostream &stream, value::null_t const &)
  { return (stream << "null"); }

  template<>
  inline std::ostream& operator <<(std::ostream &stream, array_t const &arr)
  {
    stream << arr.delim_open;
    streamjoin(arr.values_.begin(), arr.values_.end(), stream);
    stream << arr.delim_close;
    return stream;
  }

  inline std::ostream& operator <<(std::ostream &stream,
                                   map_t::internal_map_t::value_type const &p)
  { return (stream << "\"" << p.first << "\":" << p.second); }

  template<>
  inline std::ostream& operator <<(std::ostream &stream, map_t const &m)
  {
    stream << m.delim_open;
    streamjoin(m.values_.begin(), m.values_.end(), stream);
    stream << m.delim_close;
    return stream;
  }

  template <>
  value& value::get<value>()
  { return *this; }
  template <>
  value const& value::get<value>() const
  { return *this; }
  template <>
  value& value::as<value>()
  { return *this; }
  template <>
  value const& value::as<value>() const
  { return *this; }

  template <>
  struct value::to_type<value::type_null>
  { using type = value::null_t; };
  template <>
  struct value::to_type<value::type_int64>
  { using type = int_t; };
  template <>
  struct value::to_type<value::type_double>
  { using type = double; };
  template <>
  struct value::to_type<value::type_bool>
  { using type = bool; };
  template <>
  struct value::to_type<value::type_string>
  { using type = std::string; };
  template <>
  struct value::to_type<value::type_map>
  { using type = map_t; };
  template <>
  struct value::to_type<value::type_array>
  { using type = array_t; };

  template <>
  struct value::to_value<value::null_t>
  { static type_t constexpr const value{ value::type_null }; };
  template <>
  struct value::to_value<int8_t>
  { static type_t constexpr const value{ value::type_int64 }; };
  template <>
  struct value::to_value<uint8_t>
  { static type_t constexpr const value{ value::type_int64 }; };
  template <>
  struct value::to_value<int16_t>
  { static type_t constexpr const value{ value::type_int64 }; };
  template <>
  struct value::to_value<uint16_t>
  { static type_t constexpr const value{ value::type_int64 }; };
  template <>
  struct value::to_value<int32_t>
  { static type_t constexpr const value{ value::type_int64 }; };
  template <>
  struct value::to_value<uint32_t>
  { static type_t constexpr const value{ value::type_int64 }; };
  template <>
  struct value::to_value<value::int_t>
  { static type_t constexpr const value{ value::type_int64 }; };
  template <>
  struct value::to_value<uint64_t>
  { static type_t constexpr const value{ value::type_int64 }; };
  template <>
  struct value::to_value<float>
  { static type_t constexpr const value{ value::type_double }; };
  template <>
  struct value::to_value<double>
  { static type_t constexpr const value{ value::type_double }; };
  template <>
  struct value::to_value<bool>
  { static type_t constexpr const value{ value::type_bool }; };
  template <>
  struct value::to_value<std::string>
  { static type_t constexpr const value{ value::type_string }; };
  template <>
  struct value::to_value<map_t>
  { static type_t constexpr const value{ value::type_map }; };
  template <>
  struct value::to_value<array_t>
  { static type_t constexpr const value{ value::type_array }; };

  template <typename T>
  struct value::normalize_impl<T, enable_if<std::is_integral<T>::value>>
  { using type = int_t; };
  template <typename T>
  struct value::normalize_impl<T, enable_if<std::is_floating_point<T>::value>>
  { using type = float_t; };
}

using json_value = jeayeson::value;
using json_map = jeayeson::map_t;
using json_array = jeayeson::array_t;
using json_null = json_value::null_t;
using json_int = json_value::int_t;
using json_float = json_value::float_t;

namespace jeayeson
{
  inline bool operator ==(json_value const &jv, json_value const &val)
  { return jv.get_type() == val.get_type() && jv.value_ == val.value_; }
  template <typename T>
  bool operator ==(json_value const &jv, T const &val)
  { return jv.get_type() == value::to_value<T>::value && jv.as<T>() == val; }
  template <typename T>
  bool operator ==(T const &val, json_value const &jv)
  { return jv == val; }
  inline bool operator ==(json_value const &jv, value::cstr_t const val)
  { return jv.get_type() == value::type_string && jv.as<std::string>() == val; }
  inline bool operator ==(value::cstr_t const val, json_value const &jv)
  { return jv == val; }

  inline bool operator !=(json_value const &jv, json_value const &val)
  { return !(jv == val); }
  template <typename T>
  bool operator !=(json_value const &jv, T const &val)
  { return !(jv == val); }
  template <typename T>
  bool operator !=(T const &val, json_value const &jv)
  { return !(jv == val); }
  inline bool operator !=(json_value const &jv, value::cstr_t const val)
  { return !(jv == val); }
  inline bool operator !=(value::cstr_t const val, json_value const &jv)
  { return !(jv == val); }
}

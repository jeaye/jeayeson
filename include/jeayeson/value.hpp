/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: value.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <boost/variant.hpp>

#include "detail/traits.hpp"
#include "file.hpp"
#include "map.hpp"
#include "array.hpp"
#include "detail/parser.hpp"
#include "detail/escape.hpp"

namespace jeayeson
{
  class value
  {
    public:
      /* Maps to the variant 1:1. */
      enum class type
      {
        null,
        integer,
        real,
        boolean,
        string,
        map,
        array
      };

      using map_t = map<value, detail::parser>;
      using array_t = array<value, detail::parser>;
      struct null_t
      {
        bool operator ==(null_t const &) const
        { return true; }
        bool operator !=(null_t const &) const
        { return false; }
      };

      using variant_t = boost::variant
      <
        null_t,
        detail::int_t,
        detail::float_t,
        bool,
        std::string,
        map_t,
        array_t
      >;
      using cstr_t = char const * const;

      /* Enum -> Type */
      template <type T>
      struct to_type;
      /* Type -> Enum */
      template <typename T>
      struct to_value;

      value() : value_(null_t{})
      { }
      template <typename T>
      explicit value(T const &val)
        : value_{ null_t{} }
      { set(val); }
      explicit value(cstr_t const str)
        : value_{ std::string{ str } }
      { }
      value(value const &copy)
        : value_{ copy.value_ }
      { }

      template <typename T>
      auto& get()
      { return boost::get<detail::normalize<T>&>(value_); }
      template <typename T>
      auto const& get() const
      { return boost::get<detail::normalize<T> const&>(value_); }

      template <typename T>
      auto& as()
      { return get<T>(); }
      template <typename T>
      auto const& as() const
      { return get<T>(); }

      /* Convenient, but not as type-safe or performant. */
      value& operator [](map_t::key_t const &key)
      {
        if(get_type() != type::map)
        { throw std::runtime_error{ "invalid value type; required map" }; }
        return as<map_t>()[key];
      }
      value& operator [](array_t::index_t const &index)
      {
        if(get_type() != type::array)
        { throw std::runtime_error{ "invalid value type; required array" }; }
        return as<array_t>()[index];
      }

      template <typename T>
      explicit operator T()
      { return as<T&>(); }
      template <typename T>
      explicit operator T() const
      { return as<T const&>(); }

      type get_type() const
      { return static_cast<type>(value_.which()); }
      bool is(type const t) const
      { return get_type() == t; }

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

      /* We can avoid superfluous copying by checking whether or not to normalize. */
      template <typename T>
      detail::enable_if<!detail::should_normalize<T>()> set(T const &val)
      { value_ = val; }
      template <typename T>
      detail::enable_if<detail::should_normalize<T>()> set(T const &val)
      { value_ = detail::normalize<T>(val); }

      /* Treat string literals as standard strings. */
      void set(cstr_t const val)
      { value_ = std::string{ val }; }

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

  using map_t = map<value, detail::parser>;
  using array_t = array<value, detail::parser>;

  inline std::ostream& operator <<(std::ostream &stream, value const &val)
  {
    switch(static_cast<value::type>(val.value_.which()))
    {
      case value::type::string:
        return (stream << "\"" << detail::escape(val.as<std::string>()) << "\"");
      case value::type::boolean:
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

  template <>
  inline std::ostream& operator <<(std::ostream &stream, array_t const &arr)
  {
    stream << arr.delim_open;
    streamjoin(arr.values_.begin(), arr.values_.end(), stream);
    stream << arr.delim_close;
    return stream;
  }

  inline std::ostream& operator <<(std::ostream &stream,
                                   map_t::internal_map_t::value_type const &p)
  { return (stream << "\"" << detail::escape(p.first) << "\":" << p.second); }

  template <>
  inline std::ostream& operator <<(std::ostream &stream, map_t const &m)
  {
    stream << m.delim_open;
    streamjoin(m.values_.begin(), m.values_.end(), stream);
    stream << m.delim_close;
    return stream;
  }

  template <>
  inline auto& value::get<value>()
  { return *this; }
  template <>
  inline auto const& value::get<value>() const
  { return *this; }
  template <>
  inline auto& value::as<value>()
  { return get<value>(); }
  template <>
  inline auto const& value::as<value>() const
  { return get<value>(); }

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
  template <>
  struct value::to_value<int8_t>
  { static type constexpr const value{ ::jeayeson::value::type::integer }; };
  template <>
  struct value::to_value<uint8_t>
  { static type constexpr const value{ ::jeayeson::value::type::integer }; };
  template <>
  struct value::to_value<int16_t>
  { static type constexpr const value{ ::jeayeson::value::type::integer }; };
  template <>
  struct value::to_value<uint16_t>
  { static type constexpr const value{ ::jeayeson::value::type::integer }; };
  template <>
  struct value::to_value<int32_t>
  { static type constexpr const value{ ::jeayeson::value::type::integer }; };
  template <>
  struct value::to_value<uint32_t>
  { static type constexpr const value{ ::jeayeson::value::type::integer }; };
  template <>
  struct value::to_value<int64_t>
  { static type constexpr const value{ ::jeayeson::value::type::integer }; };
  template <>
  struct value::to_value<uint64_t>
  { static type constexpr const value{ ::jeayeson::value::type::integer }; };
  template <>
  struct value::to_value<float>
  { static type constexpr const value{ ::jeayeson::value::type::real }; };
  template <>
  struct value::to_value<double>
  { static type constexpr const value{ ::jeayeson::value::type::real }; };
  template <>
  struct value::to_value<bool>
  { static type constexpr const value{ ::jeayeson::value::type::boolean }; };
  template <>
  struct value::to_value<std::string>
  { static type constexpr const value{ ::jeayeson::value::type::string }; };
  template <>
  struct value::to_value<map_t>
  { static type constexpr const value{ ::jeayeson::value::type::map }; };
  template <>
  struct value::to_value<array_t>
  { static type constexpr const value{ ::jeayeson::value::type::array }; };
}

using json_value = jeayeson::value;
using json_map = jeayeson::map_t;
using json_array = jeayeson::array_t;
using json_null = json_value::null_t;
using json_int = jeayeson::detail::int_t;
using json_float = jeayeson::detail::float_t;
using json_file = jeayeson::file;

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
  { return jv.get_type() == value::type::string && jv.as<std::string>() == val; }
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

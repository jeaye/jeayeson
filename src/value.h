/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: value.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <boost/variant.hpp>

#include "map.h"
#include "array.h"
#include "parser.h"

namespace jeayeson
{
  class value
  {
    public:
      /* Maps to the variant 1:1. */
      enum type_t
      {
        type_null,
        type_int8, type_uint8, type_int16, type_uint16,
        type_int32, type_uint32, type_int64, type_uint64,
        type_int, type_unsigned_int,
        type_float, type_double,
        type_bool,
        type_string,
        type_map,
        type_array
      };

      using map_t = map<value, parser>;
      using array_t = array<value, parser>;
      using null_t = struct{ };

      using variant_t = boost::variant
      <
        null_t,                                 /* Null (empty) type. */
        int8_t, uint8_t, int16_t, uint16_t,     /* Integral types. */
        int32_t, uint32_t, int64_t, uint64_t,
        int, unsigned int, 
        float, double,                          /* Floating point types. */
        bool,                                   /* Boolean types. */
        std::string,                            /* String types. */
        map_t,                                  /* Map types. */
        array_t                                 /* Array types. */
      >;
      using cstr_t = char const * const;

      value() : value_(null_t())
      { }
      value(value const &copy) : value_{ copy.value_ }
      { }
      value(value &copy) : value_{ copy.value_ }
      { }
      template <typename T>
      value(T const &val) : value_{ null_t() }
      { set(val); }
      value(cstr_t const str) : value_{ std::string(str) }
      { }

      template <typename T>
      T& get()
      { return boost::get<T&>(value_); }
      template <typename T>
      T const& get() const
      { return boost::get<T const&>(value_); }

      template <typename T>
      T& as()
      { return boost::get<T&>(value_); }
      template <typename T>
      T const& as() const
      { return boost::get<T const&>(value_); }

      template <typename T>
      operator T() 
      { return as<T&>(); }
      template <typename T>
      operator T() const
      { return as<T const&>(); }

      type_t get_type() const
      { return static_cast<type_t>(value_.which()); }
      bool is(type_t const &type) const
      { return get_type() == type; }

      template <typename T>
      bool operator ==(T const &val) const
      { return as<T>() == val; }
      bool operator ==(cstr_t const val) const
      { return as<std::string>() == val; }

      friend std::ostream& operator <<(std::ostream &stream, value const &val);

      template <typename T>
      void set(T const &val)
      { value_ = val; }

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
      case value::type_null:
        return (stream << "null");
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
}

using json_value = jeayeson::value;
using json_map = jeayeson::map_t;
using json_array = jeayeson::array_t;

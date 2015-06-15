/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: value.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <boost/variant.hpp>

#include "detail/normalize.hpp"
#include "file.hpp"
#include "data.hpp"
#include "map.hpp"
#include "array.hpp"
#include "detail/parser.hpp"
#include "detail/escape.hpp"

/* TODO: optional_cast. */
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

      template <type T>
      struct to_type;
      template <typename T, typename E = void>
      struct to_value;

      value()
        : value_{ null_t{} }
      { }
      value(value const &copy)
        : value_{ copy.value_ }
      { }

      template
      <
        typename T,
        typename E = std::enable_if_t<detail::is_convertible<T, value>()>
      >
      value(T &&val)
        : value_{ null_t{} }
      { set(std::forward<T>(val)); }

      value
      (
        std::initializer_list
        <
          std::pair<typename map_t::key_t const, value>
        > const &list
      )
        : value_{ null_t{} }
      { set(map_t{ list }); }

      template
      <
        typename T,
        typename E = std::enable_if_t<detail::is_convertible<T, value>()>
      >
      value(std::initializer_list<T> const &list)
        : value_{ null_t{} }
      { set(array_t{ list }); }

      template <typename T>
      auto& get()
      { return boost::get<detail::normalize<T>>(value_); }
      template <typename T>
      auto const& get() const
      { return boost::get<detail::normalize<T>>(value_); }

      template <typename T>
      auto& as()
      { return get<T>(); }
      template <typename T>
      auto const& as() const
      { return get<T>(); }

      /* Convenient, but not as type-safe nor performant. */
      value& operator [](map_t::key_t const &key)
      {
        if(get_type() != type::map)
        {
          throw std::runtime_error
          {
            "invalid value type (" +
            std::to_string(value_.which()) +
            "); required map"
          };
        }
        return as<map_t>()[key];
      }
      value& operator [](array_t::index_t const &index)
      {
        if(get_type() != type::array)
        {
          throw std::runtime_error
          {
            "invalid value type (" +
            std::to_string(value_.which()) +
            "); required array"
          };
        }
        return as<array_t>()[index];
      }

      template <typename T>
      explicit operator T()
      { return as<T>(); }
      template <typename T>
      explicit operator T() const
      { return as<T>(); }

      /* TODO: Rename to type() */
      type get_type() const
      { return static_cast<type>(value_.which()); }
      bool is(type const t) const
      { return get_type() == t; }

      friend bool operator ==(value const &jv, value const &val);
      template <typename T>
      friend bool operator ==(value const &jv, T const &val);
      template <typename T>
      friend bool operator ==(T const &val, value const &jv);

      friend bool operator !=(value const &jv, value const &val);
      template <typename T>
      friend bool operator !=(value const &jv, T const &val);
      template <typename T>
      friend bool operator !=(T const &val, value const &jv);
      friend std::ostream& operator <<(std::ostream &stream, value const &val);

      /* We can avoid superfluous copying by checking whether or not to normalize. */
      template <typename T>
      std::enable_if_t<!detail::should_normalize<T>()> set(T const &val)
      { value_ = val; }
      template <typename T>
      std::enable_if_t<detail::should_normalize<T>()> set(T const &val)
      { value_ = detail::normalize<T>(val); }

      void set(std::nullptr_t)
      { value_ = null_t{}; }

      /* Shortcut add for arrays. */
      template <typename T>
      void push_back(T const &val)
      { as<array_t>().push_back(val); }

      /* Shortcut add for maps. */
      template <typename T>
      void push_back(std::string const &key, T const &val)
      { as<map_t>().set(key, val); }

      template
      <
        typename T,
        typename E = std::enable_if_t<detail::is_convertible<T, value>()>
      >
      variant_t& operator =(T const &val)
      { set(val); return value_; }

    private:
      variant_t value_;
  };

  using map_t = map<value, detail::parser>;
  using array_t = array<value, detail::parser>;

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
}

#include "detail/trait.hpp"
#include "detail/stream.hpp"

using json_value = jeayeson::value;
using json_map = jeayeson::map_t;
using json_array = jeayeson::array_t;
using json_null = json_value::null_t;
using json_int = jeayeson::detail::int_t;
using json_float = jeayeson::detail::float_t;
using json_file = jeayeson::file;
using json_data = jeayeson::data;

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

  inline bool operator !=(json_value const &jv, json_value const &val)
  { return !(jv == val); }
  template <typename T>
  bool operator !=(json_value const &jv, T const &val)
  { return !(jv == val); }
  template <typename T>
  bool operator !=(T const &val, json_value const &jv)
  { return !(jv == val); }
}

/*
  Copyright © 2013 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: value.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef JEAYESON_JSONVALUE_H
#define JEAYESON_JSONVALUE_H

#include <boost/variant.hpp>

#include "map.h"
#include "array.h"
#include "parser.h"

namespace jeayeson
{
  class value
  {
    public:
#pragma mark - Types
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

      typedef map<value, parser> map_t;
      typedef array<value, parser> array_t;
      typedef struct{ } null_t;

      typedef boost::variant<
        null_t,                                 /* Null (empty) type. */
        int64_t,                                /* Integral types. */
        double,                                 /* Floating point types. */
        bool,                                   /* Boolean types. */
        std::string,                            /* String types. */
        map_t,                                  /* Map types. */
        array_t                                 /* Array types. */
                            > variant_t;
      typedef char const * const cstr_t;

#pragma mark - Construction
      /* TODO: Move semantics. */
      inline value() : m_value(null_t())
      { }
      inline value(value const &_copy) : m_value(_copy.m_value)
      { }
      inline value(value &_copy) : m_value(_copy.m_value)
      { }
      template <typename T>
      inline value(T &_value) : m_value(null_t())
      { set(_value); }
      inline value(cstr_t _str) : m_value(std::string(_str))
      { }

#pragma mark - Accessors
      template <typename T>
      inline T& get()
      { return boost::get<T&>(m_value); }
      template <typename T>
      inline T const& get() const
      { return boost::get<T const&>(m_value); }

#pragma mark - Type Checking / Conversion
      template <typename T>
      inline T& as()
      { return boost::get<T&>(m_value); }
      template <typename T>
      inline T const& as() const
      { return boost::get<T const&>(m_value); }

      template <typename T>
      inline operator T() 
      { return as<T&>(); }
      template <typename T>
      inline operator T() const
      { return as<T const&>(); }

      inline operator int8_t() 
      { return as<int64_t&>(); }
      inline operator uint8_t() 
      { return as<int64_t&>(); }
      inline operator int16_t() 
      { return as<int64_t&>(); }
      inline operator uint16_t() 
      { return as<int64_t&>(); }
      inline operator int32_t() 
      { return as<int64_t&>(); }
      inline operator uint32_t() 
      { return as<int64_t&>(); }
      inline operator long() 
      { return as<int64_t&>(); }
      inline operator unsigned long() 
      { return as<int64_t&>(); }

      inline operator float() 
      { return as<double&>(); }

      inline type_t get_type() const
      { return static_cast<type_t>(m_value.which()); }
      inline bool is(type_t const &type) const
      { return get_type() == type; }

      template <typename T>
      inline bool operator ==(T const &_value) const
      { return as<T>() == _value; }
      inline bool operator ==(cstr_t _value) const
      { return as<std::string>() == _value; }

      /* In parser.cpp */
      friend std::ostream& operator <<(std::ostream &stream, value const &_value);

#pragma mark - Mutators
      template <typename T>
      inline void set(T const &_value)
      { m_value = _value; }

      /* Mutator specializations. */
      inline void set(int8_t const _value)
      { m_value = static_cast<int64_t>(_value); }
      inline void set(uint8_t const _value)
      { m_value = static_cast<int64_t>(_value); }
      inline void set(int16_t const _value)
      { m_value = static_cast<int64_t>(_value); }
      inline void set(uint16_t const _value)
      { m_value = static_cast<int64_t>(_value); }
      inline void set(int32_t const _value)
      { m_value = static_cast<int64_t>(_value); }
      inline void set(uint32_t const _value)
      { m_value = static_cast<int64_t>(_value); }
      inline void set(long const _value)
      { m_value = static_cast<int64_t>(_value); }
      inline void set(unsigned long const _value)
      { m_value = static_cast<int64_t>(_value); }

      inline void set(float const _value)
      { m_value = static_cast<double>(_value); }

      /* Treat string literals as standard strings. */
      inline void set(cstr_t _value)
      { m_value = static_cast<std::string>(_value); }

      /* Shortcut add for arrays. */
      template <typename T>
      inline void add(T const &_value)
      { as<array_t>().add(_value); }

      /* Shortcut add for maps. */
      template <typename T>
      inline void add(std::string const &key, T const &_value)
      { as<map_t>().set(key, _value); }

      template <typename T>
      inline variant_t& operator =(T const &_value)
      { set(_value); return m_value; }

      /* Treat string literals as standard strings. */
      inline variant_t& operator =(cstr_t _value)
      { return (m_value = std::string(_value)); }

    private:
#pragma mark - Members
      variant_t m_value;

  }; /* Class value */

  typedef map<value, parser> map_t;
  typedef array<value, parser> array_t;

  inline std::ostream& operator <<(std::ostream &stream, value const &_value)
  {
    switch(_value.m_value.which())
    {
      case value::type_null:
        return (stream << "null");
      case value::type_string:
        return (stream << "\"" << _value.m_value << "\"");
      case value::type_bool:
        return (stream << (_value.as<bool>() ? "true" : "false"));
      default:
        return (stream << _value.m_value);
    }
  }

  template <typename Iter>
  inline void streamjoin(Iter _begin, Iter _end, std::ostream &_stream, std::string const &_sep = ",")
  {
    if (_begin != _end)
      _stream << *_begin++;
    while (_begin != _end)
      _stream << _sep << *_begin++;
  }

  template<>
  inline std::ostream& operator <<(std::ostream &_stream, array_t const &_arr)
  {
    _stream << _arr.delim_open;
    streamjoin(_arr.m_values.begin(), _arr.m_values.end(), _stream);
    _stream << _arr.delim_close;
    return _stream;
  }

  inline std::ostream& operator <<(std::ostream &_stream, map_t::internal_map_t::value_type const &_p)
  {
    return (_stream << "\"" << _p.first << "\":" << _p.second);
  }

  template<>
  inline std::ostream& operator <<(std::ostream &_stream, map_t const &_map)
  {
    _stream << _map.delim_open;
    streamjoin(_map.m_values.begin(), _map.m_values.end(), _stream);
    _stream << _map.delim_close;
    return _stream;
  }
} /* Namespace jeayeson */

typedef jeayeson::value json_value;
typedef jeayeson::map_t json_map;
typedef jeayeson::array_t json_array;

#endif /* JEAYESON_JSONVALUE_H */


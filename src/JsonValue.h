/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: JsonValue.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef JEAYESON_JSONVALUE_H
#define JEAYESON_JSONVALUE_H

#include <boost/variant.hpp>

#include "JsonMap.h"
#include "JsonArray.h"

namespace JeayeSON
{
  class Value
  {
    public:
      enum Type
      {
        Type_Int,
        Type_Float,
        Type_Bool,
        Type_String,
        Type_Map,
        Type_Array
      };
      typedef boost::variant<int32_t, float, bool, std::string, Map<Value>, Array<Value> > variant_t;
      typedef char const * const cstr_t;

////#pragma mark - ctors and dtor
      Value()
      { }
      virtual ~Value()
      { }

      template <typename T>
      Value(T &value) : m_value(value)
      { }
      Value(cstr_t str) : m_value(std::string(str))
      { }

//#pragma mark - accessors
      template <typename T>
      inline T& getValue()
      { return boost::get<T&>(m_value); }
      template <typename T>
      inline T const& getValue() const
      { return boost::get<T const&>(m_value); }

      template <typename T>
      inline T& as()
      { return boost::get<T&>(m_value); }
      template <typename T>
      inline T const& as() const
      { return boost::get<T const&>(m_value); }

//#pragma mark - mutators
      template <typename T>
      inline void setValue(T const &value)
      { m_value = value; }

      /* Treat string literals as standard strings. */
      inline void setValue(cstr_t value)
      { m_value = std::string(value); }

      template <typename T>
      inline variant_t& operator =(T const &value)
      { return (m_value = value); }

      /* Treat string literals as standard strings. */
      inline variant_t& operator =(cstr_t value)
      { return (m_value = std::string(value)); }

      template <typename T>
      inline bool operator ==(T const &value) const
      { return as<T>() == value; }
      inline bool operator ==(cstr_t value) const
      { return as<std::string>() == value; }

      friend std::ostream& operator <<(std::ostream &stream, Value const &value);

    private:
//#pragma mark - members
      variant_t m_value;

  }; /* Class JsonValue */

  std::ostream& operator <<(std::ostream &stream, Value const &value)
  {
    switch(value.m_value.which())
    {
      case Value::Type_String:
        return (stream << "\"" << value.m_value << "\"");
      case Value::Type_Bool:
        return (stream << (value.as<bool>() ? "true" : "false"));
      default:
        return (stream << value.m_value);
    }
  }

} /* Namespace JeayeSON */

typedef JeayeSON::Value JsonValue;
typedef JeayeSON::Map<JsonValue> JsonMap;
typedef JeayeSON::Array<JsonValue> JsonArray;

#endif /* JEAYESON_JSONVALUE_H */


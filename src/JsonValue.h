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
      typedef boost::variant<int32_t, float, bool, std::string, Map<Value>, Array<Value> > variant_t;
      typedef char const * const cstr_t;

////#pragma mark - ctors and dtor
      Value()
      { }
      virtual ~Value()
      { }

      template <typename T>
      explicit Value(T &value) : m_value(value)
      { }

//#pragma mark - accessors
      template <typename T>
      inline T& getValue()
      { return boost::get<T&>(m_value); }

      template <typename T>
      inline T& as()
      { return boost::get<T&>(m_value); }

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
      inline bool operator ==(T const &value)
      { return as<T>() == value; }
      inline bool operator ==(cstr_t value)
      { return as<std::string>() == value; }

    private:
//#pragma mark - members
      variant_t m_value;

  }; /* Class JsonValue */
} /* Namespace JeayeSON */

typedef JeayeSON::Value JsonValue;
typedef JeayeSON::Map<JsonValue> JsonMap;
typedef JeayeSON::Array<JsonValue> JsonArray;

#endif /* JEAYESON_JSONVALUE_H */


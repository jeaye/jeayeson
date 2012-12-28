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
#include "JsonParser.h"

namespace JeayeSON
{
  class Value
  {
    private:
      typedef Map<Value, Parser> map_t;
      typedef Array<Value, Parser> array_t;
      typedef struct{ } null_t;

    public:
      enum Type
      {
        Type_Null,
        Type_UInt32,
        Type_Int32,
        Type_UInt64,
        Type_Int64,
        Type_Float,
        Type_Double,
        Type_Bool,
        Type_String,
        Type_Map,
        Type_Array
      };
      typedef boost::variant<
        null_t,                                 /* Null (empty) type. */
        uint32_t, int32_t, uint64_t, int64_t,   /* Integral types. */
        float, double,                          /* Floating point types. */
        bool,                                   /* Boolean types. */
        std::string,                            /* String types. */
        map_t,                                  /* Map types. */
        array_t                                 /* Array types. */
                            > variant_t;
      typedef char const * const cstr_t;

      Value() : m_value(null_t())
      { }
      Value(Value const &copy) : m_value(copy.m_value)
      { }
      Value(Value &copy) : m_value(copy.m_value)
      { }
      template <typename T>
      Value(T &value) : m_value(value)
      { }
      Value(cstr_t str) : m_value(std::string(str))
      { }

      template <typename T>
      inline T& get()
      { return boost::get<T&>(m_value); }
      template <typename T>
      inline T const& get() const
      { return boost::get<T const&>(m_value); }

      template <typename T>
      inline T& as()
      { return boost::get<T&>(m_value); }
      template <typename T>
      inline T const& as() const
      { return boost::get<T const&>(m_value); }

      inline Type getType() const
      { return static_cast<Type>(m_value.which()); }
      inline bool isNull() const
      { return m_value.which() == Type_Null; }

      template <typename T>
      inline bool operator ==(T const &value) const
      { return as<T>() == value; }
      inline bool operator ==(cstr_t value) const
      { return as<std::string>() == value; }

      friend std::ostream& operator <<(std::ostream &stream, Value const &value);

      template <typename T>
      inline void set(T const &value)
      { m_value = value; }

      /* Treat string literals as standard strings. */
      inline void set(cstr_t value)
      { m_value = std::string(value); }

      /* Shortcut add for arrays. */
      template <typename T>
      inline void add(T const &value)
      { as< array_t >().add(value); }

      /* Shortcut add for maps. */
      template <typename T>
      inline void add(std::string const &key, T const &value)
      { as< map_t >().set(key, value); }

      template <typename T>
      inline variant_t& operator =(T const &value)
      { return (m_value = value); }

      /* Treat string literals as standard strings. */
      inline variant_t& operator =(cstr_t value)
      { return (m_value = std::string(value)); }

    private:
      variant_t m_value;

  }; /* Class JsonValue */

  inline std::ostream& operator <<(std::ostream &stream, Value const &value)
  {
    switch(value.m_value.which())
    {
      case Value::Type_Null:
        return (stream << "null");
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
typedef JeayeSON::Map<JsonValue, JeayeSON::Parser> JsonMap;
typedef JeayeSON::Array<JsonValue, JeayeSON::Parser> JsonArray;

#endif /* JEAYESON_JSONVALUE_H */


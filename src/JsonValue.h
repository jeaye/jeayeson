/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: JsonValue.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef JEAYESON_JSONVALUE_H
#define JEAYESON_JSONVALUE_H 

#include <boost/any.hpp>

namespace JeayeSON
{
  /* A JsonValue can be absolutely any type.
   * Since they have this flexibility, some 
   * interesting approaches need to be taken for
   * data storage. A JsonValue does not know what
   * type it is, so that needs to be specified when 
   * it's accessed (with as() or getValue()).
   *
   * Some special exceptions are made, such that
   * std::strings are used instead of C-strings,
   * even when C-strings are supplied. This is
   * simply for type safety.
   */
  class Value
  {
    public:
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
      { return boost::any_cast<T&>(m_value); }

      template <typename T>
      inline T& as()
      { return boost::any_cast<T&>(m_value); }

//#pragma mark - mutators
      template <typename T>
      inline void setValue(T const &value)
      { m_value = value; }

      /* Treat string literals as standard strings. */
      inline void setValue(char const * const value)
      { m_value = std::string(value); }
      inline void setValue(boost::any const &value)
      { m_value = value; }

      template <typename T>
      inline boost::any& operator =(T const &value)
      { return (m_value = value); }

      /* Treat string literals as standard strings. */
      inline boost::any& operator =(char const * const value)
      { return (m_value = std::string(value)); }

    private:
//#pragma mark - members
      boost::any m_value;

  }; /* Class JsonValue */
} /* Namespace JeayeSON */

typedef JeayeSON::Value JsonValue;

#endif /* JEAYESON_JSONVALUE_H */


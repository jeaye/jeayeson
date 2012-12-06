/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: JsonParser.cpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include "JsonParser.h"
#include "JsonValue.h"

namespace JeayeSON
{
  typedef std::string::const_iterator str_citer;

  static void parseMap(JsonMap &root, str_citer &it)
  {

  }

  static void parseArray(JsonArray &root, str_citer &it)
  {

  }

  bool IParseable::parse(std::string const &jsonString)
  {
    str_citer it = jsonString.begin();
    Value root;

    while(*it)
    {
      switch(*it)
      {
        /* Start of a new map/array. */
        case JsonMap::delimOpen:
        {
          ++it;

          root.set(JsonMap());
          parseMap(root.as<JsonMap>(), it);
        } break;

        case JsonArray::delimOpen:
        {
          ++it;

          root.set(JsonArray());
          parseArray(root.as<JsonArray>(), it);
        } break;

        /* Whitespace. */
        default:
        {
          ++it;
        } break;
      }
    }

    return true;
  }

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


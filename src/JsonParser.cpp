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
  bool IParseable::parse(std::string const &jsonString)
  {
    std::string::const_iterator it = jsonString.begin();
    std::string name;
    Value root;
    root.set(0xDEADBEEF); /* Uninitialized value */

    while(*it)
    {
      switch(*it)
      {
        /* Start of a new map/array. */
        case JsonMap::delimOpen:
        {
          /* Check if root has been created. */
          if(root.getType() != Value::Type_Map && root.getType() != Value::Type_Array)
              root.set(JsonMap());

          ++it;
        } break;
        case JsonArray::delimOpen:
        {
          /* Check if root has been created. */
          if(root.getType() != Value::Type_Map && root.getType() != Value::Type_Array)
              root.set(JsonArray());

          ++it;
        } break;

        /* End of existing map/array. */
        case JsonMap::delimClose:
        case JsonArray::delimClose:
        {

          ++it;
        } break;

        /* Whitespace. */
        default:
        {
          std::cout << *it;
          ++it;
        } break;
      }
    }

    return false;
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


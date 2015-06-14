/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: detail/escape.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <string>
#include <algorithm>

namespace jeayeson
{
  namespace detail
  {
    char constexpr in[]
    { '\"', '\\', '\b', '\f', '\n', '\r', '\t' };
    char constexpr const * const out[]
    { "\\\"", "\\\\", "\\b", "\\f", "\\n", "\\r", "\\t" };

    inline std::string escape(std::string str)
    {
      for(std::size_t i{}; i < str.size(); ++i)
      {
        auto const found(std::find(std::begin(in), std::end(in), str[i]));
        if(found == std::end(in))
        { continue; }

        str.replace(i++, 1, out[std::distance(std::begin(in), found)]);
      }
      return str;
    }

    inline char escaped(char const c)
    {
      switch(c)
      {
        case '\"':
          return '"';
        case '\\':
          return '\\';
        case '/':
          return '/';
        case 'b':
          return '\b';
        case 'f':
          return '\f';
        case 'n':
          return '\n';
        case 'r':
          return '\r';
        case 't':
          return '\t';
        default:
          return c; /* This is not well-defined. */
      }
    }
  }
}

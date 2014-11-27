/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
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
    inline bool should_escape(char const c)
    {
      return c == '\"' || c == '\\' || c == '/' ||
             c == '\b' || c == '\f' || c == '\n' ||
             c == '\r' || c == '\t';
    }

    inline std::string escape(std::string str)
    {
      for(std::size_t i{}; i < str.size(); )
      {
        auto const found(std::find_if(std::next(str.begin(), i),
                                      str.end(), &should_escape));
        if(found == str.end())
        { break; }

        str.replace(found, std::next(found), "\\\"");
        i = std::distance(str.begin(), found) + 2;
      }
      return str;
    }
  }
}

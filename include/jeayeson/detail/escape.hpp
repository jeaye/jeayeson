/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: detail/escape.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <string>

namespace jeayeson
{
  namespace detail
  {
    inline std::string escape(std::string str)
    {
      for(std::size_t start_pos{};
          ((start_pos = str.find("\"", start_pos)) != std::string::npos);
          start_pos += 2)
      { str.replace(start_pos, 1, "\\\""); }
      return str;
    }
  }
}

/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: detail/utf.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <codecvt>
#include <locale>

namespace jeayeson
{
  using str_citer = std::string::const_iterator;

  namespace detail
  {
    inline int hex_to_num(char const c)
    {
      if(c >= '0' && c <= '9')
      { return c - '0'; }
      else if(c >= 'a' && c <= 'f')
      { return c - 'a' + 10; }
      else if(c >= 'A' && c <= 'F')
      { return c - 'A' + 10; }
      return -1;
    }

    inline std::tuple<str_citer, std::string> utf16_to_8(str_citer it)
    {
      std::u16string u16;
      for( ; *it && std::equal(it, it + 2, "\\u");)
      {
        it += 2;
        int const ch
        {
          (hex_to_num(*it++) << 12) +
          (hex_to_num(*it++) << 8) +
          (hex_to_num(*it++) << 4) +
          (hex_to_num(*it++))
        };
        u16.push_back(ch);
      }

      std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,
                                  char16_t> convert;
      return { --it, convert.to_bytes(u16) };
    }
  }
}

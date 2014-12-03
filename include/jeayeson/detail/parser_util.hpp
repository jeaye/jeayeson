/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: detail/parser_util.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <codecvt>
#include <locale>

#include "../map.hpp"
#include "../array.hpp"
#include "../file.hpp"

namespace jeayeson
{
  using str_citer = std::string::const_iterator;

  namespace detail
  {
    enum class state_t { parse_name, parse_value };

    template <typename Value, typename Parser, typename T>
    state_t add(map<Value, Parser> &m, std::string const &key,
                T const &t)
    {
      m.set(key, t);
      return state_t::parse_name;
    }

    template <typename Value, typename Parser, typename T>
    state_t add(array<Value, Parser> &arr, std::string const &,
                T const &t)
    {
      arr.add(t);
      return state_t::parse_value;
    }

    template <typename Value, typename Parser>
    map<Value, Parser>& get_map(map<Value, Parser> &m,
                                std::string const &key,
                       typename array<Value, Parser>::index_t const)
    { return m.template get<map<Value, Parser>>(key); }

    template <typename Value, typename Parser>
    map<Value, Parser>& get_map(array<Value, Parser> &arr,
                                std::string const &,
                       typename array<Value, Parser>::index_t const index)
    { return arr.template get<map<Value, Parser>>(index); }

    template <typename Value, typename Parser>
    array<Value, Parser>& get_array(map<Value, Parser> &m,
                                    std::string const &key,
                           typename array<Value, Parser>::index_t const)
    { return m.template get<array<Value, Parser>>(key); }

    template <typename Value, typename Parser>
    array<Value, Parser>& get_array(array<Value, Parser> &arr,
                                    std::string const &,
                           typename array<Value, Parser>::index_t const index)
    { return arr.template get<array<Value, Parser>>(index); }

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

    inline std::tuple<str_citer, std::string> utf16(str_citer it)
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

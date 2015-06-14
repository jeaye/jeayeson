/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: detail/stream.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

namespace jeayeson
{
  inline std::ostream& operator <<(std::ostream &stream, value const &val)
  {
    switch(static_cast<value::type>(val.value_.which()))
    {
      case value::type::string:
        return (stream << "\"" << detail::escape(val.as<std::string>()) << "\"");
      case value::type::boolean:
        return (stream << (val.as<bool>() ? "true" : "false"));
      default:
        return (stream << val.value_);
    }
  }

  template <typename Iter>
  inline void streamjoin
  (
    Iter begin, Iter const end, std::ostream &stream,
    std::string const &sep = ","
  )
  {
    if(begin != end)
    { stream << *begin++; }
    while(begin != end)
    { stream << sep << *begin++; }
  }

  inline std::ostream& operator <<(std::ostream &stream, value::null_t const &)
  { return (stream << "null"); }

  template <>
  inline std::ostream& operator <<(std::ostream &stream, array_t const &arr)
  {
    stream << arr.delim_open;
    streamjoin(arr.values_.begin(), arr.values_.end(), stream);
    stream << arr.delim_close;
    return stream;
  }

  inline std::ostream& operator <<
  (
    std::ostream &stream,
    map_t::internal_map_t::value_type const &p
  )
  { return (stream << "\"" << detail::escape(p.first) << "\":" << p.second); }

  template <>
  inline std::ostream& operator <<(std::ostream &stream, map_t const &m)
  {
    stream << m.delim_open;
    streamjoin(m.values_.begin(), m.values_.end(), stream);
    stream << m.delim_close;
    return stream;
  }
}

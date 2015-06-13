#pragma once

#include <vector>
#include <string>

#include <boost/algorithm/string.hpp>

namespace jeayeson
{
  namespace detail
  {
    inline std::vector<std::string> tokenize
    (
      std::string const &source,
      std::string const &delim
    )
    {
      std::vector<std::string> tokens;
      boost::algorithm::split(tokens, source, boost::is_any_of(delim));
      return tokens;
    }
  }
}

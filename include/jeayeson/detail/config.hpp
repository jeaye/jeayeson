/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: detail/config.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <cstdint>
#include <map>

/* XXX: This is not the file you're looking for. See config.hpp,
 * not detail/config.hpp. Less important shit happens here. */

namespace jeayeson
{
  template <typename T>
  struct config;
  struct config_tag
  { };
}

#include "../config.hpp"

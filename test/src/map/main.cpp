/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/src/map/main.cpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

#include "map/ctor.hpp"
#include "map/get.hpp"
#include "map/set.hpp"
#include "map/find.hpp"
#include "map/size.hpp"
#include "map/clear.hpp"
#include "map/delim.hpp"
#include "map/has.hpp"

int main()
{
  jest::worker const j{};
  return j();
}

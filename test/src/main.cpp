/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/src/main.cpp
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

#include "array/ctor.hpp"
#include "array/get.hpp"
#include "array/set.hpp"
#include "array/find.hpp"
#include "array/size.hpp"
#include "array/clear.hpp"
#include "array/delim.hpp"

#include "parser/escape.hpp"
#include "parser/number.hpp"
#include "parser/utf.hpp"

int main()
{
  jest::worker const j{};
  return j();
}

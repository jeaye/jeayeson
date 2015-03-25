/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/src/array/main.cpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

#include "array/ctor.hpp"
#include "array/get.hpp"
#include "array/set.hpp"
#include "array/find.hpp"
#include "array/size.hpp"
#include "array/clear.hpp"
#include "array/delim.hpp"

int main()
{
  jest::worker const j{};
  return j();
}

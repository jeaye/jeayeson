/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/src/value/main.cpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

#include "value/ctor.hpp"

int main()
{
  jest::worker const j{};
  return j();
}

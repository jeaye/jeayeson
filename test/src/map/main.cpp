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
  //jest::worker const j{};
  //return j();

  /*
{
  "str": "This is str",
  "arr":
  [ 1.1, 2.2, 3.3 ],
  "person":
  {
    "name": "Tom",
    "age": 36,
    "weapon": null,
    "inventory":
    {
      "coins": 1136,
      "skooma": 7
    }
  }
}
*/
  json_map m
  {
    //{ "str", "This is a str" },
    { "str", 42 },
    //{ { "arr", json_array{ json_value{ 1.1 }, json_value{ 2.2 }, json_value{ 3.3 } } } },
    //{
    //  "person",
    //  {
    //    { "name", "Tom" },
    //    { "age", json_value{ 36 } },
    //    { "weapon", json_null{} },
    //    {
    //      "inventoy",
    //      {
    //        { "coins", json_value{ 1136 } },
    //        { "skooma", json_value{ 7 } }
    //      }
    //    }
    //  }
    //}
  };
  std::cout << m << std::endl;
}

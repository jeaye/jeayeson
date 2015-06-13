/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/src/map/main.cpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

//#include "map/ctor.hpp"
//#include "map/get.hpp"
//#include "map/set.hpp"
//#include "map/find.hpp"
//#include "map/size.hpp"
//#include "map/clear.hpp"
//#include "map/delim.hpp"
//#include "map/has.hpp"

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
  json_value vm
  {
    { "bool", true },
    { "int", 3 },
    { "real", 3.145 },
    { "str", "This is a str" },
    { "null", json_null{} },
    {
      "map",
      {
        { "sub", 42 },
        { "arr_int", { 0, 1, 2, 3, 4 } },
        { "arr_str", { "0", "1", "2", "3", "4" } },
      }
    },
  };
  std::cout << vm << std::endl;

  //json_map m
  //{
  //  {
  //    { "str", "This is a str" },
  //    { "str", 42 },
  //    { "str", 3.1415 },
  //    { "str", json_null{} },
  //  }
  //  //{ { "arr", json_array{ json_value{ 1.1 }, json_value{ 2.2 }, json_value{ 3.3 } } } },
  //  //{
  //  //  "person",
  //  //  {
  //  //    { "name", "Tom" },
  //  //    { "age", json_value{ 36 } },
  //  //    { "weapon", json_null{} },
  //  //    {
  //  //      "inventoy",
  //  //      {
  //  //        { "coins", json_value{ 1136 } },
  //  //        { "skooma", json_value{ 7 } }
  //  //      }
  //  //    }
  //  //  }
  //  //}
  //};
  //std::cout << m << std::endl;
}

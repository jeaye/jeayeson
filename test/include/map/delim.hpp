/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/map/delim.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct map_delim_test{};
  using map_delim_group = jest::group<map_delim_test>;
  static map_delim_group const map_delim_obj{ "map delim" };
}

namespace jest
{
  template <> template <>
  void jeayeson::map_delim_group::test<0>()
  {
    json_map const map{ json_file{ "test/json/map.json" } };
    expect_equal(json_map::delim_open, '{');
    expect_equal(json_map::delim_close, '}');
    expect_equal(map.delim_open, '{');
    expect_equal(map.delim_close, '}');
  }
}

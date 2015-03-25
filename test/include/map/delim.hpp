/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
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

    /* Not referencing as to be nice to the linker. */
    expect(json_map::delim_open == '{');
    expect(json_map::delim_close == '}');
    expect(map.delim_open == '{');
    expect(map.delim_close == '}');
  }
}

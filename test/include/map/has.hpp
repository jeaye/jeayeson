/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/map/has.hpp
  Author: Jakub 'kuhar' Kuderski
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct map_has_test{};
  using map_has_group = jest::group<map_has_test>;
  static map_has_group const map_has_obj{ "map has" };
}

namespace jest
{
  template <> template <>
  void jeayeson::map_has_group::test<0>()
  {
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":null},\"arr\":[0,1,2]}" };

    expect(map.has("foo"));
    expect(map.has<json_map>("foo"));

    auto &foo(map.get<json_map>("foo"));
    expect(foo.has<int>("bar"));

    expect(map.has<json_array>("arr"));
  }
}
 

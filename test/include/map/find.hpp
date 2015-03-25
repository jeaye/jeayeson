/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/map/find.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct map_find_test{};
  using map_find_group = jest::group<map_find_test>;
  static map_find_group const map_find_obj{ "map find" };
}

namespace jest
{
  template <> template <>
  void jeayeson::map_find_group::test<0>()
  {
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":43},\"baz\":null}" };
    auto it(map.find("foo"));
    expect(it != map.end());
    expect_equal(it->second.as<json_map>().get<uint32_t>("bar"), 42);

    it = map.find("doesnotexist");
    expect(it == map.end());

    it = map.find("baz");
    expect(it != map.end());
    expect_equal(it->first, "baz");
  }
}

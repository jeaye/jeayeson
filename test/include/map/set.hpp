/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/map/set.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct map_set_test{};
  using map_set_group = jest::group<map_set_test>;
  static map_set_group const map_set_obj{ "map set" };
}

namespace jest
{
  template <> template <>
  void jeayeson::map_set_group::test<0>()
  {
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":43},\"baz\":null}" };

    map.set("baz", 77);
    expect_equal(map.get<json_int>("baz"), 77);
    map.set("burp", "soda");
    expect_equal(map.get<std::string>("burp"), "soda");
  }
}

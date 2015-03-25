/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/map/size.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct map_size_test{};
  using map_size_group = jest::group<map_size_test>;
  static map_size_group const map_size_obj{ "map size" };
}

namespace jest
{
  template <> template <>
  void jeayeson::map_size_group::test<0>()
  {
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":43},\"baz\":null}" };
    expect_equal(map.size(), 2ul);
    expect(!map.empty());
  }

  template <> template <>
  void jeayeson::map_size_group::test<1>()
  {
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":43},\"baz\":null}" };
    auto foo(map.get<json_map>("foo"));
    expect_equal(foo.size(), 2ul);
    expect(!foo.empty());
  }

  template <> template <>
  void jeayeson::map_size_group::test<2>()
  {
    expect_equal(json_map{}.size(), 0ul);
    expect(json_map{}.empty());
  }
}

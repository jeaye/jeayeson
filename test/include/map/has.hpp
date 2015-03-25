/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
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

  char constexpr const * const json
  { "{\"foo\":{\"bar\":42,\"spam\":null},\"arr\":[0,1,2]}" };
}

namespace jest
{
  template <> template <>
  void jeayeson::map_has_group::test<0>() /* json_value - existent */
  {
    json_map const map{ jeayeson::json };

    expect(map.has("foo"));

    auto const &foo(map.get("foo"));
    expect(foo.as<json_map>().has("bar"));

    expect(map.has("arr"));
  }

  template <> template <>
  void jeayeson::map_has_group::test<1>() /* json_value - non-existent */
  {
    json_map const map{ jeayeson::json };

    expect(!map.has("foo blah"));

    auto const &foo(map.get("foo"));
    expect(!foo.as<json_map>().has("kitty"));

    expect(!map.has("nonexistent"));
  }

  template <> template <>
  void jeayeson::map_has_group::test<2>() /* explicit - existent */
  {
    json_map const map{ jeayeson::json };

    expect(map.has<json_map>("foo"));

    auto const &foo(map.get<json_map>("foo"));
    expect(foo.has<json_int>("bar"));

    expect(map.has<json_array>("arr"));
  }

  template <> template <>
  void jeayeson::map_has_group::test<3>() /* explicit - non-existent */
  {
    json_map const map{ jeayeson::json };

    expect(!map.has<json_int>("foo"));

    auto const &foo(map.get<json_map>("foo"));
    expect(!foo.has<json_float>("bar"));

    expect(!map.has<json_map>("arr"));
  }
}

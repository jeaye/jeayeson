/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/map/clear.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct map_clear_test{};
  using map_clear_group = jest::group<map_clear_test>;
  static map_clear_group const map_clear_obj{ "map clear" };
}

namespace jest
{
  template <> template <>
  void jeayeson::map_clear_group::test<0>()
  {
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":43},\"baz\":null}" };
    map.clear();
    expect_equal(map.size(), 0ul);
    expect(map.find("foo") == map.end());
    map.reset("{\"foo\": \"bar\"}");
    expect_equal(map.get<std::string>("foo"), "bar");
    map.reset("{\"foo\":{\"bar\":42,\"spam\":43},\"baz\":null}");
    expect_equal(map.get_for_path<json_int>("foo.spam"), 43);

    expect(map.find("baz") != map.end());
    map.erase("baz");
    expect(map.find("baz") == map.end());
    expect_equal(map.get_for_path<json_int>("foo.spam"), 43);
  }
}

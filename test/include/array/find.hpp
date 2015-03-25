/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/array/find.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct array_find_test{};
  using array_find_group = jest::group<array_find_test>;
  static array_find_group const array_find_obj{ "array find" };
}

namespace jest
{
  template <> template <>
  void jeayeson::array_find_group::test<0>()
  {
    json_array arr{ json_file{ "test/json/array.json" } };
    auto it(arr.find("five"));
    expect(it != arr.end());
    expect_equal(it->as<std::string>(), "five");

    it = arr.find("doesnotexist");
    expect(it == arr.end());

    it = arr.find(json_value{});
    expect(it != arr.end());
  }
}

/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/array/set.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct array_set_test{};
  using array_set_group = jest::group<array_set_test>;
  static array_set_group const array_set_obj{ "array set" };
}

namespace jest
{
  template <> template <>
  void jeayeson::array_set_group::test<0>()
  {
    json_array arr{ json_file{ "test/json/array.json" } };

    arr.set(0, 77);
    expect_equal(arr.get<int64_t>(0), 77);
    arr.set(0, "soda");
    expect_equal(arr.get<std::string>(0), "soda");

    expect_equal(arr.size(), 9ul);
    arr.push_back(3.14159f);
    expect_equal(arr.size(), 10ul);
    arr.push_back(json_null{});
    arr.push_back(json_map{"{ \"foo\": true }"});
    expect_equal(arr.size(), 12ul);
    expect_equal(arr[arr.size() - 1].as<json_map>().get<bool>("foo"), true);
  }
}

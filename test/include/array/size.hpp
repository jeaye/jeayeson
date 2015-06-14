/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/array/size.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct array_size_test{};
  using array_size_group = jest::group<array_size_test>;
  static array_size_group const array_size_obj{ "array size" };
}

namespace jest
{
  template <> template <>
  void jeayeson::array_size_group::test<0>()
  {
    json_array arr{ json_file{ "test/json/array.json" } };
    expect_equal(arr.size(), 9ul);
    expect(!arr.empty());

    auto subarr(arr.get<json_array>(6));
    expect_equal(subarr.size(), 0ul);
    expect(subarr.empty());

    expect_equal(json_array{}.size(), 0ul);
    expect(json_array{}.empty());
  }
}

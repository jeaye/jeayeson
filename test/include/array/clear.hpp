/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/array/clear.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct array_clear_test{};
  using array_clear_group = jest::group<array_clear_test>;
  static array_clear_group const array_clear_obj{ "array clear" };
}

namespace jest
{
  template <> template <>
  void jeayeson::array_clear_group::test<0>()
  {
    json_array arr{ json_file{ "test/json/array.json" } };
    arr.clear();
    expect_equal(arr.size(), 0ul);
    expect(arr.find(1) == arr.end());
    arr.reset("[\"foo\",\"bar\"]");
    expect_equal(arr.get<std::string>(1), "bar");
    arr.reset("[\"foo\",\"spam\"]");
    expect_equal(arr.get(1), "spam");

    expect(arr.find("foo") != arr.end());
    arr.erase(0);
    expect(arr.find("foo") == arr.end());

    arr.push_back(42.0f);
    auto const it(arr.find(42.0f));
    expect(it != arr.end());
    arr.erase(it);
    expect(arr.find(42.0f) == arr.end());
    expect_equal(arr.size(), 1ul);

    arr.push_back("bar");
    expect_equal(arr.size(), 2ul);
    arr.erase(arr.begin(), arr.begin() + 1);
    expect_equal(arr.size(), 1ul);
    expect_equal(arr.get<std::string>(0), "bar");
  }
}

/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/array/ctor.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct array_ctor_test{};
  using array_ctor_group = jest::group<array_ctor_test>;
  static array_ctor_group const array_ctor_obj{ "array ctor" };
}

namespace jest
{
  template <> template <>
  void jeayeson::array_ctor_group::test<0>() /* default */
  {
    json_array arr;
    expect_equal(arr.size(), 0ul);
    expect_equal(arr.to_string(), "[]");

    json_array const carr;
    expect_equal(carr.size(), 0ul);
    expect_equal(carr.to_string(), "[]");
  }

  template <> template <>
  void jeayeson::array_ctor_group::test<1>() /* json */
  {
    json_array arry{ json_data{ "[ \"foo\", \"bar\" ]" } };
    expect_equal(arry.size(), 2ul);
    expect_equal(arry.to_string(), "[\"foo\",\"bar\"]" );
  }

  template <> template <>
  void jeayeson::array_ctor_group::test<2>() /* container */
  {
    std::vector<json_float> v{ 0.0, 1.0 };
    json_array arr{ v.begin(), v.end() };
    expect_equal(arr.size(), 2ul);
    expect_almost_equal(arr.get<json_float>(0), 0.0);
    expect_almost_equal(arr.get<json_float>(1), 1.0);
    expect_almost_equal(arr[0].as<json_float>(), 0.0);
    expect_almost_equal(arr[1].as<json_float>(), 1.0);
  }

  template <> template <>
  void jeayeson::array_ctor_group::test<3>() /* initializer_list */
  {
    json_array arr
    {
      0.0, 1.0,
    };
    expect_equal(arr.size(), 2ul);
    expect_almost_equal(arr.get<json_float>(0), 0.0);
    expect_almost_equal(arr.get<json_float>(1), 1.0);
    expect_almost_equal(arr[0].as<json_float>(), 0.0);
    expect_almost_equal(arr[1].as<json_float>(), 1.0);
  }
}

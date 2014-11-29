/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/parser/number.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct number_test{};
  using number_group = jest::group<number_test>;
  static number_group const number_obj{ "number" };
}

namespace jest
{
  template <> template <>
  void jeayeson::number_group::test<0>() /* int */
  {
    json_map const map{ R"raw({"foo":42, "bar":-193847, "zero": 0})raw" };
    expect_equal(map.get<json_int>("foo"), 42);
    expect_equal(map.get<json_int>("bar"), -193847);
    expect_equal(map.get<json_int>("zero"), 0);

    json_array const arr{ R"raw([42, -193847, 0])raw" };
    expect_equal(arr.get<json_int>(0), 42);
    expect_equal(arr.get<json_int>(1), -193847);
    expect_equal(arr.get<json_int>(2), 0);
  }

  /* TODO: floating point */
  template <> template <>
  void jeayeson::number_group::test<1>()
  {
  }
}

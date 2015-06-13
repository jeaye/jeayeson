/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
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

    json_array const arr{ json_data{ R"raw([42, -193847, 0])raw" } };
    expect_equal(arr.get<json_int>(0), 42);
    expect_equal(arr.get<json_int>(1), -193847);
    expect_equal(arr.get<json_int>(2), 0);
  }

  template <> template <>
  void jeayeson::number_group::test<1>() /* floating point */
  {
    json_map const map{ R"raw({"foo":42.42, "bar":-193847.07, "zero":-0.0})raw" };
    expect_almost_equal(map.get<json_float>("foo"), 42.42);
    expect_almost_equal(map.get<json_float>("bar"), -193847.07);
    expect_almost_equal(map.get<json_float>("zero"), 0.0);

    json_array const arr{ json_data{ R"raw([42.42, -193847.07, -0.0])raw" } };
    expect_almost_equal(arr.get<json_float>(0), 42.42);
    expect_almost_equal(arr.get<json_float>(1), -193847.07);
    expect_almost_equal(arr.get<json_float>(2), 0.0);
  }

  template <> template <>
  void jeayeson::number_group::test<2>() /* exponential: e */
  {
    json_map const map
    { R"raw({"foo":1.1e10, "bar":7.07e+15, "zero":2.123e-3})raw" };
    expect_almost_equal(map.get<json_float>("foo"), 1.1e10);
    expect_almost_equal(map.get<json_float>("bar"), 7.07e+15);
    expect_almost_equal(map.get<json_float>("zero"), 2.123e-3);

    json_array const arr
    { json_data{ R"raw([1.1e10, 7.07e+15, 2.123e-3])raw" } };
    expect_almost_equal(arr.get<json_float>(0), 1.1e10);
    expect_almost_equal(arr.get<json_float>(1), 7.07e+15);
    expect_almost_equal(arr.get<json_float>(2), 2.123e-3);
  }

  template <> template <>
  void jeayeson::number_group::test<3>() /* exponential: E */
  {
    json_map const map
    { R"raw({"foo":1.1E10, "bar":7.07E+15, "zero":2.123E-3})raw" };
    expect_almost_equal(map.get<json_float>("foo"), 1.1E10);
    expect_almost_equal(map.get<json_float>("bar"), 7.07E+15);
    expect_almost_equal(map.get<json_float>("zero"), 2.123E-3);

    json_array const arr
    { json_data{ R"raw([1.1E10, 7.07E+15, 2.123E-3])raw" } };
    expect_almost_equal(arr.get<json_float>(0), 1.1E10);
    expect_almost_equal(arr.get<json_float>(1), 7.07E+15);
    expect_almost_equal(arr.get<json_float>(2), 2.123E-3);
  }
}

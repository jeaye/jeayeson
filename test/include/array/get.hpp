/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/array/get.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct array_get_test{};
  using array_get_group = jest::group<array_get_test>;
  static array_get_group const array_get_obj{ "array get" };
}

namespace jest
{
  template <> template <>
  void jeayeson::array_get_group::test<0>() /* references */
  {
    std::vector<json_float> v{ 0.0, 1.0 };
    json_array arr{ v.begin(), v.end() };
    json_float &z(arr.get<json_float>(0));
    json_float &o(arr[1].as<json_float>());
    z = 42.0;
    o = -42.0;
    expect_equal(arr.size(), 2ul);
    expect_almost_equal(arr[0].as<json_float>(), 42.0);
    expect_almost_equal(arr.get<json_float>(1), -42.0);
  }

  template <> template <>
  void jeayeson::array_get_group::test<1>() /* fallbacks */
  {
    std::vector<json_float> v{ 0.0, 1.0 };
    json_array arr{ v.begin(), v.end() };
    json_float z(arr.get(0, 77.0));
    json_float o(arr.get(1, 77.0));
    json_float t(arr.get(2, 77.0));
    expect_almost_equal(z, 0.0);
    expect_almost_equal(o, 1.0);
    expect_almost_equal(t, 77.0);

    z = 42.0;
    o = -42.0;
    expect_equal(arr.size(), 2ul);
    expect_almost_equal(arr.get<json_float>(0), 0.0);
    expect_almost_equal(arr.get<json_float>(1), 1.0);

    std::vector<std::string> v2{ "foo", "test" };
    json_array sarr{ v2.begin(), v2.end() };
    expect_equal(sarr.size(), 2ul);
    expect_equal(sarr.get(1, "fallback"), "test");
    expect_equal(sarr.get(5, "fallback"), "fallback");
  }

  template <> template <>
  void jeayeson::array_get_group::test<2>() /* named */
  {
    auto arr(json_array{ json_file{ "test/json/array.json" } });

    auto &foo(arr.get<json_map>(5));
    expect_equal(foo.get("success"), true);

    expect_equal(arr.size(), 9ul);
    expect_equal(arr[1].as<int>(), 2);
    expect_equal(arr[3].as<int16_t>(), 4);
  }

  template <> template <>
  void jeayeson::array_get_group::test<3>() /* null */
  {
    auto arr(json_array{ json_file{ "test/json/array.json" } });
    expect(arr[6] != json_null{});
    expect(arr[7] == json_null{});
  }

  template <> template <>
  void jeayeson::array_get_group::test<4>() /* normalization */
  {
    auto arr(json_array{ json_file{ "test/json/array.json" } });
    expect_equal(arr[1].as<int8_t>(), 2);
    expect_equal(arr[1].as<int16_t>(), 2);
    expect_equal(arr[1].as<uint64_t>(), 2);

    expect_almost_equal(arr[8].as<float>(), -4.8901);
    expect_almost_equal(arr[8].as<double>(), -4.8901);

    expect_equal(arr[4].as<decltype("five")>(), "five");
  }
}

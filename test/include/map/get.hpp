/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/map/get.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct map_get_test{};
  using map_get_group = jest::group<map_get_test>;
  static map_get_group const map_get_obj{ "map get" };
}

namespace jest
{
  template <> template <>
  void jeayeson::map_get_group::test<0>() /* references */
  {
    std::map<std::string, json_float> sm{ { "0", 0.0 }, { "1", 1.0 } };
    json_map map{ sm };
    json_float &z(map.get<json_float>("0"));
    json_float &o(map.get<json_float>("1"));
    z = 42.0;
    o = -42.0;
    expect_equal(map.size(), 2ul);
    expect_almost_equal(map.get<json_float>("0"), 42.0);
    expect_almost_equal(map.get<json_float>("1"), -42.0);
  }

  template <> template <>
  void jeayeson::map_get_group::test<1>() /* fallbacks */
  {
    std::map<std::string, json_float> sm{ { "0", 0.0 }, { "1", 1.0 } };
    json_map map{ sm };
    json_float z(map.get("0", 77.0));
    json_float o(map.get("1", 77.0));
    json_float t(map.get("2", 77.0));
    expect_almost_equal(z, 0.0);
    expect_almost_equal(o, 1.0);
    expect_almost_equal(t, 77.0);

    z = 42.0;
    o = -42.0;
    expect_equal(map.size(), 2ul);
    expect_almost_equal(map.get<json_float>("0"), 0.0);
    expect_almost_equal(map.get<json_float>("1"), 1.0);

    std::map<std::string, std::string> sm2{ { "foo", "test" } };
    json_map smap{ sm2 };
    expect_equal(smap.size(), 1ul);
    expect_equal(smap.get("foo", "fallback"), "test");
    expect_equal(smap.get("bar", "fallback"), "fallback");
  }

  template <> template <>
  void jeayeson::map_get_group::test<2>() /* named */
  {
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":null},\"arr\":[0,1,2]}" };

    auto &foo(map.get<json_map>("foo"));
    expect_equal(foo.get<json_int>("bar"), 42);

    auto &arr(map.get<json_array>("arr"));
    expect_equal(arr.size(), 3ul);
    for(int i{}; i < 3; ++i)
    { expect_equal(arr[i].as<json_int>(), i); }
  }

  template <> template <>
  void jeayeson::map_get_group::test<3>() /* paths */
  {
    json_map map{ json_file{ "test/json/map.json" } };

    expect_almost_equal(map.get_for_path<json_float>("doesnotexist", 77.0), 77.0);
    expect_equal(map.get_for_path<std::string>("does.not.exist", "nope"), "nope");

    expect_equal(map.get_for_path<json_int>("person.inventory.coins"), 1136);
    expect_equal(map.get_for_path("person.inventory.skooma"), 7);
    expect_equal(map.get_for_path<json_int>("person.inventory.zzz", 42), 42);
    expect_equal(map.get_for_path("person.name"), "Roger");
    expect_equal(map.get_for_path<std::string>("person.notname", "zzz"), "zzz");
    expect_equal(map.get_for_path("ends.with.", 42), 42);
    expect_almost_equal
    (map.get_for_path<json_array>("arr")[8].as<json_float>(), 9.9);
  }

  template <> template <>
  void jeayeson::map_get_group::test<4>() /* null */
  {
    auto const map(json_map{ json_file{ "test/json/map.json" } });
    expect(map.get("null") == json_null{});
    expect(map.get("str") != json_null{});
  }

  template <> template <>
  void jeayeson::map_get_group::test<5>() /* normalization */
  {
    auto const map(json_map{ json_file{ "test/json/map.json" } });
    expect_equal(map.get<int8_t>("num", 0), 5000);
    expect_equal(map.get<int16_t>("num", 0), 5000);
    expect_equal(map.get<uint64_t>("num", 0), 5000);

    expect_almost_equal(map.get<float>("real", 0.0f), -3.14159);
    expect_almost_equal(map.get<double>("real", 0.0), -3.14159);

    expect_equal(map.get<decltype("Roger")>("str"), "This \"is\" a str");
  }
}

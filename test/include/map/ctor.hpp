/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/map/ctor.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct map_ctor_test{};
  using map_ctor_group = jest::group<map_ctor_test>;
  static map_ctor_group const map_ctor_obj{ "map ctor" };
}

namespace jest
{
  template <> template <>
  void jeayeson::map_ctor_group::test<0>() /* default */
  {
    { /* default ctor */
      json_map map;
      expect_equal(map.size(), 0ul);
      expect_equal(map.to_string(), "{}");

      json_map const cmap;
      expect_equal(cmap.size(), 0ul);
      expect_equal(cmap.to_string(), "{}");
    }
  }

  template <> template <>
  void jeayeson::map_ctor_group::test<1>() /* json */
  {
    json_map map{ "{ \"foo\": \"bar\" }" };
    expect_equal(map.size(), 1ul);
    expect_equal(map.to_string(), "{\"foo\":\"bar\"}" );
  }

  template <> template <>
  void jeayeson::map_ctor_group::test<2>() /* map */
  {
    std::map<std::string, json_float> sm{ { "0", 0.0 }, { "1", 1.0 } };
    json_map map{ sm };
    expect_equal(map.size(), 2ul);
    expect_almost_equal(map.get<json_float>("0"), 0.0);
    expect_almost_equal(map.get<json_float>("1"), 1.0);
  }

  template <> template <>
  void jeayeson::map_ctor_group::test<3>() /* initializer_list */
  {
    json_map map
    {
      { "0", 0.0 },
      { "1", 1.0 },
      {
        "sub_map",
        {
          { "name", "foo" },
          {
            "arr", { 0.0, 1.0, 2.0 }
          },
        }
      }
    };
    expect_equal(map.size(), 3ul);
    expect_almost_equal(map.get<json_float>("0"), 0.0);
    expect_almost_equal(map.get<json_float>("1"), 1.0);
  }
}

/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/value/ctor.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct value_ctor_test{};
  using value_ctor_group = jest::group<value_ctor_test>;
  static value_ctor_group const value_ctor_obj{ "value ctor" };
}

namespace jest
{
  template <> template <>
  void jeayeson::value_ctor_group::test<0>() /* initializer_list for map */
  {
    json_value vm
    {
      { "bool", true },
      { "int", 3 },
      { "real", 3.145 },
      { "str", "This is a str" },
      { "null", json_null{} },
      { "nullptr", nullptr },
      {
        "map",
        {
          { "sub", 42 },
          { "arr_int", { 0, 1, 2, 3, 4 } },
          { "arr_str", { "0", "1", "2", "3", "4" } },
        }
      },
    };
    expect(vm.get_type() == json_value::type::map);
    expect_equal(vm.as<json_map>().size(), 7ul);
  }

  template <> template <>
  void jeayeson::value_ctor_group::test<1>() /* initializer_list for array */
  {
    json_value vm
    {
      "str0", "str1", "str2"
    };
    expect(vm.get_type() == json_value::type::array);
    expect_equal(vm.as<json_array>().size(), 3ul);
  }
}

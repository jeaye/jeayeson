/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/array/delim.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct array_delim_test{};
  using array_delim_group = jest::group<array_delim_test>;
  static array_delim_group const array_delim_obj{ "array delim" };
}

namespace jest
{
  template <> template <>
  void jeayeson::array_delim_group::test<0>()
  {
    json_array const arr{ json_file{ "test/json/array.json" } };

    /* Not referencing as to be nice to the linker. */
    expect(json_array::delim_open == '[');
    expect(json_array::delim_close == ']');
    expect(arr.delim_open == '[');
    expect(arr.delim_close == ']');
  }
}

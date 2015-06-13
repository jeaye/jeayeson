/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/parser/utf.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct utf_test{};
  using utf_group = jest::group<utf_test>;
  static utf_group const utf_obj{ "utf" };
}

namespace jest
{
  template <> template <>
  void jeayeson::utf_group::test<0>() /* case insensitivity */
  {
    json_map const map
    { R"raw({"\u007B":"\u007b", "\u006e":"\u006E"})raw" };
    expect_equal(map["{"], "{");
    expect_equal(map["n"], "n");

    json_array const arr
    { json_data{ R"raw(["\u007B", "\u007b", "\u007e", "\u007E"])raw" } };
    expect_equal(arr[0], arr[1], "{");
    expect_equal(arr[2], arr[3], "~");
  }

  template <> template <>
  void jeayeson::utf_group::test<1>() /* ASCII */
  {
    json_map const map
    { R"raw({"\u0030\u007C":"\u007E"})raw" };
    expect_equal(map["0|"], "~");

    json_array const arr
    { json_data{ R"raw(["\u0030", "\u007C\u007B", "\u007E"])raw" } };
    expect_equal(arr[0], "0");
    expect_equal(arr[1], "|{");
    expect_equal(arr[2], "~");
  }

  template <> template <>
  void jeayeson::utf_group::test<2>() /* single code point */
  {
    json_map const map
    { R"raw({"\u0174\u0173":"\u0166\u0158"})raw" };
    expect_equal(map["Ŵų"], "ŦŘ");

    json_array const arr
    { json_data{ R"raw(["\u0174", "\u0173\u00A9", "\u0158"])raw" } };
    expect_equal(arr[0], "Ŵ");
    expect_equal(arr[1], "ų©");
    expect_equal(arr[2], "Ř");
  }

  template <> template <>
  void jeayeson::utf_group::test<3>() /* surrogate pairs */
  {
    json_map const map
    { R"raw({"\uD83D\uDE0A":"\uD83D\uDE09\uD83D\uDE0A"})raw" };
    expect_equal(map["😊"], "😉😊");

    json_array const arr
    {
      json_data
      {
        R"raw(["\uD83D\uDE0E", "\uD83D\uDE34\uD83D\uDE36", "\uD83D\uDE3A"])raw"
      }
    };
    expect_equal(arr[0], "😎");
    expect_equal(arr[1], "😴😶");
    expect_equal(arr[2], "😺");
  }

  template <> template <>
  void jeayeson::utf_group::test<4>() /* mixed */
  {
    json_map const map
    { R"raw({"\u00A9\uD83D\uDE0A":"\uD83D\uDE09\u0158\uD83D\uDE0A"})raw" };
    expect_equal(map["©😊"], "😉Ř😊");

    json_array const arr
    {
      json_data
      {
        R"raw(["\u00A9\uD83D\uDE0E",
               "\uD83D\uDE34\u0174\uD83D\uDE36",
               "\uD83D\uDE3A\u0166"])raw"
      }
    };
    expect_equal(arr[0], "©😎");
    expect_equal(arr[1], "😴Ŵ😶");
    expect_equal(arr[2], "😺Ŧ");
  }
}

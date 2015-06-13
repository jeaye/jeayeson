/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: test/include/parser/escape.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <jeayeson/jeayeson.hpp>
#include <jest/jest.hpp>

namespace jeayeson
{
  struct escape_test{};
  using escape_group = jest::group<escape_test>;
  static escape_group const escape_obj{ "escape" };
}

namespace jest
{
  template <> template <>
  void jeayeson::escape_group::test<0>() /* \" */
  {
    json_map map{ R"raw({ "foo\"": "bar\"" })raw" };
    expect_equal(map.to_string(), R"raw({"foo\"":"bar\""})raw" );
    map.clear();
    map["spam"] = R"raw(""")raw";
    expect_equal(map.to_string(), R"raw({"spam":"\"\"\""})raw" );

    json_array arr{ json_data{ R"raw([ "foo\"", "bar\"" ])raw" } };
    expect_equal(arr.to_string(), R"raw(["foo\"","bar\""])raw" );
    arr.clear();
    arr.push_back(R"raw(""")raw");
    expect_equal(arr.to_string(), R"raw(["\"\"\""])raw" );
  }

  template <> template <>
  void jeayeson::escape_group::test<1>() /* \\ */
  {
    json_map map{ R"raw({ "foo\\": "bar\\" })raw" };
    expect_equal(map.to_string(), R"raw({"foo\\":"bar\\"})raw" );
    map.clear();
    map["spam"] = R"raw(\\\)raw";
    expect_equal(map.to_string(), R"raw({"spam":"\\\\\\"})raw" );

    json_array arr{ json_data{ R"raw([ "foo\\", "bar\\" ])raw" } };
    expect_equal(arr.to_string(), R"raw(["foo\\","bar\\"])raw" );
    arr.clear();
    arr.push_back(R"raw(\\\)raw");
    expect_equal(arr.to_string(), R"raw(["\\\\\\"])raw" );
  }

  template <> template <> /* XXX: No longer escaped. */
  void jeayeson::escape_group::test<2>() /* / */
  {
    json_map map{ R"raw({ "/foo/": "/bar/" })raw" };
    expect_equal(map.to_string(), R"raw({"/foo/":"/bar/"})raw" );
    map.clear();
    map["spam"] = R"raw(///)raw";
    expect_equal(map.to_string(), R"raw({"spam":"///"})raw" );

    json_array arr{ json_data{ R"raw([ "/foo/", "/bar/" ])raw" } };
    expect_equal(arr.to_string(), R"raw(["/foo/","/bar/"])raw" );
    arr.clear();
    arr.push_back(R"raw(///)raw");
    expect_equal(arr.to_string(), R"raw(["///"])raw" );
  }

  template <> template <>
  void jeayeson::escape_group::test<3>() /* \b */
  {
    json_map map{ "{ \"\bfoo\b\": \"\bbar\b\" }" };
    expect_equal(map.to_string(), R"raw({"\bfoo\b":"\bbar\b"})raw" );
    map.clear();
    map["spam"] = "\b\b\b";
    expect_equal(map.to_string(), R"raw({"spam":"\b\b\b"})raw" );

    json_array arr{ json_data{ "[ \"\bfoo\b\", \"\bbar\b\" ]" } };
    expect_equal(arr.to_string(), R"raw(["\bfoo\b","\bbar\b"])raw" );
    arr.clear();
    arr.push_back("\b\b\b");
    expect_equal(arr.to_string(), R"raw(["\b\b\b"])raw" );
  }

  template <> template <>
  void jeayeson::escape_group::test<4>() /* \f */
  {
    json_map map{ "{ \"\ffoo\f\": \"\fbar\f\" }" };
    expect_equal(map.to_string(), R"raw({"\ffoo\f":"\fbar\f"})raw" );
    map.clear();
    map["spam"] = "\f\f\f";
    expect_equal(map.to_string(), R"raw({"spam":"\f\f\f"})raw" );

    json_array arr{ json_data{ "[ \"\ffoo\f\", \"\fbar\f\" ]" } };
    expect_equal(arr.to_string(), R"raw(["\ffoo\f","\fbar\f"])raw" );
    arr.clear();
    arr.push_back("\f\f\f");
    expect_equal(arr.to_string(), R"raw(["\f\f\f"])raw" );
  }

  template <> template <>
  void jeayeson::escape_group::test<5>() /* \n */
  {
    json_map map{ "{ \"\nfoo\n\": \"\nbar\n\" }" };
    expect_equal(map.to_string(), R"raw({"\nfoo\n":"\nbar\n"})raw" );
    map.clear();
    map["spam"] = "\n\n\n";
    expect_equal(map.to_string(), R"raw({"spam":"\n\n\n"})raw" );

    json_array arr{ json_data{ "[ \"\nfoo\n\", \"\nbar\n\" ]" } };
    expect_equal(arr.to_string(), R"raw(["\nfoo\n","\nbar\n"])raw" );
    arr.clear();
    arr.push_back("\n\n\n");
    expect_equal(arr.to_string(), R"raw(["\n\n\n"])raw" );
  }

  template <> template <>
  void jeayeson::escape_group::test<6>() /* \r */
  {
    json_map map{ "{ \"\rfoo\r\": \"\rbar\r\" }" };
    expect_equal(map.to_string(), R"raw({"\rfoo\r":"\rbar\r"})raw" );
    map.clear();
    map["spam"] = "\r\r\r";
    expect_equal(map.to_string(), R"raw({"spam":"\r\r\r"})raw" );

    json_array arr{ json_data{ "[ \"\rfoo\r\", \"\rbar\r\" ]" } };
    expect_equal(arr.to_string(), R"raw(["\rfoo\r","\rbar\r"])raw" );
    arr.clear();
    arr.push_back("\r\r\r");
    expect_equal(arr.to_string(), R"raw(["\r\r\r"])raw" );
  }

  template <> template <>
  void jeayeson::escape_group::test<7>() /* \t */
  {
    json_map map{ "{ \"\tfoo\t\": \"\tbar\t\" }" };
    expect_equal(map.to_string(), R"raw({"\tfoo\t":"\tbar\t"})raw" );
    map.clear();
    map["spam"] = "\t\t\t";
    expect_equal(map.to_string(), R"raw({"spam":"\t\t\t"})raw" );

    json_array arr{ json_data{ "[ \"\tfoo\t\", \"\tbar\t\" ]" } };
    expect_equal(arr.to_string(), R"raw(["\tfoo\t","\tbar\t"])raw" );
    arr.clear();
    arr.push_back("\t\t\t");
    expect_equal(arr.to_string(), R"raw(["\t\t\t"])raw" );
  }
}

/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: tests/array.cpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <iostream>
#include <string>
#include <cassert>

#include "jeayeson.hpp"

void test_ctor()
{
  std::cout << "test_ctor" << std::endl;
  { /* default ctor */
    json_array arr;
    assert(arr.size() == 0);
    assert(arr.to_string() == "[]");

    json_array const carr;
    assert(carr.size() == 0);
    assert(carr.to_string() == "[]");
  }
  { /* json ctor */
    json_array arry{ "[ \"foo\", \"bar\" ]" };
    assert(arry.size() == 2);
    assert(arry.to_string() ==  "[\"foo\",\"bar\"]" );
  }
  { /* container ctor */
    std::vector<json_float> v{ 0.0, 1.0 };
    json_array arr{ v.begin(), v.end() };
    assert(arr.size() == 2);
    assert(arr.get<json_float>(0) == 0.0);
    assert(arr.get<json_float>(1) == 1.0);
    assert(arr[0].as<json_float>() == 0.0);
    assert(arr[1].as<json_float>() == 1.0);
  }
  std::cout << "end test_ctor" << std::endl;
}

void test_get()
{
  std::cout << "test_get" << std::endl;
  { /* References */
    std::cout << "test_get ref" << std::endl;
    std::vector<json_float> v{ 0.0, 1.0 };
    json_array arr{ v.begin(), v.end() };
    json_float &z(arr.get<json_float>(0));
    json_float &o(arr[1].as<json_float>());
    z = 42.0;
    o = -42.0;
    assert(arr.size() == 2);
    assert(arr[0].as<json_float>() == 42.0);
    assert(arr.get<json_float>(1) == -42.0);
  }
  { /* Fallbacks */
    std::cout << "test_get fall" << std::endl;
    std::vector<json_float> v{ 0.0, 1.0 };
    json_array arr{ v.begin(), v.end() };
    json_float z(arr.get(0, 77.0));
    json_float o(arr.get(1, 77.0));
    json_float t(arr.get(2, 77.0));
    assert(z == 0.0);
    assert(o == 1.0);
    assert(t == 77.0);

    z = 42.0;
    o = -42.0;
    assert(arr.size() == 2);
    assert(arr.get<json_float>(0) == 0.0);
    assert(arr.get<json_float>(1) == 1.0);

    std::vector<std::string> v2{ "foo", "test" };
    json_array sarr{ v2.begin(), v2.end() };
    assert(sarr.size() == 2);
    assert(sarr.get(1, "fallback") == "test");
    assert(sarr.get(5, "fallback") == "fallback");
  }
  { /* Named */
    std::cout << "test_get named" << std::endl;
    auto arr(json_array{ json_file{ "src/tests/json/array.json" } });

    auto &foo(arr.get<json_map>(5));
    assert(foo.get("success") == true);

    assert(arr.size() == 8);
    assert(arr[1].as<int>() == 2);
    assert(arr[3].as<int16_t>() == 4);
  }
  { /* Null */
    std::cout << "test_get null" << std::endl;
    auto arr(json_array{ json_file{ "src/tests/json/array.json" } });
    assert(arr[6] != json_null{});
    assert(arr[7] == json_null{});
  }
  std::cout << "end test_get" << std::endl;
}

void test_size()
{
  std::cout << "test_size" << std::endl;
  {
    json_array arr{ json_file{ "src/tests/json/array.json" } };
    assert(arr.size() == 8);
    assert(!arr.empty());

    auto subarr(arr.get<json_array>(6));
    assert(subarr.size() == 0);
    assert(subarr.empty());

    assert(json_array{}.size() == 0);
    assert(json_array{}.empty());
  }
  std::cout << "end test_size" << std::endl;
}

void test_find()
{
  std::cout << "test_find" << std::endl;
  {
    json_array arr{ json_file{ "src/tests/json/array.json" } };
    auto it(arr.find("five"));
    assert(it != arr.end());
    assert(it->as<std::string>() == "five");

    it = arr.find("doesnotexist");
    assert(it == arr.end());

    it = arr.find(json_value{});
    assert(it != arr.end());
  }
  std::cout << "end test_find" << std::endl;
}

void test_set()
{
  std::cout << "test_set" << std::endl;
  {
    json_array arr{ json_file{ "src/tests/json/array.json" } };

    arr.set(0, 77);
    assert(arr.get<int64_t>(0) == 77);
    arr.set(0, "soda");
    assert(arr.get<std::string>(0) == "soda");

    assert(arr.size() == 8);
    arr.push_back(3.14159f);
    assert(arr.size() == 9);
    arr.push_back(json_null{});
    arr.push_back(json_map{"{ \"foo\": true }"});
    assert(arr.size() == 11);
    assert(arr[arr.size() - 1].as<json_map>().get<bool>("foo") == true);
  }
  std::cout << "end test_set" << std::endl;
}

void test_clear()
{
  std::cout << "test_clear" << std::endl;
  {
    json_array arr{ json_file{ "src/tests/json/array.json" } };
    arr.clear();
    assert(arr.size() == 0);
    assert(arr.find(1) == arr.end());
    arr.reset("[\"foo\",\"bar\"]");
    assert(arr.get<std::string>(1) == "bar");
    arr.reset("[\"foo\",\"spam\"]");
    assert(arr.get(1) == "spam");

    assert(arr.find("foo") != arr.end());
    arr.erase(0);
    assert(arr.find("foo") == arr.end());

    arr.push_back(42.0f);
    auto it(arr.find(42.0f));
    assert(it != arr.end());
    arr.erase(it);
    assert(arr.find(42.0f) == arr.end());
    assert(arr.size() == 1);

    arr.push_back("bar");
    assert(arr.size() == 2);
    arr.erase(arr.begin(), arr.begin() + 1);
    assert(arr.size() == 1);
    assert(arr.get<std::string>(0) == "bar");
  }
  std::cout << "end test_clear" << std::endl;
}

int main()
{
  try
  {
    json_array arr{ json_file{ "src/tests/json/array.json" } };

    assert(json_array::delim_open == '[');
    assert(json_array::delim_close == ']');
    assert(arr.delim_open == '[');
    assert(arr.delim_close == ']');

    test_ctor();
    test_get();
    test_size();
    test_find();
    test_set();
    test_clear();
  }
  catch(std::exception const &e)
  { std::cerr << "exception: " << e.what() << std::endl; }
}


/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: tests/map.cpp
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
    json_map map;
    assert(map.size() == 0);
    assert(map.to_string() == "{}");

    json_map const cmap;
    assert(cmap.size() == 0);
    assert(cmap.to_string() == "{}");
  }
  { /* json ctor */
    json_map map{ "{ \"foo\": \"bar\" }" };
    assert(map.size() == 1);
    assert(map.to_string() ==  "{\"foo\":\"bar\"}" );
  }
  { /* map ctor */
    std::map<std::string, double> sm{ { "0", 0.0 }, { "1", 1.0 } };
    json_map map{ sm };
    assert(map.size() == 2);
    assert(map.get<double>("0") == 0.0);
    assert(map.get<double>("1") == 1.0);
  }
  std::cout << "end test_ctor" << std::endl;
}

void test_get()
{
  std::cout << "test_get" << std::endl;
  { /* References */
    std::cout << "test_get ref" << std::endl;
    std::map<std::string, double> sm{ { "0", 0.0 }, { "1", 1.0 } };
    json_map map{ sm };
    double &z(map.get<double>("0"));
    double &o(map.get<double>("1"));
    z = 42.0;
    o = -42.0;
    assert(map.size() == 2);
    assert(map.get<double>("0") == 42.0);
    assert(map.get<double>("1") == -42.0);
  }
  { /* Fallbacks */
    std::cout << "test_get fall" << std::endl;
    std::map<std::string, double> sm{ { "0", 0.0 }, { "1", 1.0 } };
    json_map map{ sm };
    double z(map.get("0", 77.0));
    double o(map.get("1", 77.0));
    double t(map.get("2", 77.0));
    assert(z == 0.0);
    assert(o == 1.0);
    assert(t == 77.0);

    z = 42.0;
    o = -42.0;
    assert(map.size() == 2);
    assert(map.get<double>("0") == 0.0);
    assert(map.get<double>("1") == 1.0);

    std::map<std::string, std::string> sm2{ { "foo", "test" } };
    json_map smap{ sm2 };
    assert(smap.size() == 1);
    assert(smap.get("foo", "fallback") == "test");
    assert(smap.get("bar", "fallback") == "fallback");
  }
  { /* Named */
    std::cout << "test_get named" << std::endl;
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":null},\"arr\":[0,1,2]}" };

    auto &foo(map.get<json_map>("foo"));
    assert(foo.get<int>("bar") == 42);

    auto &arr(map.get<json_array>("arr"));
    assert(arr.size() == 3);
    for(int i{}; i < 3; ++i)
    { assert(arr[i].as<int8_t>() == i); }
  }
  { /* Paths */
    std::cout << "test_get path" << std::endl;
    json_map map{ json_file{ "src/tests/json/map.json" } };

    assert(map.get_for_path<double>("doesnotexist", 77.0) == 77.0);
    assert(map.get_for_path<std::string>("does.not.exist", "nope") == "nope");

    assert(map.get_for_path<json_int>("person.inventory.coins") == 1136);
    assert(map.get_for_path("person.inventory.skooma") == 7);
    assert(map.get_for_path<json_int>("person.inventory.zzz", 42) == 42);
    assert(map.get_for_path("person.name") == "Roger");
    assert(map.get_for_path<std::string>("person.notname", "zzz") == "zzz");
    assert(map.get_for_path<json_array>("arr")[8].as<double>() == 9.9);
  }
  { /* Null */
    std::cout << "test_get null" << std::endl;
    auto const map(json_map{ json_file{ "src/tests/json/map.json" } });
    assert(map.get("null") == json_null{});
    assert(map.get("str") != json_null{});
  }
  std::cout << "end test_get" << std::endl;
}

void test_size()
{
  std::cout << "test_size" << std::endl;
  {
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":43},\"baz\":null}" };
    assert(map.size() == 2);
    assert(!map.empty());

    auto foo(map.get<json_map>("foo"));
    assert(foo.size() == 2);
    assert(!foo.empty());

    assert(json_map{}.size() == 0);
    assert(json_map{}.empty());
  }
  std::cout << "end test_size" << std::endl;
}

void test_find()
{
  std::cout << "test_find" << std::endl;
  {
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":43},\"baz\":null}" };
    auto it(map.find("foo"));
    assert(it != map.end());
    assert(it->second.as<json_map>().get<uint32_t>("bar") == 42);

    it = map.find("doesnotexist");
    assert(it == map.end());

    it = map.find(json_value{"baz"});
    assert(it != map.end());
    assert(it->first == "baz");
  }
  std::cout << "end test_find" << std::endl;
}

void test_set()
{
  std::cout << "test_set" << std::endl;
  {
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":43},\"baz\":null}" };

    map.set("baz", 77);
    assert(map.get<json_int>("baz") == 77);
    map.set("burp", "soda");
    assert(map.get<std::string>("burp") == "soda");

    map.set<json_int>("map", { { "one", 1 } });
    assert(map.get<json_map>("map").get<json_int>("one") == 1);
  }
  std::cout << "end test_set" << std::endl;
}

void test_clear()
{
  std::cout << "test_clear" << std::endl;
  {
    json_map map{ "{\"foo\":{\"bar\":42,\"spam\":43},\"baz\":null}" };
    map.clear();
    assert(map.size() == 0);
    assert(map.find("foo") == map.end());
    map.reset("{\"foo\": \"bar\"}");
    assert(map.get<std::string>("foo") == "bar");
    map.reset("{\"foo\":{\"bar\":42,\"spam\":43},\"baz\":null}");
    assert(map.get_for_path<json_int>("foo.spam") == 43);

    assert(map.find("baz") != map.end());
    map.erase("baz");
    assert(map.find("baz") == map.end());
    assert(map.get_for_path<json_int>("foo.spam") == 43);
  }
  std::cout << "end test_clear" << std::endl;
}

int main()
{
  try
  {
    json_map map{ json_file{ "src/tests/json/map.json" } };

    assert(json_map::delim_open == '{');
    assert(json_map::delim_close == '}');
    assert(map.delim_open == '{');
    assert(map.delim_close == '}');

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

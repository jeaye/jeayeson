/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: main.cpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <iostream>
#include <stdint.h>
#include <string>

#include "JsonValue.h"

int32_t main(int32_t argc, char **argv)
{
  std::cout << "Starting\n";

  JsonMap jm;
  jm.loadFile("NOTES");

  JsonArray ja;
  ja.loadFile("NOTES");
  ja.add("foo");
  ja.add("bar");
  ja.add("spam");
  ja.add("jove");
  ja.add("tots");

  JsonMap inner;
  inner.set("in foo", "in bar");
  inner.set("in bar", true);
  inner.set("names", ja);
  jm.set("in", inner);

  std::cout << jm.jsonString();

  std::cout << "\n\n\nDone\n";
  return 0;
}


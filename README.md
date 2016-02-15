JeayeSON - a very sane C++14 JSON library [![Build Status](https://travis-ci.org/jeaye/jeayeson.svg?branch=master)](https://travis-ci.org/jeaye/jeayeson)
---

JeayeSON was designed out of frustration that there aren't many
template-based approaches to handling JSON in modern C++. Given a
very limited number of types (that JSON offers), functions
can be written in a generic manner to provide the most consistent
interface, no matter the type. JeayeSON is a non-validating JSON library;
it expects valid JSON all of the time.

  * Header only (easy to add to any project)
  * Small, consistent C++ API
  * Typesafe, C++14 interface
  * Absolutely no macros needed nor used
  * UTF-8 (with UTF-16 transcoding)
  * Extensive test suite (using [jest](https://github.com/jeaye/jest), a "sane and minimal C++14 unit test framework")

Practice
----
Assuming the JeayeSON headers are in your search path, simply
```cpp
#include <jeayeson/jeayeson.hpp>
```
**NOTE:** If you're on Windows and symbolic links don't work, use
```cpp
#include <jeayeson/value.hpp>
```
This will give you access to the following types:
```cpp
json_value    /* variant type */
json_map      /* string->json_value map */
json_array    /* contiguous array of json_values */
json_null     /* json_value's default state */
json_int      /* defaults to int64_t */
json_float    /* defaults to double */
json_file     /* aggregate type representing a filename */
json_data     /* aggregate type representing json data as a string */
```

Building JSON
----
Assume we want to create this JSON:
```json
{
  "hello": "world",
  "arr":
  [ 1.1, 2.2, 3.3 ],
  "person":
  {
    "name": "Tom",
    "age": 36,
    "weapon": null
  }
}
```
We can simply do:
```cpp
json_value val // json_value can be any json type; here, it's a json_map
{
  { "hello", "world" }, // nested pairs represent key/value
  {
    "arr",
    { 1.1, 2.2, 3.3 } // arrays are easy
  },
  {
    "person",
    {
      { "name", "Tom" },
      { "age", 36 },
      { "weapon", nullptr } // can also use json_null
    }
  }
};
```
Or, if we wanted to build it piece by piece:
```cpp
json_map val; // explicitly make a map this time

val["hello"] = "world"; // simple assignments work on all compatible types

val["arr"] = { 1.1, 2.2, 3.3 }; // implicit array construction

json_map person; // we'll build the person separately, too
person["name"] = "Tom";
person["age"] = 36;
person["weapon"] = nullptr;

val["person"] = person; // now we can just put the person map into val

// we can even dig into nested maps and arrays using op[]
val["person"]["name"] = "Susan";
```

### Reading a JSON string
```cpp
std::string json; // Acquired/initialized elsewhere
json_array arr{ json_data{ json } }; // simple aggregate for type-safety
```
### Reading a file
```cpp
json_map map{ json_file{ "my_file.json" } }; // simple aggregate for type-safety
```
### Writing JSON
```cpp
// maps, arrays, and values can all be used with streams
json_map map{ json_file{ "my_file.json" } };
std::cout << map << std::endl;

// you can also just write them to a string
std::string const json{ map.to_string() };
```

Feels like the C++ stdlib
----
You'll find all the normal stdlib-like functions, including iterator support.
```cpp
json_array json;
json.push_back("string");
json.push_back(3.14159);
json.push_back(false);

for(auto const &j : json) // has begin, end, cbegin, cend, etc
{ std::cout << j << std::endl; }

// works like an std::vector or std::map
json.size();
json.empty();
json.clear();
json.reserve(42);
```

Type checking and casting
----
```cpp
json_map json
{
  { "boolean", false },
  { "str", "..." }
};

// check types with is<T>()
json["boolean"].is<json_value::type::boolean>(); // true
json["str"].is<json_value::type::string>(); // true

// query the type enum
auto const type(json["str"].get_type());

// cast with as<T>() to get the complete type
auto const str(json["str"].as<json_string>());
```

### Installation
The `./configure` script must be used at least once to automagically generate `jeayeson/config.hpp` (see [Customization](https://github.com/jeaye/jeayeson#customization)). Since JeayeSON is a header-only library, simply copy over the contents of `include` to your project, or, better yet, add JeayeSON as a submodule and introduce `jeayeson/include` to your header search paths

A full installation can also be achieved by using `./configure && make install`. See the `./configure` script for prefix options.

Customization
---
**NOTE**: All configuration is easily done in `jeayeson/config.hpp`, which is generated when you run `./configure` (and is not overwritten subsequently -- delete it to reset).

Customization can be achieved by adjusting the types in the `jeayeson::config` struct template. A specialization is already provided, which contains the default types used by JeayeSON. Feel free to change the types to any other, still compatible, types.

For example, you may want the json integer type to be 32bit instead of the default 64bit. Or, you may want to use `std::unordered_map` instead of `std::map`.

### Building tests
**NOTE:** You don't actually have to build JeayeSON, since it's a header-only
library. This build process is only for the tests.

In the project directory, run:
```bash
$ ./configure && make

# Depending on your compiler setup (gcc or clang, linux or osx, etc)
# you may need to specify some extra flags. An example case:
# (allows clang & libc++ to work on Arch Linux)
$ CXX=clang++ CXX_FLAGS=-stdlib=libc++ LD_LIBS=-lc++abi make
```
Once built, you can run the tests:
```bash
$ make test
```
The tests (in `test/src` and `test/include`) can give more examples
on how to use JeayeSON.

JeayeSON - a very sane C++14 JSON library
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

Reading JSON
----
### Reading a JSON string
```cpp
std::string json; // Acquired/initialized elsewhere
json_array arr{ json };
```
### Reading a file
```cpp
json_map map{ json_file{ "my_file.json" } };
```
**NOTE:** Loading a file into a `json_map` must mean that the JSON begins with a map `{ }`. Furthermore, any JSON that begins with an array `[ ]` must be loaded into a `json_array`.

Writing JSON
----
```cpp
json_map map{ json_file{ "my_file.json" } };
std::cout << map << std::endl;
```
Interacting with JSON
----
Assume the JSON we're working with is as follows:
```json
{
  "str": "This is str",
  "arr":
  [ 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9 ],
  "person":
  {
    "name": "Tom",
    "age": 36,
    "weapon": null,
    "inventory":
    {
      "coins": 1136,
      "skooma": 7
    }
  }
}
```
And the following code to interact with the JSON:
```cpp
json_map map{ json_file{ "test/json/main.json" } };

std::string &str{ map.get<std::string>("str") };
std::cout << "str = " << str << std::endl;
auto &arr(map.get<json_array>("arr"));

/* A fallback value can also be specified with "get". It does two things:
     1. Helps deduce the type so that an explicit invocation is not needed
     2. Provides a default fallback value, should anything go wrong while accessing
   Note that these functions do NOT return references, due to incompatibilities
   with the fallback (could be an rvalue). */
std::string const str_copy{ map.get("str", "meow") }; // Second param is the default

/* Delving into maps using dot-notated paths works, too.
   The type can be explicitly specified, or implicit based on the provided fallback.
   They default to json_value, which offers op==, op<<, et cetera. */
std::cout << map.get_for_path("person.name") << " has " // Returns json_value&
          << map.get_for_path("person.inventory.coins", 0) << " coins\n";

/* A less verbose way is to just use op[] on the json_values; this is more convenient,
 * but it comes at the cost of less type-safety and more runtime checks. */
std::cout << map["person"]["inventory"]["coins"] << std::endl;
std::cout << map["arr"][1] << std::endl;

for(auto const &it : arr)
{ std::cout << it.as<json_float>() << " "; }
std::cout << std::endl;
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

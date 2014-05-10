JeayeSON
---

A very sane (header only) C++ JSON library. JeayeSON was designed out of 
frustration that there aren't many templated approaches to handling JSON
in C++. Given a very limited number of types (that JSON offers), functions
can be written in a generic manner to provide the most consistent
interface, no matter the type. JeayeSON is a non-validating JSON library;
it expects valid JSON all of the time.

  * Header only (easy to add to any project)
  * Small, consistent C++ API
  * Typesafe, C++1y interface

Practice
----
Assuming the JeayeSON headers are in your search path, simply  
```cpp
#include <jeayeson.hpp>
```
This will give you access to `json_map`, `json_array`, and `json_value` which are the three types you'll be interfacing with directly (though, ocassionally, `json_null` may be used).

Reading JSON
----
### Reading a JSON string
```cpp
std::string json; // Acquired/initialized elsewhere
json_array arr{ json };
```
### Reading a file
```cpp
json_map map;
map.load_file("my_file.json");
```
**NOTE:** Loading a file into a `json_map` must mean that the JSON begins with a map `{ }`. Furthermore, any JSON that begins with an array `[ ]` must be loaded into a `json_array`.

Writing JSON
----
```cpp
json_map map;
map.load_file("my_file.json");
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
    "name": "Jeaye",
    "age": 22,
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
  /* To start with, create a map and load a file. */
  json_map map;
  map.load_file("doc/getting_started/example.json");

  /* We can look at some specify top-level values with "get".
     Notice that "get" returns a reference to the object. */
  std::string &str(map.get<std::string>("str")); /* Get "str" as a mutable string reference. */
  std::cout << "str = " << str << std::endl;
  json_array &arr(map.get_array("arr")); /* get_array and get_map are convenience functions. */

  /* A fallback value can also be specified with "get". It does two things:
       1. Helps deduce the type so that an explicit invocation is not needed
       2. Provides a default fallback value, should anything go wrong while accessing
     Note that these functions do NOT return references, due to incompatibilities with the fallback. */
  std::string const str_copy{ map.get("str", "Default awesomeness") }; // Second param is the default

  /* Delving into maps using dot-notated paths works, too.
     The type can be explicitly specified, or implicit based on the provided fallback.
     They default to json_value, which offers op==, op<<, et cetera. */
  std::cout << map.get_for_path("person.name") << " has " // No fallback, returns json_value&
            << map.get_for_path("person.inventory.coins", 0) << " coins\n"; // Fallback is 0

  /* Iterators work as expected, based on the C++ stdlib. (const and non-const) */
  for(auto const &it : arr)
  { std::cout << it.as<double>() << " "; }
  std::cout << std::endl;
```

Building
---
**NOTE:** You don't actually have to build JeayeSON, since it's a header-only
library. This build process is simply for the tests.

In the project directory, run:
```bash
$ make
```
Once built, you can run the tests:
```bash
$ make test
```
The tests (in src/tests/main.cpp and friends) can give more examples
on how to use JeayeSON.

Customization
---

**NOTE**: These are all easily changed in src/defines.hpp

`#define JEAYESON_USE_STD_MAP`  
  *Use std::map (ideal in most cases)*  
`#define JEAYESON_USE_STD_UNORD`  
  *Use std::unordered_map*  
`#define JEAYESON_USE_BOOST_UNORD`  
  *Use boost::unordered_map*  
`#define JEAYESON_USE_OTHER_MAP`  
  *Specify a custom map to use with* `JEAYESON_OTHER_MAP`  
`#define JEAYESON_OTHER_MAP MyMapType`  
  *Use the specified map type -- must be used in conjunction with* `JEAYESON_USE_OTHER_MAP`  

`#define JEAYESON_STD_FSTREAM_LOAD {0-1} (default 1)`  
  *Use std::ifstream instead of boost's memory mapping for normal (not streaming) file reading.*  

Help/Contact
---
If you have questions, first check the JeayeSON wiki: https://github.com/jeaye/jeayeson/wiki

For any other bug reports, questions, comments, or suggestions, please [make an issue](https://github.com/jeaye/jeayeson/issues).

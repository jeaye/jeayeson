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
#include <jeayeson/jeayeson.hpp>
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
  json_map map{ json_file{ "src/test/json/main.json" } };

  /* We can look at some specify top-level values with "get".
     Notice that "get" returns a reference to the object. */
  std::string &str(map.get<std::string>("str")); /* Get "str" as a mutable string reference. */
  std::cout << "str = " << str << std::endl;
  auto &arr(map.get<json_array>("arr"));

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

  /* A less verbose way is to just use op[] on the json_values; this is more convenient,
   * but it comes at the cost of less type-safety and more runtime checks. */
  std::cout << map["person"]["inventory"]["coins"] << std::endl;
  std::cout << map["arr"][1] << std::endl;

  /* Iterators work as expected, based on the C++ stdlib. (const and non-const) */
  for(auto const &it : arr)
  { std::cout << it.as<json_float>() << " "; }
  std::cout << std::endl;
```

Building
---
**NOTE:** You don't actually have to build JeayeSON, since it's a header-only
library. This build process is only for the tests.

In the project directory, run:
```bash
$ ./configure
$ make
```
Once built, you can run the tests:
```bash
$ make test
```
The tests (in src/test/main.cpp and friends) can give more examples
on how to use JeayeSON.

Customization
---

**NOTE**: These are all easily changed in `jeayeson/defines.hpp`

`#define JEAYESON_USE_STD_MAP`  
  *Use std::map (ideal in most cases)*  
`#define JEAYESON_USE_STD_UNORD`  
  *Use std::unordered_map*  
`#define JEAYESON_USE_BOOST_UNORD`  
  *Use boost::unordered_map*  
`#define JEAYESON_USE_OTHER_MAP`  
  *Specify a custom map to use with* `JEAYESON_OTHER_MAP`  
`#define JEAYESON_OTHER_MAP my_map_type`  
  *Use the specified map type -- must be used in conjunction with* `JEAYESON_USE_OTHER_MAP`  

`#define JEAYESON_FLOAT my_float_type`  
  *Use the specified float type -- must be included before jeayeson if non-intrinsic*  
`#define JEAYESON_INT my_int_type`  
  *Use the specified int type -- must be included before jeayeson if non-intrinsic*

Help/Contact
---
For any other bug reports, questions, comments, or suggestions, please [make an issue](https://github.com/jeaye/jeayeson/issues).

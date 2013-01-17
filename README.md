JeayeSON
---

A very sane (header only) C++ JSON library. JeayeSON was designed out of 
frustration that there aren't many templated approaches to handling JSON
in C++. Given a very limited number of types (that JSON offers), functions
can be written in a generic manner to provide the most consistent
interface, no matter the type. JeayeSON is a non-validating JSON library;
it expects valid JSON all of the time.

JeayeSON is designed to handle like any C++ stdlib container would, so
experienced C++ programmers should feel right at home.

Building
---
**NOTE:** You don't actually have to build JeayeSON, since it's a header-only
library. This build process is simply for the tests.

In the project directory, run:

`$ ./build.sh`
  
Once built, you can run the tests by executing the produced binary:

`$ ./bin/jeayeson`
  
The tests should show (in src/main.cpp) how to use JeayeSON. Enjoy!

Customization
---

NOTE: These are all easily changed in src/defines.h

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
If you have questions, first check the JeayeSON wiki: https://github.com/Jeaye/JeayeSON/wiki

For any other bug reports, questions, comments, or suggestions, contact me directly, or at Github:

Email: jeaye (at) arrownext (dot) com  
Github: https://github.com/Jeaye/JeayeSON  
IRC: irc.freenode.net #arrownext  

Thanks
---
A special thanks to Cogwheel for his contributions and suggestions


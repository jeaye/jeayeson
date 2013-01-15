/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: Defines.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef DEFINES_H
#define DEFINES_H

/*
  Defines the internal map type used by JsonMap. std::map is sufficient for most uses, and optimal
  for many. You can specify other map types if a need arises.
*/
#define JEAYESON_USE_STD_MAP           // uses std::map
// #define JEAYESON_USE_STD_UNORD      // uses std::unordered_map
// #define JEAYESON_USE_BOOST_UNORD    // uses boost::unordered_map
// #define JEAYESON_USE_OTHER_MAP      // specify an alternate type (see below)

/*
  If JEAYESON_USE_OTHER_MAP is defined, specify the type here. You must ensure that any necessary
  headers are included before instantiating a JsonMap. The custom map type must be compatible with std::map.
*/
// #define JEAYESON_OTHER_MAP YourMapTypeHere

/* Use std::ifstream instead of boost's memory mapping for normal (not streaming) file reading. */
#define JEAYESON_STD_FSTREAM_LOAD 1

#endif /* DEFINES_H */


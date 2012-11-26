/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: Defines.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef DEFINES_H
#define DEFINES_H

/* Use std::map instead of boost::unordered_map (decrease in performance on large data sets). */
#define JEAYESON_STD_MAP 0

/* Use std::ifstream instead of boost's memory mapping for normal (not streaming) file reading. */
#define JEAYESON_STD_FSTREAM_LOAD 1

#endif /* DEFINES_H */


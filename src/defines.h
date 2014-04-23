/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: defines.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

/*
  Defines the internal map type used by JsonMap. std::map is sufficient for most uses, and optimal
  for many. You can specify other map types if a need arises.
*/
#define JEAYESON_USE_STD_MAP           /* uses std::map */
// #define JEAYESON_USE_STD_UNORD      /* uses std::unordered_map */
// #define JEAYESON_USE_BOOST_UNORD    /* uses boost::unordered_map */
// #define JEAYESON_USE_OTHER_MAP      /* specify an alternate type (see below) */

/*
  If JEAYESON_USE_OTHER_MAP is defined, specify the type here.
  You must ensure that any necessary headers are included before
  instantiating a json_map. The custom map type must have a
  compatible interface with std::map. op[], begin, end, et cetera
*/
// #define JEAYESON_OTHER_MAP your_map_type_here

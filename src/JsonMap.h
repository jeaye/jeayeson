/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: JsonMap.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef JEAYESON_JSONMAP_H
#define JEAYESON_JSONMAP_H 

#include <map>

#include "JsonValue.h"

namespace JeayeSON
{
  /* JsonMaps provide a wrapper for
   * string-indexed JsonValues, which
   * could be anything (including more
   * JsonMaps!).
   */
  class Map
  {
    public:
//#pragma mark - ctors and dtor
      Map()
      { }
      virtual ~Map()
      { }

//#pragma mark - accessors
      template <typename T>
      inline T get(char const * const key, T const &fallback)
      { return (hasKey(key) ? m_values[key].as<T>() : fallback); }
      inline std::string get(char const * const key, char const * const fallback)
      { return (hasKey(key) ? m_values[key].as<std::string>() : fallback); }

      inline bool hasKey(char const * const key) const
      { return (m_values.find(key) != m_values.end()); }
      inline bool hasKey(std::string const &key) const
      { return (m_values.find(key) != m_values.end()); }

      inline bool isEmpty() const
      { return m_values.empty(); }

//#pragma mark - mutators
      template <typename T>
      inline void set(char const * const key, T value)
      { m_values[key] = value; }
      template <typename T>
      inline void set(std::string const &key, T value)
      { m_values[key] = value; }

      /* Completely wipes out all data in the map. */
      inline void clear()
      { m_values.clear(); }

      /* Completely removes the specified key and destroys its data. */
      inline void erase(char const * const key)
      { m_values.erase(key); }
      inline void erase(std::string const &key)
      { m_values.erase(key); }

      /* Adds the specified map into this map. */
      inline void merge(Map const &map)
      { m_values.insert(map.m_values.begin(), map.m_values.end()); }

    private:
//#pragma mark - members
      std::map<std::string, Value> m_values;

  }; /* Class Map */
} /* Namespace JeayeSON */

typedef JeayeSON::Map JsonMap;

#endif /* JEAYESON_JSONMAP_H */


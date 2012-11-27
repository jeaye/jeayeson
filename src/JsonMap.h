/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: JsonMap.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef JEAYESON_JSONMAP_H
#define JEAYESON_JSONMAP_H

#if JEAYESON_STDMAP
  #include <map>
#else
  #include <boost/unordered_map.hpp>
#endif
#include <string>

#include "JsonParser.h"

namespace JeayeSON
{
  /* JsonMaps provide a wrapper for
   * string-indexed JsonValues, which
   * could be anything (including more
   * JsonMaps!).
   */
  template <typename Value>
  class Map : public IParseable
  {
    public:
      typedef char const * const cstr_t;
      static char const delimOpen = '{';
      static char const delimClose = '}';

//#pragma mark - ctors and dtor
      Map()
      { }
      explicit Map(std::string const &json)
      { if(load(json) == false) throw "Failed to load json!"; }
      ~Map()
      { }

//#pragma mark - accessors
      template <typename T>
      inline T get(std::string const &key, T const &fallback)
      { return (hasKey(key) ? m_values[key].template as<T>() : fallback); }
      inline std::string get(std::string const &key, cstr_t fallback)
      { return (hasKey(key) ? m_values[key].template as<std::string>() : fallback);  }

      inline bool hasKey(std::string const &key) const
      { return (m_values.find(key) != m_values.end()); }

      inline bool isEmpty() const
      { return m_values.empty(); }

//#pragma mark - mutators
      template <typename T>
      inline void set(cstr_t key, T value)
      { m_values[key] = value; }
      template <typename T>
      inline void set(std::string const &key, T value)
      { m_values[key] = value; }

      /* Completely wipes out all data in the map. */
      inline void clear()
      { m_values.clear(); }

      /* Completely removes the specified key and destroys its data. */
      inline void erase(std::string const &key)
      { m_values.erase(key); }

      /* Adds the specified map into this map. */
      inline void merge(Map const &map)
      { m_values.insert(map.m_values.begin(), map.m_values.end()); }

      /* Loads the specified JSON string. */
      bool load(std::string const &json)
      { return parse<Map<Value> >(json); }
      static Map<Value> loadNew(std::string const &json)
      { Map<Value> m; m.parse<Map<Value> >(json); return m; }

      /* Loads the specified JSON file. */
      bool loadFile(std::string const &jsonFile)
      { return parseFile<Map<Value> >(jsonFile); }
      static Map<Value> loadFileNew(std::string const &jsonFile)
      { Map<Value> m; m.parseFile<Map<Value> >(jsonFile); return m; }

    private:
//#pragma mark - members
#if JEAYESON_STD_MAP
      std::map<std::string, Value> m_values;
#else
      boost::unordered_map<std::string, Value> m_values;
#endif

  }; /* Class Map */
} /* Namespace JeayeSON */

#endif /* JEAYESON_JSONMAP_H */


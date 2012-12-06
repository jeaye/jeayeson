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
#if JEAYESON_STD_MAP
      typedef std::map<std::string, Value> map_t;
#else
      typedef boost::unordered_map<std::string, Value> map_t;
#endif
      static char const delimOpen = '{';
      static char const delimClose = '}';

      Map()
      { }
      explicit Map(std::string const &json)
      { if(load(json) == false) throw "Failed to load json!"; }
      ~Map()
      { }

      template <typename T>
      inline T get(std::string const &key, T const &fallback)
      { return (hasKey(key) ? m_values[key].template as<T>() : fallback); }
      inline std::string get(std::string const &key, cstr_t fallback)
      { return (hasKey(key) ? m_values[key].template as<std::string>() : fallback);  }

      inline bool hasKey(std::string const &key) const
      { return (m_values.find(key) != m_values.end()); }

      inline bool isEmpty() const
      { return m_values.empty(); }

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
      { return parse(json); }
      static Map<Value> loadNew(std::string const &json)
      { Map<Value> m; m.parse(json); return m; }

      /* Loads the specified JSON file. */
      bool loadFile(std::string const &jsonFile)
      { return parseFile<Map<Value> >(jsonFile); }
      static Map<Value> loadFileNew(std::string const &jsonFile)
      { Map<Value> m; m.parseFile<Map<Value> >(jsonFile); return m; }

      /* Writes the JSON data to string form. */
      inline std::string jsonString()
      { return save<Map<Value> >(*this); }

      template <typename T>
      friend std::ostream& operator <<(std::ostream &stream, Map<T> const &map);

    private:
      map_t m_values;

  }; /* Class Map */

  template <typename Value>
  std::ostream& operator <<(std::ostream &stream, Map<Value> const &map)
  {
    stream << map.delimOpen;
    for(typename Map<Value>::map_t::const_iterator i = map.m_values.begin(); i != map.m_values.end(); ++i, stream.put(','))
      stream << "\"" << i->first << "\":" << i->second;

    /* Replace the last comma with the object's close delim. */
    stream.seekp(-1, std::ios_base::end);
    stream.put(map.delimClose);

    return stream;
  }

} /* Namespace JeayeSON */

#endif /* JEAYESON_JSONMAP_H */


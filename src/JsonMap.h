/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: JsonMap.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef JEAYESON_JSONMAP_H
#define JEAYESON_JSONMAP_H

#include "Defines.h"
#include <string>

/* Decide underlying map type */
#undef JEAYESON_MAP_T

#ifdef JEAYESON_USE_STD_MAP
  #include <map>
  #define JEAYESON_MAP_T std::map

#elif defined JEAYESON_USE_STD_UNORD
  #include <map>
  #define JEAYESON_MAP_T std::unordered_map

#elif defined JEAYESON_USE_BOOST_UNORD
  #include <boost/unordered_map.hpp>
  #define JEAYESON_MAP_T boost::unordered_map

#elif defined JEAYESON_USE_OTHER_MAP
  /* Client is responsible for appropriate includes */
  #ifndef JEAYESON_OTHER_MAP
    #error JEAYESON_USE_OTHER_MAP specified but JEAYSON_OTHER_MAP is undefined.
  #else
    #define JEAYESON_MAP_T JEAYESON_OTHER_MAP
  #endif // defined(JEAYESON_OTHER_MAP)

#else
  #error No JEAYESON_USE_[map type] is defined. See Defines.h.

#endif


namespace JeayeSON
{
  template <typename Value, typename Parser>
  class Array;

  /* JsonMaps provide a wrapper for
   * string-indexed JsonValues, which
   * could be anything (including more
   * JsonMaps!).
   */
  template <typename Value, typename Parser>
  class Map
  {
    public:
      typedef Map<Value, Parser> this_t;
      typedef Value value_t;
      typedef Parser parser_t;
      typedef char const * const cstr_t;
      typedef JEAYESON_MAP_T<std::string, Value> map_t;

      static char const delimOpen = '{';
      static char const delimClose = '}';

      Map()
      { }
      explicit Map(std::string const &json)
      { load(json); }

      template <typename T>
      inline T get(std::string const &key, T const &fallback)
      { return (hasKey(key) ? m_values[key].template as<T>() : fallback); }
      inline std::string& get(std::string const &key, cstr_t fallback)
      { return (hasKey(key) ? m_values[key].template as<std::string>() : fallback);  }
      inline this_t& getMap(std::string const &key)
      { return m_values[key].template as<this_t >(); }
      inline Array<value_t, parser_t>& getArray(std::string const &key)
      { return m_values[key].template as<Array<value_t, parser_t> >(); }

      inline bool hasKey(std::string const &key) const
      { return (m_values.find(key) != m_values.end()); }

      inline bool isEmpty() const
      { return m_values.empty(); }
      inline size_t getSize() const
      { return m_values.size(); }

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
      void load(std::string const &json)
      { *this = Parser::template parse< this_t >(json); }
      static this_t loadNew(std::string const &json)
      { return Parser::template parse< this_t >(json); }

      /* Loads the specified JSON file. */
      void loadFile(std::string const &jsonFile)
      { *this = Parser::template parseFile< this_t >(jsonFile); }
      static this_t loadFileNew(std::string const &jsonFile)
      { return Parser::template parseFile< this_t >(jsonFile); }

      /* Writes the JSON data to string form. */
      inline std::string jsonString()
      { return Parser::template save< this_t >(*this); }

      template <typename ValueT, typename ParserT>
      friend std::ostream& operator <<(std::ostream &stream, Map<ValueT, ParserT> const &map);

    private:
      map_t m_values;

  }; /* Class Map */
} /* Namespace JeayeSON */

#endif /* JEAYESON_JSONMAP_H */


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


namespace jeayeson
{
  template <typename Value, typename Parser>
  class array;

  /* json_maps provide a wrapper for
   * string-indexed JsonValues, which
   * could be any valid JSON value.
   */
  template <typename Value, typename Parser>
  class map
  {
    public:
      typedef map<Value, Parser> this_t;
      typedef Value value_t;
      typedef Parser parser_t;
      typedef char const * const cstr_t;
      typedef JEAYESON_MAP_T<std::string, Value> map_t;
      typedef typename map_t::iterator iterator;
      typedef typename map_t::const_iterator const_iterator;

      static char const delim_open = '{';
      static char const delim_close = '}';

      map()
      { }
      explicit map(std::string const &json)
      { load(json); }

      template <typename T>
      inline T& get(std::string const &_key) // No fallback, but you get a reference
      { return m_values[_key].template as<T>(); }
      template <typename T>
      inline T get(std::string const &_key, T const &_fallback)
      { return (has_key(_key) ? m_values[_key].template as<T>() : _fallback); }
      inline std::string get(std::string const &_key, cstr_t _fallback)
      { return (has_key(_key) ? m_values[_key].template as<std::string>() : _fallback);  }
      inline this_t& get_map(std::string const &_key)
      { return m_values[_key].template as<this_t >(); }
      inline array<value_t, parser_t>& get_array(std::string const &_key)
      { return m_values[_key].template as<array<value_t, parser_t> >(); }

      inline iterator begin()
      { return m_values.begin(); }
      inline const_iterator cbegin() const
      { return m_values.cbegin(); }

      inline iterator end()
      { return m_values.end(); }
      inline const_iterator cend() const
      { return m_values.cend(); }

      inline bool has_key(std::string const &_key) const
      { return (m_values.find(_key) != m_values.end()); }

      inline bool empty() const
      { return m_values.empty(); }
      inline size_t size() const
      { return m_values.size(); }

      template <typename T>
      inline void set(cstr_t _key, T _value)
      { m_values[_key] = _value; }
      template <typename T>
      inline void set(std::string const &_key, T _value)
      { m_values[_key] = _value; }

      /* Completely wipes out all data in the map. */
      inline void clear()
      { m_values.clear(); }
      inline void reset(std::string const &_json)
      { *this = load(_json); }

      /* Completely removes the specified key and destroys its data. */
      inline void erase(std::string const &_key)
      { m_values.erase(_key); }

      /* Adds the specified map into this map. */
      inline void merge(map const &_map)
      { m_values.insert(_map.m_values.begin(), _map.m_values.end()); }

      /* Loads the specified JSON string. */
      void load(std::string const &_json)
      { *this = Parser::template parse< this_t >(_json); }
      static this_t load_new(std::string const &_json)
      { return Parser::template parse< this_t >(_json); }

      /* Loads the specified JSON file. */
      void load_file(std::string const &_json_file)
      { *this = Parser::template parse_file< this_t >(_json_file); }
      static this_t load_file_new(std::string const &_json_file)
      { return Parser::template parse_file< this_t >(_json_file); }

      /* Writes the JSON data to string form. */
      inline std::string to_string()
      { return Parser::template save< this_t >(*this); }

      template <typename ValueT, typename ParserT>
      friend std::ostream& operator <<(std::ostream &stream, map<ValueT, ParserT> const &_map);

    private:
      map_t m_values;

  }; /* Class map */
} /* Namespace jeayeson */

#endif /* JEAYESON_JSONMAP_H */


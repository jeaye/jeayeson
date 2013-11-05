/*
  Copyright © 2013 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: map.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef JEAYESON_JSONMAP_H
#define JEAYESON_JSONMAP_H

#include "defines.h"
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

/* Decide underlying map type */
#undef JEAYESON_MAP_T

#ifdef JEAYESON_USE_STD_MAP
  #include <map>
  #define JEAYESON_MAP_T std::map

#elif defined JEAYESON_USE_STD_UNORD
  #include <unordered_map>
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
  std::vector<std::string> tokenize(std::string const &_source, std::string const &_delim);

  /* Maps provide a wrapper for
   * string-indexed values, which
   * could be any valid JSON object.
   */
  template <typename Value, typename Parser>
  class map
  {
    public:
#pragma mark - Types
      typedef map<Value, Parser> map_t;
      typedef array<Value, Parser> array_t;
      typedef std::string key_t;
      typedef Value value_t;
      typedef Parser parser_t;
      typedef char const * const cstr_t;
      typedef JEAYESON_MAP_T<key_t, Value> internal_map_t;
      typedef typename internal_map_t::iterator iterator;
      typedef typename internal_map_t::const_iterator const_iterator;

      static char const delim_open = '{';
      static char const delim_close = '}';

#pragma mark - Construction
      inline map()
      { }
      inline map(map const &_map) : m_values(_map.m_values)
      { }
      inline map(std::string const &_json)
      { load(_json); }
      inline map(value_t const &_val)
      {
        if(_val.get_type() == value_t::type_map)
          *this = _val.template as<map_t>();
        else
          throw std::runtime_error("Failed to construct map from non-map");
      }
      template <typename T>
      inline map(std::map<key_t, T> const &_container)
      {
        for(auto const &it : _container)
          set(it.first, it.second);
      }

#pragma mark - Accessors
      /* Access the internal variant type. */
      inline value_t& get(key_t const &_key)
      { return m_values[_key]; }
      inline value_t const& get(key_t const &_key) const
      { return m_values[_key]; }

      template <typename T>
      inline T& get(key_t const &_key)
      { return m_values[_key].template as<T>(); } 
      template <typename T>
      inline T const& get(key_t const &_key) const
      { return m_values[_key].template as<T>(); } 

      /* Access with a fallback. */
      template <typename T> /* TODO: Will be slow. */
      inline T get(key_t const &_key, T const &_fallback) const
      { return (has_key(_key) ? m_values[_key].template as<T>() : _fallback); }

      /* Fallback specializations. */
      inline std::string get(key_t const &_key, cstr_t _fallback) const
      { return (has_key(_key) ? m_values[_key].template as<std::string>() : _fallback);  }

      inline int64_t get(key_t const &_key, int8_t const _fallback) const
      { return (has_key(_key) ? m_values[_key].template as<int64_t>() : _fallback);  }
      inline int64_t get(key_t const &_key, uint8_t const _fallback) const
      { return (has_key(_key) ? m_values[_key].template as<int64_t>() : _fallback);  }
      inline int64_t get(key_t const &_key, int16_t const _fallback) const
      { return (has_key(_key) ? m_values[_key].template as<int64_t>() : _fallback);  }
      inline int64_t get(key_t const &_key, uint16_t const _fallback) const
      { return (has_key(_key) ? m_values[_key].template as<int64_t>() : _fallback);  }
      inline int64_t get(key_t const &_key, int32_t const _fallback) const
      { return (has_key(_key) ? m_values[_key].template as<int64_t>() : _fallback);  }
      inline int64_t get(key_t const &_key, uint32_t const _fallback) const
      { return (has_key(_key) ? m_values[_key].template as<int64_t>() : _fallback);  }
      inline int64_t get(key_t const &_key, long const _fallback) const
      { return (has_key(_key) ? m_values[_key].template as<int64_t>() : _fallback);  }
      inline int64_t get(key_t const &_key, unsigned long const _fallback) const
      { return (has_key(_key) ? m_values[_key].template as<int64_t>() : _fallback);  }

      inline double get(key_t const &_key, float const _fallback) const
      { return (has_key(_key) ? m_values[_key].template as<double>() : _fallback);  }

      /* Named specializations. */
      inline map_t& get_map(key_t const &_key)
      { return m_values[_key].template as<map_t >(); }
      inline map_t const& get_map(key_t const &_key) const
      { return m_values[_key].template as<map_t >(); }
      inline array_t& get_array(key_t const &_key)
      { return m_values[_key].template as<array_t>(); }
      inline array_t const& get_array(key_t const &_key) const
      { return m_values[_key].template as<array_t>(); }

      inline bool empty() const
      { return m_values.empty(); }
      inline size_t size() const
      { return m_values.size(); }

#pragma mark - Compatability Accessors
      template <typename T>
      inline T& get_for_path(std::string const &_path) const
      {
        std::vector<std::string> const tokens(tokenize(_path, "."));
        std::size_t const path_size(tokens.size() - 1);

        map_t *sub_map(const_cast<map_t*>(this));
        for(std::size_t i = 0; i < path_size; ++i)
          sub_map = &sub_map->get<map_t>(tokens[i]);

        return sub_map->get<T>(tokens[path_size]);
      }

      template <typename T>
      inline T get_for_path(std::string const &_path, T const &_fallback) const
      {
        std::vector<std::string> const tokens(tokenize(_path, "."));
        std::size_t const path_size(tokens.size() - 1);

        map_t *sub_map(const_cast<map_t*>(this));
        for(std::size_t i = 0; i < path_size; ++i)
        {
          typename map_t::iterator const it(sub_map->find(tokens[i]));
          if(it == sub_map->end())
            return _fallback;

          sub_map = &((*it).second.template as<map_t>());
        }

        typename map_t::iterator const it(sub_map->find(tokens[path_size]));
        if(it == sub_map->end())
          return _fallback;
        return (*it).second.template as<T>();
      }

      inline std::vector<key_t> get_keys() const
      {
        std::vector<key_t> keys;
        for(auto const &it : m_values)
          keys.push_back(it.first);
        return keys;
      }

      inline array_t get_values() const
      {
        array_t arr;
        for(auto const &it : m_values)
          arr.add(it.second);
        return arr;
      }

#pragma mark - Searching
      /* Searches for an entry with the specified key. */
      inline iterator find(key_t const &_key)
      { return m_values.find(_key); }
      inline const_iterator find(key_t const &_key) const
      { return m_values.find(_key); }

      inline bool has_key(key_t const &_key) const
      { return (m_values.find(_key) != m_values.end()); }

#pragma mark - Iterators
      inline iterator begin()
      { return m_values.begin(); }
      inline const_iterator begin() const
      { return m_values.begin(); }
      inline const_iterator cbegin() const
      { return m_values.begin(); }

      inline iterator end()
      { return m_values.end(); }
      inline const_iterator end() const
      { return m_values.end(); }
      inline const_iterator cend() const
      { return m_values.end(); }

#pragma mark - Mutators
      /* Generic set for valid JSON types. */
      template <typename T>
      inline void set(key_t const &_key, T const &_value)
      { m_values[_key] = _value; }

      /* Specialized mutators. */
      inline void set(key_t const &_key, int8_t const _value)
      { m_values[_key] = static_cast<int64_t>(_value); }
      inline void set(key_t const &_key, uint8_t const _value)
      { m_values[_key] = static_cast<int64_t>(_value); }
      inline void set(key_t const &_key, int16_t const _value)
      { m_values[_key] = static_cast<int64_t>(_value); }
      inline void set(key_t const &_key, uint16_t const _value)
      { m_values[_key] = static_cast<int64_t>(_value); }
      inline void set(key_t const &_key, int32_t const _value)
      { m_values[_key] = static_cast<int64_t>(_value); }
      inline void set(key_t const &_key, uint32_t const _value)
      { m_values[_key] = static_cast<int64_t>(_value); }
      inline void set(key_t const &_key, long const _value)
      { m_values[_key] = static_cast<int64_t>(_value); }
      inline void set(key_t const &_key, unsigned long const _value)
      { m_values[_key] = static_cast<int64_t>(_value); }
      inline void set(key_t const &_key, int64_t const _value)
      { m_values[_key] = _value; }
      inline void set(key_t const &_key, uint64_t const _value)
      { m_values[_key] = _value; }

      inline void set(key_t const &_key, bool const _value)
      { m_values[_key] = _value; }

      inline void set(key_t const &_key, float const _value)
      { m_values[_key] = static_cast<double>(_value); }
      inline void set(key_t const &_key, double const _value)
      { m_values[_key] = _value; }

      inline void set(key_t const &_key, cstr_t _value)
      { m_values[_key] = static_cast<std::string>(_value); }
      inline void set(key_t const &_key, std::string const&_value)
      { m_values[_key] = _value; }

      template <typename T>
      inline void set(key_t const &_key, std::map<key_t, T> const &_data)
      { set(_key, map(_data)); }

      /* Completely wipes out all data in the map. */
      inline void clear()
      { m_values.clear(); }
      inline void reset(std::string const &_json)
      { *this = load(_json); }

      /* Completely removes the specified key and destroys its data. */
      inline void erase(key_t const &_key)
      { m_values.erase(_key); }

      /* Adds the specified map into this map. */
      inline void merge(map const &_map)
      { m_values.insert(_map.m_values.begin(), _map.m_values.end()); }

#pragma mark - Loading
      /* Loads the specified JSON string. */
      inline bool load(std::string const &_json)
      { *this = Parser::template parse< map_t >(_json); return true; }
      static inline map_t load_new(std::string const &_json)
      { return Parser::template parse< map_t >(_json); }

      /* Loads the specified JSON file. */
      inline void load_file(std::string const &_json_file)
      { *this = Parser::template parse_file< map_t >(_json_file); }
      static inline map_t load_file_new(std::string const &_json_file)
      { return Parser::template parse_file< map_t >(_json_file); }

      /* Writes the JSON data to string form. */
      inline std::string to_string() const
      { return Parser::template save< map_t >(*this); }

      template <typename Stream_Value, typename Stream_Parser>
      friend std::ostream& operator <<(std::ostream &stream, map<Stream_Value, Stream_Parser> const &_map);

    private:
#pragma mark - Members
      mutable internal_map_t m_values;

  }; /* Class map */

  inline std::vector<std::string> tokenize(std::string const &_source, std::string const &_delim)
  {
    std::vector<std::string> tokens;
    boost::algorithm::split(tokens, _source, boost::is_any_of(_delim));
    return tokens;
  }
} /* Namespace jeayeson */

#endif /* JEAYESON_JSONMAP_H */


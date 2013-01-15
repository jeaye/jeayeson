/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: JsonArray.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef JEAYESON_JSONARRAY_H
#define JEAYESON_JSONARRAY_H

#include <vector>
#include <string>
#include <stdint.h>
#include <algorithm>

namespace jeayeson
{
  template <typename Value, typename Parser>
  class map;

  /* Arrays provide storage of
   * arbitrarily-typed JSON objects
   * in contiguous memory. */
  template <typename Value, typename Parser>
  class array
  {
    public:
      typedef array<Value, Parser> this_t;
      typedef Value value_t;
      typedef Parser parser_t;
      typedef uint32_t index_t;
      typedef char const * const cstr_t;
      typedef std::vector<Value> array_t;
      typedef typename array_t::iterator iterator;
      typedef typename array_t::const_iterator const_iterator;

      static index_t const npos = 0xFFFFFFFF;
      static char const delim_open = '[';
      static char const delim_close = ']';

      array()
      { }
      explicit array(std::string const &_json)
      { load(_json); }

      template <typename T>
      inline T& get(index_t _index)
      { return m_values[_index].template as<T>(); }
      template <typename T>
      inline T& get(index_t _index, T const &)
      { return m_values[_index].template as<T>(); } /* TODO: Additional checking here? */
      inline this_t& get_array(index_t _index)
      { return m_values[_index].template as<this_t >(); }
      inline map<value_t, parser_t>& get_map(index_t _index)
      { return m_values[_index].template as<map<value_t, parser_t> >(); }

      /* Searches for the specified value. */
      inline iterator find(value_t const &_val)
      {
        for(typename array_t::iterator it(m_values.begin()); it != m_values.end(); ++it)
          if(*it == _val)
            return it;
        return end();
      }
      inline const_iterator find(value_t const &_val) const
      {
        for(typename array_t::const_iterator it(m_values.begin()); it != m_values.end(); ++it)
          if(*it == _val)
            return it;
        return cend();
      }

      inline iterator begin()
      { return m_values.begin(); }
      inline const_iterator cbegin() const
      { return m_values.begin(); }

      inline iterator end()
      { return m_values.end(); }
      inline const_iterator cend() const
      { return m_values.end(); }

      inline size_t size() const
      { return m_values.size(); }
      inline bool empty() const
      { return m_values.empty(); }

      inline void clear()
      { m_values.clear(); }
      inline void reset(std::string const &_json)
      { *this = load(_json); }

      /* Stores the specified value at the specified index.
       * The specified index should already exist. */
      template <typename T>
      inline void set(index_t _index, T const &_t)
      { m_values[_index] = _t; }
      inline void set(index_t _index, cstr_t _str)
      { m_values[_index] = std::string(_str); }

      /* Erases ONE value, starting at position _index_. */
      inline void erase(index_t _index)
      { m_values.erase(m_values.begin() + _index); }

      /* Erases _amount_ number of objects from the starting
       * point _index_. This does no bounds checking. */
      inline void erase(index_t _index, std::size_t _amount)
      { m_values.erase(m_values.begin() + _index, m_values.begin() + _index + _amount); }

      template <typename T>
      inline void add(T const &_t)
      { m_values.push_back(Value(_t)); }

      /* Finds the specified value and returns the index of it.
       * If the value is not found, array::npos is returned. */
      template <typename T>
      index_t find(T const &_val)
      {
        for(index_t i = 0; i < m_values.size(); ++i)
          if(m_values == _val)
            return i;
        return npos;
      }

      /* Loads the specified JSON string. */
      inline void load(std::string const &_json)
      { *this = Parser::template parse< this_t >(_json); }
      static inline this_t load_new(std::string const &_json)
      { return Parser::template parse< this_t >(_json); }

      /* Loads the specified JSON file. */
      inline void load_file(std::string const &_json_file)
      { *this = Parser::template parse_file< this_t >(_json_file); }
      static inline this_t load_new_file(std::string const &_json_file)
      { return Parser::template parse_file< this_t >(_json_file); }

      /* Writes the JSON data to string form. */
      inline std::string to_string()
      { return Parser::template save< this_t >(*this); }

      template <typename ValueT, typename ParserT>
      friend std::ostream& operator <<(std::ostream &stream, array<ValueT, ParserT> const &_arr);

    private:
      array_t m_values;

  }; /* Class array */
} /* Namespace jeayeson */

#endif /* JEAYESON_JSONARRAY_H */


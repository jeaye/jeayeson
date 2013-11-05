/*
  Copyright © 2013 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: array.h
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
#pragma mark - Types
      typedef map<Value, Parser> map_t;
      typedef array<Value, Parser> array_t;
      typedef Value value_t;
      typedef Parser parser_t;
      typedef uint32_t index_t;
      typedef std::string key_t;
      typedef char const * const cstr_t;
      typedef std::vector<Value> internal_array_t;
      typedef typename internal_array_t::iterator iterator;
      typedef typename internal_array_t::const_iterator const_iterator;

      static index_t const npos = 0xFFFFFFFF;
      static char const delim_open = '[';
      static char const delim_close = ']';

#pragma mark - Construction
      inline array()
      { }
      inline array(array const &_array) : m_values(_array.m_values)
      { }
      inline explicit array(std::string const &_json)
      { load(_json); }
      inline explicit array(value_t const &_val)
      {
        if(_val.get_type() == value_t::type_array)
          *this = _val.template as<array_t>();
        else
          throw std::runtime_error("Failed to construct array from non-array");
      }
      template <typename T>
      inline explicit array(T const &_container)
      {
        reserve(_container.size());
        for(auto const &it : _container)
          add(it);
      }

#pragma mark - Accessors
      /* Access the internal variant type. */
      inline value_t& get(index_t const &_index)
      { return m_values[_index]; }
      inline value_t const& get(index_t const &_index) const
      { return m_values[_index]; }

      template <typename T>
      inline T& get(index_t _index) const
      { return m_values[_index].template as<T>(); }
      template <typename T>
      inline T& get(index_t _index, T const &) const /* TODO: Doesn't use fallback. */
      { return m_values[_index].template as<T>(); }
      inline std::string& get_string(index_t _index) const
      { return m_values[_index].template as<std::string>(); }
      inline array_t& get_array(index_t _index) const 
      { return m_values[_index].template as<array_t >(); }
      inline map<value_t, parser_t>& get_map(index_t _index) const
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

#pragma mark - Array Subscript Operators
      inline value_t& operator [](index_t const _index)
      { return m_values[_index]; }
      inline value_t const& operator [](index_t const _index) const
      { return m_values[_index]; }

#pragma mark - Searching
      /* Finds the specified value and returns the index of it.
       * If the value is not found, array::npos is returned. */
      template <typename T>
      inline index_t find(T const &_val) const
      {
        for(index_t i(0); i < m_values.size(); ++i)
          if(m_values[i] == _val)
            return i;
        return npos;
      }

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

      inline size_t size() const
      { return m_values.size(); }
      inline bool empty() const
      { return m_values.empty(); }

#pragma mark - Mutators
      /* Stores the specified value at the specified index.
       * The specified index should already exist. */
      template <typename T>
      inline void set(index_t _index, T const &_t)
      { m_values[_index] = _t; }

      /* Specialized mutators. */
      inline void set(index_t const &_index, int8_t const _value)
      { m_values[_index] = static_cast<int64_t>(_value); }
      inline void set(index_t const &_index, uint8_t const _value)
      { m_values[_index] = static_cast<int64_t>(_value); }
      inline void set(index_t const &_index, int16_t const _value)
      { m_values[_index] = static_cast<int64_t>(_value); }
      inline void set(index_t const &_index, uint16_t const _value)
      { m_values[_index] = static_cast<int64_t>(_value); }
      inline void set(index_t const &_index, int32_t const _value)
      { m_values[_index] = static_cast<int64_t>(_value); }
      inline void set(index_t const &_index, uint32_t const _value)
      { m_values[_index] = static_cast<int64_t>(_value); }
      inline void set(index_t const &_index, long const _value)
      { m_values[_index] = static_cast<int64_t>(_value); }
      inline void set(index_t const &_index, unsigned long const _value)
      { m_values[_index] = static_cast<int64_t>(_value); }
      inline void set(index_t const &_index, int64_t const _value)
      { m_values[_index] = _value; }
      inline void set(index_t const &_index, uint64_t const _value)
      { m_values[_index] = _value; }

      inline void set(index_t const &_index, bool const _value)
      { m_values[_index] = _value; }

      inline void set(index_t const &_index, float const _value)
      { m_values[_index] = static_cast<double>(_value); }
      inline void set(index_t const &_index, double const _value)
      { m_values[_index] = _value; }

      inline void set(index_t const &_index, cstr_t _value)
      { m_values[_index] = static_cast<std::string>(_value); }
      inline void set(index_t const &_index, std::string const &_value)
      { m_values[_index] = _value; }

      template <typename T>
      inline void add(T const &_t)
      { m_values.push_back(Value(_t)); }

      /* Erases ONE value, starting at position _index_. */
      inline void erase(index_t _index)
      { m_values.erase(m_values.begin() + _index); }

      /* Erases _amount_ number of objects from the starting
       * point _index_. This does no bounds checking. */
      inline void erase(index_t _index, std::size_t _amount)
      { m_values.erase(m_values.begin() + _index, m_values.begin() + _index + _amount); }

      inline void clear()
      { m_values.clear(); }
      inline void reset(std::string const &_json)
      { *this = load(_json); }

      inline void reserve(std::size_t const _size)
      { m_values.reserve(_size); }

      /* Loads the specified JSON string. */
      inline bool load(std::string const &_json)
      { *this = Parser::template parse< array_t >(_json); return true; }
      static inline array_t load_new(std::string const &_json)
      { return Parser::template parse< array_t >(_json); }

      /* Loads the specified JSON file. */
      inline void load_file(std::string const &_json_file)
      { *this = Parser::template parse_file< array_t >(_json_file); }
      static inline array_t load_new_file(std::string const &_json_file)
      { return Parser::template parse_file< array_t >(_json_file); }

      /* Writes the JSON data to string form. */
      inline std::string to_string() const
      { return Parser::template save< array_t >(*this); }

      template <typename Stream_Value, typename Stream_Parser>
      friend std::ostream& operator <<(std::ostream &stream, array<Stream_Value, Stream_Parser> const &_arr);

    private:
#pragma mark - Members
      /* Mutable for operator[] access. */
      mutable internal_array_t m_values;

  }; /* Class array */
} /* Namespace jeayeson */

#endif /* JEAYESON_JSONARRAY_H */


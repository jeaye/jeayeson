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

namespace JeayeSON
{
  template <typename Value>
  class Array
  {
    public:
      static size_t const npos;
      typedef uint32_t index_t;
      typedef char const * const cstr_t;

      Array()
      { }
      Array(std::string const &json)
      { if(load(json) == false) throw "Failed to load json!"; }
      virtual ~Array()
      { }

      /* Erases ONE value, starting at position _index_. */
      inline void erase(index_t index)
      { m_values.erase(m_values.begin() + index); }

      /* Erases _amount_ number of objects from the starting
       * point _index_. This does no bounds checking. */
      inline void erase(index_t index, size_t amount)
      { m_values.erase(m_values.begin() + index, m_values.begin() + index + amount); }

      /* Stores the specified value at the specified index.
       * The specified index should already exist. */
      template <typename T>
      inline void set(index_t index, T const &t)
      { m_values[index] = t; }
      inline void set(index_t index, cstr_t str)
      { m_values[index] = std::string(str); }

      template <typename T>
      inline void add(T const &t)
      { m_values.push_back(t); }

      inline void size() const
      { return m_values.size(); }
      inline bool isEmpty() const
      { return m_values.empty(); }

      /* Finds the specified value and returns the index of it.
       * If the value is not found, Array::npos is returned. */
      template <typename T>
      index_t find(T const &val)
      {
        for(index_t i = 0; i < m_values.size(); ++i)
          if(m_values == val)
            return i;
        return npos;
      }

      /* Parses the specified Json string into a map. */
      bool load(std::string const &json);
      bool loadFile(std::string const &jsonFile);

    private:
      std::vector<Value> m_values;

  }; /* Class Array */
} /* Namespace JeayeSON */

#endif /* JEAYESON_JSONARRAY_H */


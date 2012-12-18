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
  template <typename Value, typename Parser>
  class Map;

  template <typename Value, typename Parser>
  class Array
  {
    public:
      typedef Array<Value, Parser> this_t;
      typedef Value value_t;
      typedef Parser parser_t;
      typedef uint32_t index_t;
      typedef char const * const cstr_t;
      typedef std::vector<Value> array_t;

      static index_t const npos;
      static char const delimOpen = '[';
      static char const delimClose = ']';

      Array()
      { }
      explicit Array(std::string const &json)
      { if(load(json) == false) throw "Failed to load json!"; }

      template <typename T>
      inline T get(index_t index)
      { return m_values[index].template as<T>(); }
      inline this_t& getArray(index_t index)
      { return m_values[index].template as<this_t >(); }
      inline Map<value_t, parser_t>& getMap(index_t index)
      { return m_values[index].template as<Map<value_t, parser_t> >(); }

      inline size_t getSize() const
      { return m_values.size(); }
      inline bool isEmpty() const
      { return m_values.empty(); }

      /* Stores the specified value at the specified index.
       * The specified index should already exist. */
      template <typename T>
      inline void set(index_t index, T const &t)
      { m_values[index] = t; }
      inline void set(index_t index, cstr_t str)
      { m_values[index] = std::string(str); }

      /* Erases ONE value, starting at position _index_. */
      inline void erase(index_t index)
      { m_values.erase(m_values.begin() + index); }

      /* Erases _amount_ number of objects from the starting
       * point _index_. This does no bounds checking. */
      inline void erase(index_t index, std::size_t amount)
      { m_values.erase(m_values.begin() + index, m_values.begin() + index + amount); }

      template <typename T>
      inline void add(T const &t)
      { m_values.push_back(Value(t)); }

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

      /* Loads the specified JSON string. */
      inline void load(std::string const &json)
      { *this = Parser::template parse< this_t >(json); }
      static inline this_t loadNew(std::string const &json)
      { return Parser::template parse< this_t >(json); }

      /* Loads the specified JSON file. */
      inline void loadFile(std::string const &jsonFile)
      { *this = Parser::template parseFile< this_t >(jsonFile); }
      static inline this_t loadNewFile(std::string const &jsonFile)
      { return Parser::template parseFile< this_t >(jsonFile); }

      /* Writes the JSON data to string form. */
      inline std::string jsonString()
      { return Parser::template save< this_t >(*this); }

      template <typename ValueT, typename ParserT>
      friend std::ostream& operator <<(std::ostream &stream, Array<ValueT, ParserT> const &arr);

    private:
      array_t m_values;

  }; /* Class Array */

  template <typename Value, typename Parser>
  std::ostream& operator <<(std::ostream &stream, Array<Value, Parser> const &arr)
  {
    stream << arr.delimOpen;
    for(typename Array<Value, Parser>::array_t::const_iterator i = arr.m_values.begin();
        i != arr.m_values.end(); ++i, stream.put(','))
      stream << *i;

    /* Replace the last comma with the object's close delim. */
    stream.seekp(-1, std::ios_base::end);
    stream.put(arr.delimClose);

    return stream;
  }

} /* Namespace JeayeSON */

#endif /* JEAYESON_JSONARRAY_H */


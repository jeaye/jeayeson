/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: array.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <utility>
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
      using map_t = map<Value, Parser>;
      using array_t = array<Value, Parser>;
      using value_t = Value;
      using parser_t = Parser;
      using index_t = uint32_t;
      using key_t = std::string;
      using cstr_t = char const * const;
      using internal_array_t = std::vector<Value>;
      using iterator = typename internal_array_t::iterator;
      using const_iterator = typename internal_array_t::const_iterator;

      static index_t const npos = 0xFFFFFFFF;
      static char const delim_open = '[';
      static char const delim_close = ']';

      array() = default;
      explicit array(std::string const &json)
      { load(json); }
      explicit array(value_t const &val)
      {
        if(val.get_type() == value_t::type_array)
        { *this = val.template as<array_t>(); }
        else
        { throw std::runtime_error("Failed to construct array from non-array"); }
      }
      template <typename T>
      explicit array(T const &container)
      {
        reserve(container.size());
        std::copy(container.begin(), container.end(), std::back_inserter(*this));
      }
      array(array const &arr) : values_(arr.values_)
      { }

      /* Access the internal variant type. */
      value_t& get(index_t const index)
      { return values_[index]; }
      value_t const& get(index_t const index) const
      { return values_[index]; }

      template <typename T>
      T& get(index_t const index) const
      { return values_[index].template as<T>(); }
      template <typename T>
      T& get(index_t const index, T const &) const /* TODO: Doesn't use fallback. */
      { return values_[index].template as<T>(); }

      /* Named specialization. */
      array_t& get_array(index_t const index)
      { return values_[index].template as<array_t>(); }
      array_t const & get_array(index_t const index) const 
      { return values_[index].template as<array_t>(); }

      map<value_t, parser_t>& get_map(index_t const index)
      { return values_[index].template as<map<value_t, parser_t>>(); }
      map<value_t, parser_t> const & get_map(index_t const index) const
      { return values_[index].template as<map<value_t, parser_t>>(); }

      /* Searches for the specified value. */
      template <typename T>
      iterator find(T const &val)
      { return values_.find(val); }
      template <typename T>
      const_iterator find(T const &val) const
      { return values_.find(val); }

      value_t& operator [](index_t const index)
      { return values_[index]; }
      value_t const& operator [](index_t const index) const
      { return values_[index]; }

      iterator begin()
      { return values_.begin(); }
      const_iterator begin() const
      { return values_.begin(); }
      const_iterator cbegin() const
      { return values_.begin(); }

      iterator end()
      { return values_.end(); }
      const_iterator end() const
      { return values_.end(); }
      const_iterator cend() const
      { return values_.end(); }

      size_t size() const
      { return values_.size(); }
      bool empty() const
      { return values_.empty(); }

      /* Stores the specified value at the specified index.
       * The specified index should already exist. */
      template <typename T>
      void set(index_t const index, T &&t)
      { values_[index] = std::forward<T>(t); }
      void set(index_t const &index, cstr_t const value)
      { values_[index] = std::string{ value }; }

      template <typename T>
      void add(T &&t)
      { values_.push_back(Value{ std::forward<T>(t) }); }
      template <typename T>
      void push_back(T &&t)
      { values_.push_back(Value{ std::forward<T>(t) }); }

      /* Erases ONE value, starting at position _index_. */
      void erase(index_t const index)
      { values_.erase(values_.begin() + index); }

      /* Erases _amount_ number of objects from the starting
       * point _index_. This does no bounds checking. */
      void erase(index_t const index, size_t const amount)
      { values_.erase(values_.begin() + index, values_.begin() + index + amount); }

      void clear()
      { values_.clear(); }
      void reset(std::string const &json)
      { *this = load(json); }

      void reserve(size_t const size)
      { values_.reserve(size); }

      void load(std::string const &json)
      { *this = Parser::template parse<array_t>(json); }
      static array_t load_new(std::string const &json)
      { return Parser::template parse<array_t>(json); }

      void load_file(std::string const &json_file)
      { *this = Parser::template parse_file<array_t>(json_file); }
      static array_t load_new_file(std::string const &json_file)
      { return Parser::template parse_file<array_t>(json_file); }

      std::string to_string() const
      { return Parser::template save<array_t>(*this); }

      template <typename Stream_Value, typename Stream_Parser>
      friend std::ostream& operator <<(std::ostream &stream,
                                       array<Stream_Value, Stream_Parser> const &arr);

    private:
      /* Mutable for operator[] access. */
      mutable internal_array_t values_;
  };
}

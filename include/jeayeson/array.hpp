/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
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
#include <stdexcept>
#include <initializer_list>

#include "detail/normalize.hpp"
#include "file.hpp"

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
      using value_type = Value;
      using parser_t = Parser;
      using index_t = uint32_t;
      using key_t = std::string;
      using internal_array_t = std::vector<Value>;
      using iterator = typename internal_array_t::iterator;
      using const_iterator = typename internal_array_t::const_iterator;

      static index_t const npos = 0xFFFFFFFF;
      static char const delim_open = '[';
      static char const delim_close = ']';

      array(){} /* XXX: User-defined ctor required for variant. */
      array(array const &arr)
        : values_{ arr.values_ }
      { }
      array(data const &json)
      { reset(json); }
      array(file const &f)
      { reset(f); }

      template
      <
        typename It,
        typename = std::enable_if_t
        <
          std::is_convertible
          <
            typename std::iterator_traits<It>::iterator_category,
            std::input_iterator_tag
          >::value
        >
      >
      array(It const &begin, It const &end)
      {
        reserve(std::distance(begin, end));
        std::copy(begin, end, std::back_inserter(*this));
      }

      /* TODO: Allow different types. */
      template
      <
        typename T,
        typename E = std::enable_if_t<detail::is_convertible<T, value_type>()>
      >
      array(std::initializer_list<T> const &list)
      {
        reserve(std::distance(list.begin(), list.end()));
        std::copy(list.begin(), list.end(), std::back_inserter(*this));
      }

      template <typename T = Value>
      auto& get(index_t const index) const
      { return values_[index].template as<T>(); }
      template <typename T = Value>
      auto get(index_t const index, T const &fallback) const
      {
        if(index >= values_.size())
        { return static_cast<detail::normalize<T>>(fallback); }
        return values_[index].template as<T>();
      }

      template <typename T>
      iterator find(T const &val)
      { return std::find(values_.begin(), values_.end(), val); }
      template <typename T>
      const_iterator find(T const &val) const
      { return std::find(values_.begin(), values_.end(), val); }

      value_type& operator [](index_t const index)
      { return values_[index]; }
      value_type const& operator [](index_t const index) const
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

      template <typename T>
      void set(index_t const index, T &&t)
      { values_[index] = std::forward<T>(t); }
      void set(index_t const &index, std::nullptr_t)
      { values_[index] = typename Value::null_t{}; }

      template <typename T>
      void push_back(T &&t)
      { values_.push_back(Value(std::forward<T>(t))); }

      void erase(index_t const index)
      { values_.erase(values_.begin() + index); }
      iterator erase(const_iterator const it)
      { return values_.erase(it); }
      iterator erase(const_iterator const first, const_iterator const second)
      { return values_.erase(first, second); }

      void erase(index_t const index, size_t const amount)
      { values_.erase(values_.begin() + index, values_.begin() + index + amount); }

      void clear()
      { values_.clear(); }

      void reserve(size_t const size)
      { values_.reserve(size); }

      void reset(data const &json)
      { *this = Parser::template parse<array_t>(json.data); }
      void reset(std::string const &json)
      { *this = Parser::template parse<array_t>(json); }
      void reset(file const &f)
      { *this = Parser::template parse<array_t>(f); }

      std::string to_string() const
      { return Parser::template save<array_t>(*this); }

      template <typename Stream_Value, typename Stream_Parser>
      friend std::ostream& operator <<
      (
        std::ostream &stream,
        array<Stream_Value, Stream_Parser> const &arr
      );
      template <typename V, typename P>
      friend bool operator ==(array<V, P> const &lhs, array<V, P> const &rhs);
      template <typename V, typename P>
      friend bool operator !=(array<V, P> const &lhs, array<V, P> const &rhs);

    private:
      /* Mutable for operator[] access. */
      mutable internal_array_t values_;
  };

  template <typename V, typename P>
  bool operator ==(array<V, P> const &lhs, array<V, P> const &rhs)
  { return lhs.values_ == rhs.values_; }
  template <typename V, typename P>
  bool operator !=(array<V, P> const &lhs, array<V, P> const &rhs)
  { return !(lhs == rhs); }
}

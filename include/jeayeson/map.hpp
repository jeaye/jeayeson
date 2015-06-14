/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: map.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include "detail/normalize.hpp"
#include "detail/config.hpp"
#include "detail/tokenize.hpp"
#include "file.hpp"
#include "data.hpp"

#include <string>
#include <vector>
#include <utility>
#include <initializer_list>

namespace jeayeson
{
  template <typename Value, typename Parser>
  class array;

  /* Maps provide a wrapper for
   * string-indexed values, which
   * could be any valid JSON object.
   */
  template <typename Value, typename Parser>
  class map
  {
    public:
      using map_t = map<Value, Parser>;
      using array_t = array<Value, Parser>;
      using key_t = std::string;
      using value_type = Value;
      using parser_t = Parser;
      using internal_map_t = config<config_tag>::map_t<key_t, Value>;
      using iterator = typename internal_map_t::iterator;
      using const_iterator = typename internal_map_t::const_iterator;

      static char const delim_open = '{';
      static char const delim_close = '}';

      map(){} /* XXX: User-defined ctor required for variant. */
      map(map const &m)
        : values_{ m.values_ }
      { }
      map(data const &json)
      { reset(json); }
      map(std::string const &json)
      { reset(json); }
      map(file const &f)
      { reset(f); }
      template <typename T>
      map(std::map<key_t, T> const &container)
      {
        for(auto const &it : container)
        { set(it.first, it.second); }
      }
      map(std::initializer_list<std::pair<key_t const, value_type>> const &list)
        : values_{ list }
      { }

      template <typename T = Value>
      auto& get(key_t const &key)
      { return values_[key].template as<T>(); }
      template <typename T = Value>
      auto const& get(key_t const &key) const
      { return values_[key].template as<T>(); }

      template <typename T = Value>
      auto get(key_t const &key, T &&fallback) const
      {
        auto const it(values_.find(key));
        if(it != values_.end())
        { return it->second.template as<T>(); }
        else
        { return static_cast<detail::normalize<T>>(fallback); }
      }

      Value& operator [](key_t const &key)
      { return get(key); }
      Value const& operator [](key_t const &key) const
      { return get(key); }

      bool empty() const
      { return values_.empty(); }
      size_t size() const
      { return values_.size(); }

      template <typename T = Value>
      T& get_for_path(std::string const &path) const
      {
        std::vector<std::string> const tokens(detail::tokenize(path, "."));
        size_t const path_size(tokens.size() - 1);

        map_t *sub_map(const_cast<map_t*>(this));
        for(size_t i{}; i < path_size; ++i)
        { sub_map = &sub_map->get<map_t>(tokens[i]); }

        return sub_map->get<T>(tokens[path_size]);
      }

      template <typename T = Value>
      T get_for_path(std::string const &path, T &&fallback) const
      {
        std::vector<std::string> const tokens(detail::tokenize(path, "."));
        size_t const path_size(tokens.size() - 1);

        map_t const *sub_map(const_cast<map_t*>(this));
        for(size_t i{}; i < path_size; ++i)
        {
          auto const it(sub_map->find(tokens[i]));
          if(it == sub_map->end())
          { return std::forward<T>(fallback); }

          sub_map = &(it->second.template as<map_t>());
        }

        auto const it(sub_map->find(tokens[path_size]));
        if(it == sub_map->end())
        { return std::forward<T>(fallback); }
        return it->second.template as<T>();
      }

      std::vector<key_t> get_keys() const
      {
        std::vector<key_t> keys;
        keys.reserve(values_.size());
        for(auto const &it : values_)
        { keys.push_back(it.first); }
        return keys;
      }

      array_t get_values() const
      {
        array_t arr;
        arr.reserve(values_.size());
        for(auto const &it : values_)
        { arr.push_back(it.second); }
        return arr;
      }

      iterator find(key_t const &key)
      { return values_.find(key); }
      const_iterator find(key_t const &key) const
      { return values_.find(key); }

      bool has(key_t const &key)
      { return find(key) != end(); }
      bool has(key_t const &key) const
      { return find(key) != end(); }

      template<typename VT>
      bool has(key_t const &key)
      {
        auto const it(find(key));
        return it != end() && it->second.is(Value::template to_value<VT>::value);
      }
      template<typename VT>
      bool has(key_t const &key) const
      {
        auto const it(find(key));
        return it != end() && it->second.is(Value::template to_value<VT>::value);
      }

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

      template <typename T>
      void set(key_t const &key, T &&value)
      { values_[key] = std::forward<T>(value); }
      void set(key_t const &key, std::nullptr_t)
      { values_[key] = typename Value::null_t{}; }

      void clear()
      { values_.clear(); }

      void erase(key_t const &key)
      { values_.erase(key); }

      void merge(map const &m)
      { values_.insert(m.values_.begin(), m.values_.end()); }

      void reset(data const &json)
      { reset(json.data); }
      void reset(std::string const &json)
      { *this = Parser::template parse<map_t>(json); }
      void reset(file const &f)
      { *this = Parser::template parse<map_t>(f); }

      std::string to_string() const
      { return Parser::template save<map_t>(*this); }

      template <typename Stream_Value, typename Stream_Parser>
      friend std::ostream& operator <<
      (
        std::ostream &stream,
        map<Stream_Value, Stream_Parser> const &m
      );
      template <typename V, typename P>
      friend bool operator ==(map<V, P> const &lhs, map<V, P> const &rhs);
      template <typename V, typename P>
      friend bool operator !=(map<V, P> const &lhs, map<V, P> const &rhs);

    private:
      mutable internal_map_t values_;
  };

  template <typename V, typename P>
  bool operator ==(map<V, P> const &lhs, map<V, P> const &rhs)
  { return lhs.values_ == rhs.values_; }
  template <typename V, typename P>
  bool operator !=(map<V, P> const &lhs, map<V, P> const &rhs)
  { return !(lhs == rhs); }
}

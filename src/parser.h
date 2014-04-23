/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: parser.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>

#include "defines.h"
#include "map.h"
#include "array.h"

namespace jeayeson
{
  class parser
  {
    private:
      using str_citer = std::string::const_iterator;

      enum class state_t { parse_name, parse_value };

/********************* Helper Accessors *********************/
      template <typename Value, typename Parser, typename T>
      static inline state_t add(map<Value, Parser> &m, std::string const &key, T const &t)
      {
        m.set(key, t);
        return state_t::parse_name;
      }

      template <typename Value, typename Parser, typename T>
      static inline state_t add(array<Value, Parser> &arr, std::string const &, T const &t)
      {
        arr.add(t);
        return state_t::parse_value;
      }

      template <typename Value, typename Parser>
      static inline map<Value, Parser>& get_map(map<Value, Parser> &m, std::string const &key,
                                                typename array<Value, Parser>::index_t const)
      { return m.get_map(key); }

      template <typename Value, typename Parser>
      static inline map<Value, Parser>& get_map(array<Value, Parser> &arr, std::string const &,
                                                typename array<Value, Parser>::index_t const index)
      { return arr.get_map(index); }

      template <typename Value, typename Parser>
      static inline array<Value, Parser>& get_array(map<Value, Parser> &m, std::string const &key,
                                                    typename array<Value, Parser>::index_t const)
      { return m.get_array(key); }

      template <typename Value, typename Parser>
      static inline array<Value, Parser>& get_array(array<Value, Parser> &arr, std::string const &,
                                                    typename array<Value, Parser>::index_t const index)
      { return arr.get_array(index); }
/********************* Helper Accessors *********************/

      template <typename Container>
      static void parse(Container &container, str_citer &it)
      {
        using json_map = map<typename Container::value_t, typename Container::parser_t>;
        using json_array = array<typename Container::value_t, typename Container::parser_t>;

        std::string name;
        name.reserve(128);
        std::string value;
        value.reserve(128);

        /* Maps start out parsing keys, arrays just want values. */
        state_t state(container.delim_open == json_map::delim_open ?
                      state_t::parse_name : state_t::parse_value);

        while(*it)
        {
          switch(*it)
          {
            /* Start of a new map. */
            case json_map::delim_open:
            {
              ++it;

              state = add(container, name, json_map());
              parse(get_map(container, name, container.size() - 1), it);
            } break;

            /* Start of a new array. */
            case json_array::delim_open:
            {
              ++it;

              state = add(container, name, json_array());
              parse(get_array(container, name, container.size() - 1), it);
            } break;

            /* End of the current node. */
            case json_map::delim_close:
            case json_array::delim_close:
              ++it;
              return;

            /* Start of a value (the key). */
            case '"':
            {
              ++it;

              if(state == state_t::parse_value)
              {
                value.clear();
                for( ; *it != '"'; ++it)
                {
                  if(*it == '\\' && *(it + 1) == '"')
                  { value += *(++it); }
                  else
                  { value += *it; }
                }

                state = add(container, name, value);
              }
              else /* Parsing a key/name. */
              {
                name.clear();
                for( ; *it != '"'; ++it)
                {
                  if(*it == '\\' && *(it + 1) == '"')
                  { name += *(++it); }
                  else
                  { name += *it; }
                }

                state = state_t::parse_value;
              }

              ++it;
            } break;

            /* Start of an int or float. */
            case '-':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            {
              /* Determine if the value is integral or floating point. */
              str_citer is_int{ it };
              while(*is_int == '-' || (*is_int >= '0' && *is_int <= '9'))
              { ++is_int; }
              if(*is_int == '.' || *is_int == 'e' || *is_int == 'E')
              { state = add(container, name, std::atof(&*it)); }
              else
              { state = add(container, name, std::atoi(&*it)); }

              /* Progress to the next element. */
              while(*it == '-' || *it == '.' || (*it >= '0' && *it <= '9'))
              { ++it; }
              ++it;
            } break;

            /* Start of null, true, or false. */
            case 'n':
            case 't':
            case 'f':
            {
              if(*it == 'n' && *(it + 1) == 'u' && *(it + 2) == 'l' && *(it + 3) == 'l')
              { state = add(container, name, typename Container::value_t()); }
              else if(*it == 't' && *(it + 1) == 'r' && *(it + 2) == 'u' && *(it + 3) == 'e')
              { state = add(container, name, true); }
              else
              { state = add(container, name, false); }

              /* Progress to the next element. */
              while(*it != ',' && *it != json_map::delim_close)
              { ++it; }
              ++it;
            } break;

            /* Whitespace or unimportant/unknown characters. */
            default:
              ++it;
              break;
          }
        }
      }

    public:

      template <typename Container>
      static Container parse_file(std::string const &json_file)
      {
        std::ifstream file(json_file.c_str());

        if(!file.is_open())
        { throw std::runtime_error("Failed to parse non-existent file."); }

        file.seekg(0, std::ios_base::end);
        int64_t const file_size{ file.tellg() };
        file.seekg(0, std::ios_base::beg);

        std::string json;
        json.reserve(file_size);
        std::getline(file, json, static_cast<char>(-1)); /* EOF */
        file.close();

        return parse<Container>(json);
      }

      template <typename Container>
      static Container parse(std::string const &json_string)
      {
        str_citer it{ json_string.begin() };

        while(*it)
        {
          switch(*it)
          {
            case Container::delim_open:
            {
              ++it;

              /* Recursively parse the object. */
              Container c;
              parse(c, it);
              return c;

            } break;

            /* Whitespace. */
            default:
            {
              ++it;
            } break;
          }
        }

        return Container{};
      }

      template <typename Container>
      static inline std::string save(Container const &container)
      {
        std::stringstream output;
        output << container;
        return output.str();
      }
  };
}

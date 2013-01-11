/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: JsonParser.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef JEAYESON_JSONPARSER_H
#define JEAYESON_JSONPARSER_H

#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>

#if !JEAYESON_STD_FSTREAM_LOAD
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#endif

#include "Defines.h"
#include "JsonMap.h"
#include "JsonArray.h"

#if __cplusplus >= 201103L
#define JEAYESON_CPP11
#define MOVE(x) std::move((x))
#else
#define MOVE(x) (x)
#endif

namespace jeayeson
{
  class parser
  {
    private:
      typedef std::string::const_iterator str_citer;

      enum state_t { parse_name, parse_value };

/********************* Helper Accessors *********************/
      template <typename Value, typename Parser, typename T>
      static inline state_t add(map<Value, Parser> &_map, std::string const &_key, T const &_t)
      {
        _map.set(_key, _t);
        return parse_name;
      }

      template <typename Value, typename Parser, typename T>
      static inline state_t add(array<Value, Parser> &_arr, std::string const &, T const &_t)
      {
        _arr.add(_t);
        return parse_value;
      }

      template <typename Value, typename Parser>
      static inline map<Value, Parser>& get_map(map<Value, Parser> &_map, std::string const &_key,
                                              typename array<Value, Parser>::index_t)
      { return _map.get_map(_key); }

      template <typename Value, typename Parser>
      static inline map<Value, Parser>& get_map(array<Value, Parser> &_arr, std::string const &,
                                              typename array<Value, Parser>::index_t _index)
      { return _arr.get_map(_index); }

      template <typename Value, typename Parser>
      static inline array<Value, Parser>& get_array(map<Value, Parser> &_map, std::string const &_key,
                                                  typename array<Value, Parser>::index_t)
      { return _map.get_array(_key); }

      template <typename Value, typename Parser>
      static inline array<Value, Parser>& get_array(array<Value, Parser> &_arr, std::string const &,
                                                  typename array<Value, Parser>::index_t _index)
      { return _arr.get_array(_index); }
/********************* Helper Accessors *********************/

      template <typename Container>
      static void parse(Container &_container, str_citer &_it)
      {
        typedef map<typename Container::value_t, typename Container::parser_t> json_map;
        typedef array<typename Container::value_t, typename Container::parser_t> json_array;

        std::string name;
        name.reserve(128);
        std::string value;
        value.reserve(128);

        /* Maps start out parsing keys, arrays just want values. */
        state_t state(_container.delim_open == json_map::delim_open ? parse_name : parse_value);

        while(*_it)
        {
          switch(*_it)
          {
            /* Start of a new map. */
            case json_map::delim_open:
            {
              ++_it;

              state = add(_container, name, json_map());
              parse(get_map(_container, name, _container.size() - 1), _it);
            } break;

            /* Start of a new array. */
            case json_array::delim_open:
            {
              ++_it;

              state = add(_container, name, json_array());
              parse(get_array(_container, name, _container.size() - 1), _it);
            } break;

            /* End of the current node. */
            case json_map::delim_close:
            case json_array::delim_close:
              ++_it;
              return;

            /* Start of a value (the key). */
            case '"':
            {
              ++_it;

              if(state == parse_value)
              {
                value.clear();
                for( ; *_it != '"'; ++_it)
                  value += *_it;

                state = add(_container, name, value);
              }
              else // Parsing a key/name
              {
                name.clear();
                for( ; *_it != '"'; ++_it)
                  name += *_it;

                state = parse_value;
              }

              ++_it;
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
              str_citer is_int(_it);
              while(*is_int == '-' || (*is_int >= '0' && *is_int <= '9'))
                ++is_int;
              if(*is_int == '.' || *is_int == 'e' || *is_int == 'E')
                state = add(_container, name, std::atof(&*_it));
              else
                state = add(_container, name, std::atoi(&*_it));

              /* Progress to the next element. */
              while(*_it == '-' || *_it == '.' || (*_it >= '0' && *_it <= '9'))
                ++_it;
              ++_it;
            } break;

            /* Start of null, true, or false. */
            case 'n':
            case 't':
            case 'f':
            {
              if(*_it == 'n' && *(_it + 1) == 'u' && *(_it + 2) == 'l' && *(_it + 3) == 'l')
                state = add(_container, name, typename Container::value_t());
              else if(*_it == 't' && *(_it + 1) == 'r' && *(_it + 2) == 'u' && *(_it + 3) == 'e')
                state = add(_container, name, true);
              else
                state = add(_container, name, false);

              /* Progress to the next element. */
              while(*_it != ',' && *_it != json_map::delim_close)
                ++_it;
              ++_it;
            } break;

            /* Whitespace or unimportant characters. */
            default:
              ++_it;
              break;
          }
        }
      }

    public:

      template <typename Container>
      static Container parse_file(std::string const &_json_file)
      {
        std::string json;

#if JEAYESON_STD_FSTREAM_LOAD
        std::ifstream file(_json_file.c_str());
        std::size_t file_size = 0;

        /* Ensure the file was opened. */
        if(file.is_open() == false)
          throw "Failed to parse non-existent file.";

        /* Determine the file length. */
        file.seekg(0, std::ios_base::end);
        file_size = file.tellg();
        file.seekg(0, std::ios_base::beg);

        /* Reserve space in the string. */
        json.reserve(file_size);

        /* Read the file. */
        std::getline(file, json, static_cast<char>(-1)); // EOF
        file.close();
#else
        boost::interprocess::file_mapping file(json_file.c_str(), boost::interprocess::read_only);
        boost::interprocess::mapped_region region(file, boost::interprocess::read_only);

        char *str = static_cast<char*>(region.get_address());
        std::size_t elements = region.get_size() / sizeof(char);

        /* Reserve space for the string. */
        json.reserve(elements);

        /* Copy over the data. */
        json.assign(str, elements);
#endif

        /* Parse the file normally. */
        return parse<Container>(json);
      }

      template <typename Container>
      static Container parse(std::string const &_json_string)
      {
        typedef map<typename Container::value_t, typename Container::parser_t> json_map;
        typedef array<typename Container::value_t, typename Container::parser_t> json_array;

        str_citer it = _json_string.begin();

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

        return Container();
      }

      template <typename Container>
      static inline std::string save(Container const &_container)
      {
        std::stringstream output;
        output << _container;
        return output.str();
      }

  }; /* Class parser */
} /* Namespace jeayeson */

#endif /* JEAYESON_JSONPARSER_H */


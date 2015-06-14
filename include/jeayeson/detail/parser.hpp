/*
  Copyright © 2015 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: detail/parser.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>

#include "parser_util.hpp"
#include "escape.hpp"

namespace jeayeson
{
  namespace detail
  {
    class parser
    {
      private:
        template <typename Container>
        static str_citer parse(Container &container, str_citer it)
        {
          using json_map = map
          <
            typename Container::value_type,
            typename Container::parser_t
          >;
          using json_array = array
          <
            typename Container::value_type,
            typename Container::parser_t
          >;

          std::string name;
          std::string value;

          /* Maps start out parsing keys, arrays just want values. */
          state_t state
          {
            container.delim_open == json_map::delim_open ?
            state_t::parse_name : state_t::parse_value
          };

          while(*it)
          {
            switch(*it)
            {
              /* Start of a new map. */
              case json_map::delim_open:
              {
                ++it;

                state = push_back(container, name, json_map{});
                it = parse(get_map(container, name, container.size() - 1), it);
              } break;

              /* Start of a new array. */
              case json_array::delim_open:
              {
                ++it;

                state = push_back(container, name, json_array{});
                it = parse(get_array(container, name, container.size() - 1), it);
              } break;

              /* End of the current node. */
              case json_map::delim_close:
              case json_array::delim_close:
              { return ++it; }

              /* Start of a value or key. */
              case '"':
              {
                ++it;

                if(state == state_t::parse_value)
                {
                  value.clear();
                  for( ; *it != '"'; ++it)
                  {
                    if(*it == '\\')
                    {
                      if(*(it + 1) != 'u')
                      { value += escaped(*(++it)); }
                      else
                      {
                        std::string converted;
                        std::tie(it, converted) = utf16_to_8(it);
                        value += converted;
                      }
                    }
                    else
                    { value += *it; }
                  }

                  state = push_back(container, name, value);
                }
                else /* Parsing a key/name. */
                {
                  name.clear();
                  for( ; *it != '"'; ++it)
                  {
                    if(*it == '\\')
                    {
                      if(*(it + 1) != 'u')
                      { name += escaped(*(++it)); }
                      else
                      {
                        std::string converted;
                        std::tie(it, converted) = utf16_to_8(it);
                        name += converted;
                      }
                    }
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
                {
                  char *end{};
                  state = push_back(container, name, std::strtof(&*it, &end));
                  std::advance
                  (
                    it,
                    std::distance(&*it, const_cast<char const*>(end))
                  );
                }
                else
                { state = push_back(container, name, std::atoi(&*it)); }

                /* Progress to the next element. */
                while(*it == '-' || *it == '.' || (*it >= '0' && *it <= '9'))
                { ++it; }
              } break;

              /* Start of null, true, or false. */
              case 'n':
              case 't':
              case 'f':
              {
                if(std::equal(it, it + 3, "null"))
                {
                  state = push_back
                  (
                    container, name,
                    typename Container::value_type{}
                  );
                }
                else if(std::equal(it, it + 3, "true"))
                { state = push_back(container, name, true); }
                else
                { state = push_back(container, name, false); }

                /* Progress to the next element. */
                while
                (
                  *it != ',' && *it != json_map::delim_close &&
                  *it != json_array::delim_close
                )
                { ++it; }
              } break;

              /* Whitespace or unimportant/unknown characters. */
              default:
              { ++it; } break;
            }
          }

          return it;
        }

      public:
        template <typename Container>
        static Container parse(file const &json_file)
        {
          std::string json;
          {
            std::ifstream infile{ json_file.data.c_str() };
            if(!infile.is_open())
            {
              throw std::runtime_error
              { "failed to parse non-existent file: " + json_file.data };
            }

            infile.seekg(0, std::ios_base::end);
            auto const file_size(infile.tellg());
            infile.seekg(0, std::ios_base::beg);

            json.reserve(file_size);
            std::getline(infile, json, static_cast<char>(-1)); /* EOF */
          }

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
                it = parse(c, it);
                return c;
              }

              /* Whitespace. */
              default:
              { ++it; } break;
            }
          }

          return Container{};
        }

        template <typename Container>
        static std::string save(Container const &container)
        {
          std::stringstream output;
          output << container;
          return output.str();
        }
    };
  }
}

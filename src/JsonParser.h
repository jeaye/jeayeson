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
#warning Compiling with C++11 support!
#define JEAYESON_CPP11
#define MOVE(x) std::move((x))
#else
#define MOVE(x) (x)
#endif

namespace JeayeSON
{
  class Parser
  {
    private:
      typedef std::string::const_iterator str_citer;

      enum State { Parse_Name, Parse_Value };

      template <typename Map>
      static void parseMap(Map &map, str_citer &it)
      {
        typedef Map JsonMap;
        typedef Array<typename Map::value_t, typename Map::parser_t> JsonArray;

        std::string name;
        name.reserve(128);
        std::string value;
        value.reserve(128);

        State state(Parse_Name);

        while(*it)
        {
          switch(*it)
          {
            case JsonMap::delimOpen:
            {
              ++it;

              map.set(name, JsonMap());
              parseMap(map.getMap(name), it);

              state = Parse_Name;
            } break;

            case JsonArray::delimOpen:
            {

            } break;

            /* Start of a value (the key). */
            case '"':
            {
              ++it;

              if(state == Parse_Value)
              {
                value.clear();
                for( ; *it != '"'; ++it)
                  value += *it;

                map.set(name, value);
                state = Parse_Name;
              }
              else
              {
                name.clear();
                for( ; *it != '"'; ++it)
                  name += *it;

                state = Parse_Value;
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
              str_citer isInt(it);
              while(*isInt == '-' || (*isInt >= '0' && *isInt <= '9'))
                ++isInt;
              if(*isInt == '.' || *isInt == 'e' || *isInt == 'E')
                map.set(name, std::atof(&*it));
              else
                map.set(name, std::atoi(&*it));

              while(*it == '-' || *it == '.' || (*it >= '0' && *it <= '9'))
                ++it;
              ++it;

              state = Parse_Name;
            } break;

            /* Start of null, true, or false. */
            case 'n':
            case 't':
            case 'f':
            {
              if(*it == 'n' && *(it + 1) == 'u' && *(it + 2) == 'l' && *(it + 3) == 'l')
                map.set(name, typename Map::value_t());
              else if(*it == 't' && *(it + 1) == 'r' && *(it + 2) == 'u' && *(it + 3) == 'e')
                map.set(name, true);
              else
                map.set(name, false);

              while(*it != ',' && *it != JsonMap::delimClose)
                ++it;
              ++it;

              state = Parse_Name;
            } break;

            default:
              ++it;
              break;
          }
        }
      }

      template <typename Array>
      static void parseArray(Array &arr, str_citer &it)
      {

      }

    public:

      template <typename Container>
      static Container parseFile(std::string const &jsonFile)
      {
        std::string json;

#if JEAYESON_STD_FSTREAM_LOAD
        std::ifstream file(jsonFile.c_str());
        std::size_t fileSize = 0;

        /* Ensure the file was opened. */
        if(file.is_open() == false)
          throw "Failed to parse non-existent file.";

        /* Determine the file length. */
        file.seekg(0, std::ios_base::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios_base::beg);

        /* Reserve space in the string. */
        json.reserve(fileSize);

        /* Read the file. */
        std::getline(file, json, static_cast<char>(-1)); // EOF
        file.close();
#else
        boost::interprocess::file_mapping file(jsonFile.c_str(), boost::interprocess::read_only);
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
      static Container parse(std::string const &jsonString)
      {
        typedef Map<typename Container::value_t, typename Container::parser_t> JsonMap;
        typedef Array<typename Container::value_t, typename Container::parser_t> JsonArray;

        str_citer it = jsonString.begin();

        while(*it)
        {
          switch(*it)
          {
            case Container::delimOpen:
            {
              ++it;

              Container c;
              if(Container::delimOpen == JsonMap::delimOpen)
                parseMap(c, it);
              else
                parseArray(c, it);
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
      static inline std::string save(Container const &container)
      {
        std::stringstream output;
        output << container;
        return output.str();
      }

  }; /* Class IParseable */
} /* Namespace JeayeSON */

#endif /* JEAYESON_JSONPARSER_H */


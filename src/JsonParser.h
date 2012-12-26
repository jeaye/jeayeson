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

namespace JeayeSON
{
  class Parser
  {
    private:
      typedef std::string::const_iterator str_citer;

      enum State { Parse_Name, Parse_Value };

/********************* Helper Accessors *********************/
      template <typename Value, typename Parser, typename T>
      static inline State add(Map<Value, Parser> &map, std::string const &key, T const &t)
      {
        map.set(key, t);
        return Parse_Name;
      }

      template <typename Value, typename Parser, typename T>
      static inline State add(Array<Value, Parser> &arr, std::string const &notUsed, T const &t)
      {
        arr.add(t);
        return Parse_Value;
      }

      template <typename Value, typename Parser>
      static inline Map<Value, Parser>& getMap(Map<Value, Parser> &map, std::string const &key,
                                              typename Array<Value, Parser>::index_t notUsed)
      { return map.getMap(key); }

      template <typename Value, typename Parser>
      static inline Map<Value, Parser>& getMap(Array<Value, Parser> &arr, std::string const &notUsed,
                                              typename Array<Value, Parser>::index_t index)
      { return arr.getMap(index); }

      template <typename Value, typename Parser>
      static inline Array<Value, Parser>& getArray(Map<Value, Parser> &map, std::string const &key,
                                                  typename Array<Value, Parser>::index_t notUsed)
      { return map.getArray(key); }

      template <typename Value, typename Parser>
      static inline Array<Value, Parser>& getArray(Array<Value, Parser> &arr, std::string const &notUsed,
                                                  typename Array<Value, Parser>::index_t index)
      { return arr.getArray(index); }
/********************* Helper Accessors *********************/

      template <typename Container>
      static void parse(Container &container, str_citer &it)
      {
        typedef Map<typename Container::value_t, typename Container::parser_t> JsonMap;
        typedef Array<typename Container::value_t, typename Container::parser_t> JsonArray;

        std::string name;
        name.reserve(128);
        std::string value;
        value.reserve(128);

        /* Maps start out parsing keys, arrays just want values. */
        State state(container.delimOpen == JsonMap::delimOpen ? Parse_Name : Parse_Value);

        while(*it)
        {
          switch(*it)
          {
            /* Start of a new map. */
            case JsonMap::delimOpen:
            {
              ++it;

              state = add(container, name, JsonMap());
              parse(getMap(container, name, container.getSize() - 1), it);
            } break;

            /* Start of a new array. */
            case JsonArray::delimOpen:
            {
              ++it;

              state = add(container, name, JsonArray());
              parse(getArray(container, name, container.getSize() - 1), it);
            } break;

            /* End of the current node. */
            case JsonMap::delimClose:
            case JsonArray::delimClose:
              ++it;
              return;

            /* Start of a value (the key). */
            case '"':
            {
              ++it;

              if(state == Parse_Value)
              {
                value.clear();
                for( ; *it != '"'; ++it)
                  value += *it;

                state = add(container, name, value);
              }
              else // Parsing a key/name
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
              /* Determine if the value is integral or floating point. */
              str_citer isInt(it);
              while(*isInt == '-' || (*isInt >= '0' && *isInt <= '9'))
                ++isInt;
              if(*isInt == '.' || *isInt == 'e' || *isInt == 'E')
                state = add(container, name, std::atof(&*it));
              else
                state = add(container, name, std::atoi(&*it));

              /* Progress to the next element. */
              while(*it == '-' || *it == '.' || (*it >= '0' && *it <= '9'))
                ++it;
              ++it;
            } break;

            /* Start of null, true, or false. */
            case 'n':
            case 't':
            case 'f':
            {
              if(*it == 'n' && *(it + 1) == 'u' && *(it + 2) == 'l' && *(it + 3) == 'l')
                state = add(container, name, typename Container::value_t());
              else if(*it == 't' && *(it + 1) == 'r' && *(it + 2) == 'u' && *(it + 3) == 'e')
                state = add(container, name, true);
              else
                state = add(container, name, false);

              /* Progress to the next element. */
              while(*it != ',' && *it != JsonMap::delimClose)
                ++it;
              ++it;
            } break;

            /* Whitespace or unimportant characters. */
            default:
              ++it;
              break;
          }
        }
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
      static inline std::string save(Container const &container)
      {
        std::stringstream output;
        output << container;
        return output.str();
      }

  }; /* Class IParseable */
} /* Namespace JeayeSON */

#endif /* JEAYESON_JSONPARSER_H */


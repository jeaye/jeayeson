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
            /* Start of a new map/array. */
            case JsonMap::delimOpen:
              {
                ++it;

              } break;

            case JsonArray::delimOpen:
              {
                ++it;

              } break;

              /* Whitespace. */
            default:
              {
                std::cout << *it;
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


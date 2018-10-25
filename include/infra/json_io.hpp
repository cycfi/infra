/*=============================================================================
   Copyright (c) 2016-2018 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(INFRA_JSON_IO_DECEMBER_18_2017)
#define INFRA_JSON_IO_DECEMBER_18_2017

#include <infra/json.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/optional.hpp>

namespace cycfi { namespace json
{
   namespace fs = boost::filesystem;

   template <typename T>
   boost::optional<T> load(fs::path path)
   {
      if (fs::exists(path))
      {
         fs::ifstream file(path);
         std::string src(
            (std::istreambuf_iterator<char>(file))
         , std::istreambuf_iterator<char>());
         T data;
         auto f = src.begin();
         auto l = src.end();
         if (x3::phrase_parse(f, l, parser{}, x3::space, data))
            return { std::move(data) };
      }
      return {};
   }

   template <typename T>
   void save(fs::path path, T const& attr)
   {
      fs::ofstream file(path);
      std::string json;
      printer pr(file);
      pr.print(attr);
   }
}}

#endif

/*=============================================================================
   Copyright (c) 2016-2018 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(INFRA_EXCEPTION_OCTOBER_3_2016)
#define INFRA_EXCEPTION_OCTOBER_3_2016

#include <stdexcept>

namespace cycfi
{
   struct exception : std::runtime_error
   {
                     exception()
                       : std::runtime_error("")
                     {}

      explicit       exception(std::string const& what)
                       : std::runtime_error(what)
                     {}
   };
}

#endif

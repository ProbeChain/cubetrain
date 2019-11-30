#pragma once

#include <cubetrainlib/cubetrain.hpp>

namespace cubetrain {

   class sudo : public contract {
      public:
         sudo( account_name self ):contract(self){}

         void exec();

   };

} /// namespace cubetrain

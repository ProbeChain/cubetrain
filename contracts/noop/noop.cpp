/**
 *  @file
 *  @copyright defined in seat/LICENSE.txt
 */

#include <cubetrainlib/cubetrain.hpp>

namespace cubetrain {

   class noop: public contract {
      public:
         noop( account_name self ): contract( self ) { }
         void anyaction( account_name from,
                         const std::string& /*type*/,
                         const std::string& /*data*/ )
         {
            require_auth( from );
         }
   };

   CUBETRAIN_ABI( noop, ( anyaction ) )

} /// cubetrain     

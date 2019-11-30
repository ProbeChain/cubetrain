#include <cubetrainlib/cubetrain.hpp>

using namespace cubetrain;

class hello : public cubetrain::contract {
  public:
      using contract::contract;

      /// @abi action 
      void hi( account_name user ) {
         print( "Hello, ", name{user} );
      }
};

CUBETRAIN_ABI( hello, (hi) )

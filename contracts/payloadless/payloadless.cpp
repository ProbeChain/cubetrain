#include <cubetrainlib/cubetrain.hpp>
#include <cubetrainlib/print.hpp>
using namespace cubetrain;

class payloadless : public cubetrain::contract {
  public:
      using contract::contract;

      void doit() {
         print( "Im a payloadless action" );
      }
};

CUBETRAIN_ABI( payloadless, (doit) )

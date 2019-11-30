/**
 *  @file
 *  @copyright defined in seat/LICENSE.txt
 */

#include <cubetrainlib/cubetrain.hpp>

namespace asserter {
   struct assertdef {
      int8_t      condition;
      std::string message;

      SEATLIB_SERIALIZE( assertdef, (condition)(message) )
   };
}

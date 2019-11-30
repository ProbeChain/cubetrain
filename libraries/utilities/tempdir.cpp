/**
 *  @file
 *  @copyright defined in seat/LICENSE.txt
 */

#include <cubetrain/utilities/tempdir.hpp>

#include <cstdlib>

namespace cubetrain { namespace utilities {

fc::path temp_directory_path()
{
   const char* seat_tempdir = getenv("SEAT_TEMPDIR");
   if( seat_tempdir != nullptr )
      return fc::path( seat_tempdir );
   return fc::temp_directory_path() / "seat-tmp";
}

} } // cubetrain::utilities

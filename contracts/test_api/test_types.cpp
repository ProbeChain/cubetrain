/**
 *  @file
 *  @copyright defined in seat/LICENSE.txt
 */
#include <cubetrainlib/cubetrain.hpp>

#include "test_api.hpp"

void test_types::types_size() {

   cubetrain_assert( sizeof(int64_t) == 8, "int64_t size != 8");
   cubetrain_assert( sizeof(uint64_t) ==  8, "uint64_t size != 8");
   cubetrain_assert( sizeof(uint32_t) ==  4, "uint32_t size != 4");
   cubetrain_assert( sizeof(int32_t) ==  4, "int32_t size != 4");
   cubetrain_assert( sizeof(uint128_t) == 16, "uint128_t size != 16");
   cubetrain_assert( sizeof(int128_t) == 16, "int128_t size != 16");
   cubetrain_assert( sizeof(uint8_t) ==  1, "uint8_t size != 1");

   cubetrain_assert( sizeof(account_name) ==  8, "account_name size !=  8");
   cubetrain_assert( sizeof(table_name) ==  8, "table_name size !=  8");
   cubetrain_assert( sizeof(time) ==  4, "time size !=  4");
   cubetrain_assert( sizeof(cubetrain::key256) == 32, "key256 size != 32" );
}

void test_types::char_to_symbol() {

   cubetrain_assert( cubetrain::char_to_symbol('1') ==  1, "cubetrain::char_to_symbol('1') !=  1");
   cubetrain_assert( cubetrain::char_to_symbol('2') ==  2, "cubetrain::char_to_symbol('2') !=  2");
   cubetrain_assert( cubetrain::char_to_symbol('3') ==  3, "cubetrain::char_to_symbol('3') !=  3");
   cubetrain_assert( cubetrain::char_to_symbol('4') ==  4, "cubetrain::char_to_symbol('4') !=  4");
   cubetrain_assert( cubetrain::char_to_symbol('5') ==  5, "cubetrain::char_to_symbol('5') !=  5");
   cubetrain_assert( cubetrain::char_to_symbol('a') ==  6, "cubetrain::char_to_symbol('a') !=  6");
   cubetrain_assert( cubetrain::char_to_symbol('b') ==  7, "cubetrain::char_to_symbol('b') !=  7");
   cubetrain_assert( cubetrain::char_to_symbol('c') ==  8, "cubetrain::char_to_symbol('c') !=  8");
   cubetrain_assert( cubetrain::char_to_symbol('d') ==  9, "cubetrain::char_to_symbol('d') !=  9");
   cubetrain_assert( cubetrain::char_to_symbol('e') == 10, "cubetrain::char_to_symbol('e') != 10");
   cubetrain_assert( cubetrain::char_to_symbol('f') == 11, "cubetrain::char_to_symbol('f') != 11");
   cubetrain_assert( cubetrain::char_to_symbol('g') == 12, "cubetrain::char_to_symbol('g') != 12");
   cubetrain_assert( cubetrain::char_to_symbol('h') == 13, "cubetrain::char_to_symbol('h') != 13");
   cubetrain_assert( cubetrain::char_to_symbol('i') == 14, "cubetrain::char_to_symbol('i') != 14");
   cubetrain_assert( cubetrain::char_to_symbol('j') == 15, "cubetrain::char_to_symbol('j') != 15");
   cubetrain_assert( cubetrain::char_to_symbol('k') == 16, "cubetrain::char_to_symbol('k') != 16");
   cubetrain_assert( cubetrain::char_to_symbol('l') == 17, "cubetrain::char_to_symbol('l') != 17");
   cubetrain_assert( cubetrain::char_to_symbol('m') == 18, "cubetrain::char_to_symbol('m') != 18");
   cubetrain_assert( cubetrain::char_to_symbol('n') == 19, "cubetrain::char_to_symbol('n') != 19");
   cubetrain_assert( cubetrain::char_to_symbol('o') == 20, "cubetrain::char_to_symbol('o') != 20");
   cubetrain_assert( cubetrain::char_to_symbol('p') == 21, "cubetrain::char_to_symbol('p') != 21");
   cubetrain_assert( cubetrain::char_to_symbol('q') == 22, "cubetrain::char_to_symbol('q') != 22");
   cubetrain_assert( cubetrain::char_to_symbol('r') == 23, "cubetrain::char_to_symbol('r') != 23");
   cubetrain_assert( cubetrain::char_to_symbol('s') == 24, "cubetrain::char_to_symbol('s') != 24");
   cubetrain_assert( cubetrain::char_to_symbol('t') == 25, "cubetrain::char_to_symbol('t') != 25");
   cubetrain_assert( cubetrain::char_to_symbol('u') == 26, "cubetrain::char_to_symbol('u') != 26");
   cubetrain_assert( cubetrain::char_to_symbol('v') == 27, "cubetrain::char_to_symbol('v') != 27");
   cubetrain_assert( cubetrain::char_to_symbol('w') == 28, "cubetrain::char_to_symbol('w') != 28");
   cubetrain_assert( cubetrain::char_to_symbol('x') == 29, "cubetrain::char_to_symbol('x') != 29");
   cubetrain_assert( cubetrain::char_to_symbol('y') == 30, "cubetrain::char_to_symbol('y') != 30");
   cubetrain_assert( cubetrain::char_to_symbol('z') == 31, "cubetrain::char_to_symbol('z') != 31");

   for(unsigned char i = 0; i<255; i++) {
      if((i >= 'a' && i <= 'z') || (i >= '1' || i <= '5')) continue;
      cubetrain_assert( cubetrain::char_to_symbol((char)i) == 0, "cubetrain::char_to_symbol() != 0");
   }
}

void test_types::string_to_name() {

   cubetrain_assert( cubetrain::string_to_name("a") == N(a) , "cubetrain::string_to_name(a)" );
   cubetrain_assert( cubetrain::string_to_name("ba") == N(ba) , "cubetrain::string_to_name(ba)" );
   cubetrain_assert( cubetrain::string_to_name("cba") == N(cba) , "cubetrain::string_to_name(cba)" );
   cubetrain_assert( cubetrain::string_to_name("dcba") == N(dcba) , "cubetrain::string_to_name(dcba)" );
   cubetrain_assert( cubetrain::string_to_name("edcba") == N(edcba) , "cubetrain::string_to_name(edcba)" );
   cubetrain_assert( cubetrain::string_to_name("fedcba") == N(fedcba) , "cubetrain::string_to_name(fedcba)" );
   cubetrain_assert( cubetrain::string_to_name("gfedcba") == N(gfedcba) , "cubetrain::string_to_name(gfedcba)" );
   cubetrain_assert( cubetrain::string_to_name("hgfedcba") == N(hgfedcba) , "cubetrain::string_to_name(hgfedcba)" );
   cubetrain_assert( cubetrain::string_to_name("ihgfedcba") == N(ihgfedcba) , "cubetrain::string_to_name(ihgfedcba)" );
   cubetrain_assert( cubetrain::string_to_name("jihgfedcba") == N(jihgfedcba) , "cubetrain::string_to_name(jihgfedcba)" );
   cubetrain_assert( cubetrain::string_to_name("kjihgfedcba") == N(kjihgfedcba) , "cubetrain::string_to_name(kjihgfedcba)" );
   cubetrain_assert( cubetrain::string_to_name("lkjihgfedcba") == N(lkjihgfedcba) , "cubetrain::string_to_name(lkjihgfedcba)" );
   cubetrain_assert( cubetrain::string_to_name("mlkjihgfedcba") == N(mlkjihgfedcba) , "cubetrain::string_to_name(mlkjihgfedcba)" );
   cubetrain_assert( cubetrain::string_to_name("mlkjihgfedcba1") == N(mlkjihgfedcba2) , "cubetrain::string_to_name(mlkjihgfedcba2)" );
   cubetrain_assert( cubetrain::string_to_name("mlkjihgfedcba55") == N(mlkjihgfedcba14) , "cubetrain::string_to_name(mlkjihgfedcba14)" );

   cubetrain_assert( cubetrain::string_to_name("azAA34") == N(azBB34) , "cubetrain::string_to_name N(azBB34)" );
   cubetrain_assert( cubetrain::string_to_name("AZaz12Bc34") == N(AZaz12Bc34) , "cubetrain::string_to_name AZaz12Bc34" );
   cubetrain_assert( cubetrain::string_to_name("AAAAAAAAAAAAAAA") == cubetrain::string_to_name("BBBBBBBBBBBBBDDDDDFFFGG") , "cubetrain::string_to_name BBBBBBBBBBBBBDDDDDFFFGG" );
}

void test_types::name_class() {

   cubetrain_assert( cubetrain::name{cubetrain::string_to_name("azAA34")}.value == N(azAA34), "cubetrain::name != N(azAA34)" );
   cubetrain_assert( cubetrain::name{cubetrain::string_to_name("AABBCC")}.value == 0, "cubetrain::name != N(0)" );
   cubetrain_assert( cubetrain::name{cubetrain::string_to_name("AA11")}.value == N(AA11), "cubetrain::name != N(AA11)" );
   cubetrain_assert( cubetrain::name{cubetrain::string_to_name("11AA")}.value == N(11), "cubetrain::name != N(11)" );
   cubetrain_assert( cubetrain::name{cubetrain::string_to_name("22BBCCXXAA")}.value == N(22), "cubetrain::name != N(22)" );
   cubetrain_assert( cubetrain::name{cubetrain::string_to_name("AAAbbcccdd")} == cubetrain::name{cubetrain::string_to_name("AAAbbcccdd")}, "cubetrain::name == cubetrain::name" );

   uint64_t tmp = cubetrain::name{cubetrain::string_to_name("11bbcccdd")};
   cubetrain_assert(N(11bbcccdd) == tmp, "N(11bbcccdd) == tmp");
}

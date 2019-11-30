/**
 *  @file
 *  @copyright defined in seat/LICENSE.txt
 */
#pragma once

#include <cubetrainlib/cubetrain.hpp>
#include <cubetrainlib/token.hpp>
#include <cubetrainlib/reflect.hpp>
#include <cubetrainlib/generic_currency.hpp>

#include <bancor/converter.hpp>
#include <currency/currency.hpp>

namespace bancor {
   typedef cubetrain::generic_currency< cubetrain::token<N(other),S(4,OTHER)> >  other_currency;
   typedef cubetrain::generic_currency< cubetrain::token<N(bancor),S(4,RELAY)> > relay_currency;
   typedef cubetrain::generic_currency< cubetrain::token<N(currency),S(4,CUR)> > cur_currency;

   typedef converter<relay_currency, other_currency, cur_currency > example_converter;
} /// bancor


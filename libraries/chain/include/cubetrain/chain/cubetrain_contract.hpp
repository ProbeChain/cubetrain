/**
 *  @file
 *  @copyright defined in seat/LICENSE.txt
 */
#pragma once

#include <cubetrain/chain/types.hpp>
#include <cubetrain/chain/contract_types.hpp>

namespace cubetrain { namespace chain {

   class apply_context;

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_cubetrain_newaccount(apply_context&);
   void apply_cubetrain_updateauth(apply_context&);
   void apply_cubetrain_deleteauth(apply_context&);
   void apply_cubetrain_linkauth(apply_context&);
   void apply_cubetrain_unlinkauth(apply_context&);

   /*
   void apply_cubetrain_postrecovery(apply_context&);
   void apply_cubetrain_passrecovery(apply_context&);
   void apply_cubetrain_vetorecovery(apply_context&);
   */

   void apply_cubetrain_setcode(apply_context&);
   void apply_cubetrain_setabi(apply_context&);

   void apply_cubetrain_canceldelay(apply_context&);
   ///@}  end action handlers

} } /// namespace cubetrain::chain

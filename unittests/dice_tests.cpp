#include <boost/test/unit_test.hpp>
#include <cubetrain/testing/tester.hpp>
#include <cubetrain/chain/abi_serializer.hpp>
#include <cubetrain/chain/contract_table_objects.hpp>

#include <dice/dice.wast.hpp>
#include <dice/dice.abi.hpp>

#include <cubetrain.tk/cubetrain.tk.wast.hpp>
#include <cubetrain.tk/cubetrain.tk.abi.hpp>

#include <Runtime/Runtime.h>

#include <fc/variant_object.hpp>

#ifdef NON_VALIDATING_TEST
#define TESTER tester
#else
#define TESTER validating_tester
#endif

using namespace cubetrain;
using namespace cubetrain::chain;
using namespace cubetrain::testing;
using namespace fc;
using namespace std;
using mvo = fc::mutable_variant_object;

struct offer_bet_t {
   asset             bet;
   account_name      player;
   checksum256_type  commitment;

   static account_name get_account() { return N(dice); }
   static action_name get_name() {return N(offerbet); }
};
FC_REFLECT(offer_bet_t, (bet)(player)(commitment));

struct cancel_offer_t {
   checksum256_type commitment;

   static account_name get_account() { return N(dice); }
   static action_name get_name() {return N(canceloffer); }
};
FC_REFLECT(cancel_offer_t, (commitment));

struct reveal_t {
   checksum256_type commitment;
   checksum256_type source;

   static account_name get_account() { return N(dice); }
   static action_name get_name() {return N(reveal); }
};
FC_REFLECT(reveal_t, (commitment)(source));

struct deposit_t {
   account_name from;
   asset        amount;

   static account_name get_account() { return N(dice); }
   static action_name get_name() {return N(deposit); }
};
FC_REFLECT( deposit_t, (from)(amount) );

struct withdraw_t {
   account_name to;
   asset        amount;

   static account_name get_account() { return N(dice); }
   static action_name get_name() {return N(withdraw); }
};
FC_REFLECT( withdraw_t, (to)(amount) );

struct __attribute((packed)) account_t {
   account_name owner;
   asset        seat_balance;
   uint32_t     open_offers;
   uint32_t     open_games;
};
FC_REFLECT(account_t, (owner)(seat_balance)(open_offers)(open_games));

struct player_t {
   checksum_type commitment;
   checksum_type reveal;
};
FC_REFLECT(player_t, (commitment)(reveal));

struct __attribute((packed)) game_t {
   uint64_t             gameid;
   asset                bet;
   fc::time_point_sec   deadline;
   player_t             player1;
   player_t             player2;
};
FC_REFLECT(game_t, (gameid)(bet)(deadline)(player1)(player2));

struct dice_tester : TESTER {

   template<typename IndexType, typename Scope>
   const auto& get_index() {
      return control->db().get_index<IndexType,Scope>();
   }

   void offer_bet(account_name account, asset amount, const checksum_type& commitment) {
      signed_transaction trx;
      action act( {{account, config::active_name}},
         offer_bet_t{amount, account, commitment} );
      trx.actions.push_back(act);
      set_transaction_headers(trx);
      trx.sign(get_private_key( account, "active" ), control->get_chain_id());
      auto ptrx = packed_transaction(trx,packed_transaction::none);
      push_transaction(ptrx);
   }

   void cancel_offer(account_name account, const checksum_type& commitment) {
      signed_transaction trx;
      action act( {{account, config::active_name}},
         cancel_offer_t{commitment} );
      trx.actions.push_back(act);
      set_transaction_headers(trx);
      trx.sign(get_private_key( account, "active" ), control->get_chain_id());
      auto ptrx = packed_transaction(trx,packed_transaction::none);
      push_transaction(ptrx);
   }

   void deposit(account_name account, asset amount) {
      signed_transaction trx;
      action act( {{account, config::active_name}},
         deposit_t{account, amount} );
      trx.actions.push_back(act);
      set_transaction_headers(trx);
      trx.sign(get_private_key( account, "active" ), control->get_chain_id());
      auto ptrx = packed_transaction(trx,packed_transaction::none);
      push_transaction(ptrx);
   }

   void withdraw(account_name account, asset amount) {
      signed_transaction trx;
      action act( {{account, config::active_name}},
         withdraw_t{account, amount} );
      trx.actions.push_back(act);
      set_transaction_headers(trx);
      trx.sign(get_private_key( account, "active" ), control->get_chain_id());
      auto ptrx = packed_transaction(trx,packed_transaction::none);
      push_transaction(ptrx);
   }

   void reveal(account_name account, const checksum_type& commitment, const checksum_type& source ) {
      signed_transaction trx;
      action act( {{account, config::active_name}},
         reveal_t{commitment, source} );
      trx.actions.push_back(act);
      set_transaction_headers(trx);
      trx.sign(get_private_key( account, "active" ), control->get_chain_id());
      auto ptrx = packed_transaction(trx,packed_transaction::none);
      push_transaction(ptrx);
   }

   bool dice_account(account_name account, account_t& acnt) {
      auto* maybe_tid = find_table(N(dice), N(dice), N(account));
      if(maybe_tid == nullptr) return false;

      auto* o = control->db().find<key_value_object, by_scope_primary>(boost::make_tuple(maybe_tid->id, account));
      if(o == nullptr) {
         return false;
      }

      fc::raw::unpack(o->value.data(), o->value.size(), acnt);
      return true;
   }

   bool dice_game(uint64_t game_id, game_t& game) {
      const bool not_required = false;
      return get_table_entry(game, N(dice), N(dice), N(game), game_id, not_required);
   }

   uint32_t open_games(account_name account) {
      account_t acnt;
      if(!dice_account(account, acnt)) return 0;
      return acnt.open_games;
   }

   asset game_bet(uint64_t game_id) {
      game_t game;
      if(!dice_game(game_id, game)) return asset();
      return game.bet;
   }

   uint32_t open_offers(account_name account) {
      account_t acnt;
      if(!dice_account(account, acnt)) return 0;
      return acnt.open_offers;
   }

   asset balance_of(account_name account) {
      account_t acnt;
      if(!dice_account(account, acnt)) return asset();
      return acnt.seat_balance;
   }

   checksum_type commitment_for( const char* secret ) {
      return commitment_for(checksum_type(secret));
   }

   checksum_type commitment_for( const checksum_type& secret ) {
      return fc::sha256::hash( secret.data(), sizeof(secret) );
   }

   void add_dice_authority(account_name account) {
      auto auth = authority{
         1,
         {
            {.key = get_public_key(account,"active"), .weight = 1}
         },
         {
            {.permission = {N(dice),N(active)}, .weight = 1}
         }
      };
      set_authority(account, N(active), auth, N(owner) );
   }
};

BOOST_AUTO_TEST_SUITE(dice_tests)

BOOST_FIXTURE_TEST_CASE( dice_test, dice_tester ) try {

   create_accounts( {N(cubetrain.tk), N(dice),N(alice),N(bob),N(carol),N(david)}, false);

   set_code(N(cubetrain.tk), cubetrain_tk_wast);
   set_abi(N(cubetrain.tk), cubetrain_tk_abi);

   produce_block();

   add_dice_authority(N(alice));
   add_dice_authority(N(bob));
   add_dice_authority(N(carol));

   push_action(N(cubetrain.tk), N(create), N(cubetrain.tk), mvo()
     ("issuer", "cubetrain.tk")
     ("maximum_supply", core_from_string("1000000000.0000"))
   );

   push_action(N(cubetrain.tk), N(issue), N(cubetrain.tk), mvo()
     ("to", "cubetrain")
     ("quantity", core_from_string("1000000000.0000"))
     ("memo", "")
   );

   transfer( config::system_account_name, N(alice), core_from_string("10000.0000"), "", N(cubetrain.tk) );
   transfer( config::system_account_name, N(bob),   core_from_string("10000.0000"), "", N(cubetrain.tk) );
   transfer( config::system_account_name, N(carol), core_from_string("10000.0000"), "", N(cubetrain.tk) );

   produce_block();

   set_code(N(dice), dice_wast);
   set_abi(N(dice), dice_abi);

   produce_block();

   // Alice deposits 1000
   deposit( N(alice), core_from_string("1000.0000"));
   produce_block();

   BOOST_REQUIRE_EQUAL( balance_of(N(alice)), core_from_string("1000.0000"));
   BOOST_REQUIRE_EQUAL( open_games(N(alice)), 0);

   // Alice tries to bet 0 (fail)
   // secret : 9b886346e1351d4144d0b8392a975612eb0f8b6de7eae1cc9bcc55eb52be343c
   BOOST_CHECK_THROW( offer_bet( N(alice), core_from_string("0.0000"),
      commitment_for("9b886346e1351d4144d0b8392a975612eb0f8b6de7eae1cc9bcc55eb52be343c")
   ), fc::exception);

   // Alice bets 10 (success)
   // secret : 0ba044d2833758ee2c8f24d8a3f70c82c334abe6ce13219a4cf3b862abb03c46
   offer_bet( N(alice), core_from_string("10.0000"),
      commitment_for("0ba044d2833758ee2c8f24d8a3f70c82c334abe6ce13219a4cf3b862abb03c46")
   );
   produce_block();

   // Bob tries to bet using a secret previously used by Alice (fail)
   // secret : 00000000000000000000000000000002c334abe6ce13219a4cf3b862abb03c46
   BOOST_CHECK_THROW( offer_bet( N(bob), core_from_string("10.0000"),
      commitment_for("0ba044d2833758ee2c8f24d8a3f70c82c334abe6ce13219a4cf3b862abb03c46")
   ), fc::exception);
   produce_block();

   // Alice tries to bet 1000 (fail)
   // secret : a512f6b1b589a8906d574e9de74a529e504a5c53a760f0991a3e00256c027971
   BOOST_CHECK_THROW( offer_bet( N(alice), core_from_string("1000.0000"),
      commitment_for("a512f6b1b589a8906d574e9de74a529e504a5c53a760f0991a3e00256c027971")
   ), fc::exception);
   produce_block();

   // Bob tries to bet 90 without deposit
   // secret : 4facfc98932dde46fdc4403125a16337f6879a842a7ff8b0dc8e1ecddd59f3c8
   BOOST_CHECK_THROW( offer_bet( N(bob), core_from_string("90.0000"),
      commitment_for("4facfc98932dde46fdc4403125a16337f6879a842a7ff8b0dc8e1ecddd59f3c8")
   ), fc::exception);
   produce_block();

   // Bob deposits 500
   deposit( N(bob), core_from_string("500.0000"));
   BOOST_REQUIRE_EQUAL( balance_of(N(bob)), core_from_string("500.0000"));

   // Bob bets 11 (success)
   // secret : eec3272712d974c474a3e7b4028b53081344a5f50008e9ccf918ba0725a8d784
   offer_bet( N(bob), core_from_string("11.0000"),
      commitment_for("eec3272712d974c474a3e7b4028b53081344a5f50008e9ccf918ba0725a8d784")
   );
   produce_block();

   // Bob cancels (success)
   BOOST_REQUIRE_EQUAL( open_offers(N(bob)), 1);
   cancel_offer( N(bob), commitment_for("eec3272712d974c474a3e7b4028b53081344a5f50008e9ccf918ba0725a8d784") );
   BOOST_REQUIRE_EQUAL( open_offers(N(bob)), 0);

   // Carol deposits 300
   deposit( N(carol), core_from_string("300.0000"));

   // Carol bets 10 (success)
   // secret : 3efb4bd5e19b780f4980c919330c0306f8157f93db1fc72c7cefec63e0e7f37a
   offer_bet( N(carol), core_from_string("10.0000"),
      commitment_for("3efb4bd5e19b780f4980c919330c0306f8157f93db1fc72c7cefec63e0e7f37a")
   );
   produce_block();

   BOOST_REQUIRE_EQUAL( open_games(N(alice)), 1);
   BOOST_REQUIRE_EQUAL( open_offers(N(alice)), 0);

   BOOST_REQUIRE_EQUAL( open_games(N(carol)), 1);
   BOOST_REQUIRE_EQUAL( open_offers(N(carol)), 0);

   BOOST_REQUIRE_EQUAL( game_bet(1), core_from_string("10.0000"));


   // Alice tries to cancel a nonexistent bet (fail)
   BOOST_CHECK_THROW( cancel_offer( N(alice),
      commitment_for("00000000000000000000000000000000000000000000000000000000abb03c46")
   ), fc::exception);

   // Alice tries to cancel an in-game bet (fail)
   BOOST_CHECK_THROW( cancel_offer( N(alice),
      commitment_for("0ba044d2833758ee2c8f24d8a3f70c82c334abe6ce13219a4cf3b862abb03c46")
   ), fc::exception);

   // Alice reveals secret (success)
   reveal( N(alice),
      commitment_for("0ba044d2833758ee2c8f24d8a3f70c82c334abe6ce13219a4cf3b862abb03c46"),
      checksum_type("0ba044d2833758ee2c8f24d8a3f70c82c334abe6ce13219a4cf3b862abb03c46")
   );
   produce_block();

   // Alice tries to reveal again (fail)
   BOOST_CHECK_THROW( reveal( N(alice),
      commitment_for("0ba044d2833758ee2c8f24d8a3f70c82c334abe6ce13219a4cf3b862abb03c46"),
      checksum_type("0ba044d2833758ee2c8f24d8a3f70c82c334abe6ce13219a4cf3b862abb03c46")
   ), fc::exception);

   // Bob tries to reveal an invalid (secret,commitment) pair (fail)
   BOOST_CHECK_THROW( reveal( N(bob),
      commitment_for("121344d2833758ee2c8f24d8a3f70c82c334abe6ce13219a4cf3b862abb03c46"),
      checksum_type("141544d2833758ee2c8f24d8a3f70c82c334abe6ce13219a4cf3b862abb03c46")
   ), fc::exception);

   // Bob tries to reveal a valid (secret,commitment) pair that has no offer/game (fail)
   BOOST_CHECK_THROW( reveal( N(bob),
      commitment_for("e48c6884bb97ac5f5951df6012ce79f63bb8549ad0111315ad9ecbaf4c9b1eb8"),
      checksum_type("e48c6884bb97ac5f5951df6012ce79f63bb8549ad0111315ad9ecbaf4c9b1eb8")
   ), fc::exception);

   // Bob reveals Carol's secret (success)
   reveal( N(bob),
      commitment_for("3efb4bd5e19b780f4980c919330c0306f8157f93db1fc72c7cefec63e0e7f37a"),
      checksum_type("3efb4bd5e19b780f4980c919330c0306f8157f93db1fc72c7cefec63e0e7f37a")
   );

   BOOST_REQUIRE_EQUAL( open_games(N(alice)), 0);
   BOOST_REQUIRE_EQUAL( open_offers(N(alice)), 0);
   BOOST_REQUIRE_EQUAL( balance_of(N(alice)), core_from_string("1010.0000"));

   BOOST_REQUIRE_EQUAL( open_games(N(carol)), 0);
   BOOST_REQUIRE_EQUAL( open_offers(N(carol)), 0);
   BOOST_REQUIRE_EQUAL( balance_of(N(carol)), core_from_string("290.0000"));

   // Alice withdraw 1009 (success)
   withdraw( N(alice), core_from_string("1009.0000"));
   BOOST_REQUIRE_EQUAL( balance_of(N(alice)), core_from_string("1.0000"));

   BOOST_REQUIRE_EQUAL(
      get_currency_balance(N(cubetrain.tk), symbol(CORE_SYMBOL), N(alice)),
      core_from_string("10009.0000")
   );

   // Alice withdraw 2 (fail)
   BOOST_CHECK_THROW( withdraw( N(alice), core_from_string("2.0000")),
      fc::exception);

   // Alice withdraw 1 (success)
   withdraw( N(alice), core_from_string("1.0000"));

   BOOST_REQUIRE_EQUAL(
      get_currency_balance(N(cubetrain.tk), symbol(CORE_SYMBOL), N(alice)),
      core_from_string("10010.0000")
   );

   // Verify alice account was deleted
   account_t alice_account;
   BOOST_CHECK(dice_account(N(alice), alice_account) == false);

   // No games in table
   auto* game_tid = find_table(N(dice), N(dice), N(game));
   BOOST_CHECK(game_tid == nullptr);

   // No offers in table
   auto* offer_tid = find_table(N(dice), N(dice), N(offer));
   BOOST_CHECK(offer_tid == nullptr);

   // 2 records in account table (Bob & Carol)
   auto* account_tid = find_table(N(dice), N(dice), N(account));
   BOOST_CHECK(account_tid != nullptr);
   BOOST_CHECK(account_tid->count == 2);

} FC_LOG_AND_RETHROW() /// basic_test

BOOST_AUTO_TEST_SUITE_END()

/**
 *  @file
 *  @copyright defined in seat/LICENSE.txt
 */
#include <utility>
#include <vector>
#include <string>
#include <cubetrainlib/cubetrain.hpp>
#include <cubetrainlib/contract.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wsign-compare"

class test_ram_limit : public cubetrain::contract {
   public:
      const uint32_t FIVE_MINUTES = 5*60;

      test_ram_limit(account_name self)
      :cubetrain::contract(self)
      {}

      //@abi action
      void setentry(account_name payer, uint64_t from, uint64_t to, uint64_t size) {
         const auto self = get_self();
         cubetrain::print("test_ram_limit::setentry ", cubetrain::name{self}, "\n");
         test_table table(self, self);
         for (int key = from; key <=to; ++key) {
            auto itr = table.find(key);
            if (itr != table.end()) {
               table.modify(itr, payer, [size](test& t) {
                  t.data.assign(size, (int8_t)size);
               });
            } else {
               table.emplace(payer, [key,size](test& t) {
                  t.key = key;
                  t.data.assign(size, (int8_t)size);
               });
            }
         }
      }

      //@abi action
      void rmentry(uint64_t from, uint64_t to) {
         const auto self = get_self();
         cubetrain::print("test_ram_limit::rmentry ", cubetrain::name{self}, "\n");
         test_table table(self, self);
         for (int key = from; key <=to; ++key) {
            auto itr = table.find(key);
            cubetrain_assert (itr != table.end(), "could not find test_table entry");
            table.erase(itr);
         }
      }

      //@abi action
      void printentry(uint64_t from, uint64_t to) {
         const auto self = get_self();
         cubetrain::print("test_ram_limit::printout ", cubetrain::name{self}, ":");
         test_table table(self, self);
         for (int key = from; key <=to; ++key) {
            auto itr = table.find(key);
            cubetrain::print("\nkey=", key);
            cubetrain_assert (itr != table.end(), "could not find test_table entry");
            cubetrain::print(" size=", itr->data.size());
         }
      }

   private:
      struct test {
         uint64_t            key;
         std::vector<int8_t> data;

         uint64_t primary_key()const { return key; }

         SEATLIB_SERIALIZE( test, (key)(data) )
      };
      typedef cubetrain::multi_index< N(test.table), test> test_table;
};

#pragma clang diagnostic pop

CUBETRAIN_ABI( test_ram_limit, (setentry)(rmentry)(printentry) )

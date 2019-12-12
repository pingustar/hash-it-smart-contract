#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract]] hashhashhash : public eosio::contract {

  private:

    struct [[eosio::table]] hash_info {
      uint64_t        id;
      name            username;
      string          content;
      checksum256     hash;
      uint64_t        hashInt;
      uint64_t        item_count;
      time_point_sec  created_at;

      auto primary_key() const { return id; }
      checksum256 by_hash() const { return hash; }
    };

    struct [[eosio::table]] item_info {
      uint64_t        id;
      checksum256     hash;
      name            username;
      string          content;
      time_point_sec  created_at;

      auto primary_key() const { return id; }
    };

    typedef eosio::multi_index<name("hashes"), hash_info, indexed_by<name("byhash"), const_mem_fun<hash_info, checksum256, &hash_info::by_hash>>> hashes_table;
    typedef eosio::multi_index<name("items"), item_info> items_table;

    hashes_table _hashes;

    uint64_t checksum_to_uint64( eosio::checksum256 hash ) {
      uint128_t * data = hash.data();

      return ((uint64_t)data[0] << 56) +
        ((uint64_t)data[1] << 48) +
        ((uint64_t)data[2] << 40) +
        ((uint64_t)data[3] << 32) +
        ((uint64_t)data[4] << 24) +
        ((uint64_t)data[5] << 16) +
        ((uint64_t)data[6] << 8) +
        ((uint64_t)data[7] << 4);
    }

  public:

    hashhashhash( name receiver, name code, datastream<const char*> ds ):contract(receiver, code, ds), _hashes(receiver, receiver.value) {}

    [[eosio::action]]
    void submit(name username, checksum256 hash, string content);

    [[eosio::action]]
    void clear();

};

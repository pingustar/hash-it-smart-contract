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
      uint64_t        hash;
      uint64_t        item_count;
      time_point_sec  created_at;

      auto primary_key() const { return id; }
      uint64_t by_hash() const { return hash; }
    };

    struct [[eosio::table]] item_info {
      uint64_t        id;
      uint64_t        hash;
      name            username;
      string          content;
      time_point_sec  created_at;

      auto primary_key() const { return id; }
    };

    typedef eosio::multi_index<name("hashes"), hash_info, indexed_by<name("byhash"), const_mem_fun<hash_info, uint64_t, &hash_info::by_hash>>> hashes_table;
    typedef eosio::multi_index<name("items"), item_info> items_table;

    hashes_table _hashes;

  public:

    hashhashhash( name receiver, name code, datastream<const char*> ds ):contract(receiver, code, ds), _hashes(receiver, receiver.value) {}

    [[eosio::action]]
    void submit(name username, uint64_t hash, string content);

    [[eosio::action]]
    void clear();

};

#include <hashhashhash.hpp>

void hashhashhash::submit(name username, checksum256 hash, string content) {
  // Ensure this action is authorized by the user
  require_auth(username);

  // Ensure the hash is valid
  // eosio::check(hash.length() == 64, "The hash has to be exactly 64 characters.");

  auto hashes = _hashes.get_index<name("byhash")>();

  // Check if hash exists, add to hashes table if it doesnt
  auto h_itr = hashes.find(hash);

  // Get item count of hash
  uint64_t count = h_itr->item_count;

  // Convert checksum to uint64
  uint64_t hashInt = hashhashhash::checksum_to_uint64(hash);
  
  // Create or Update hash in hashes table
  if (h_itr == hashes.end()) {
    _hashes.emplace(get_self(),  [&](auto& new_hash) {
      new_hash.id = _hashes.available_primary_key();
      new_hash.hashInt = hashInt;
      new_hash.hash = hash;
      new_hash.content = content;
      new_hash.username = username;
      new_hash.item_count = 1;
      new_hash.created_at = time_point_sec(current_time_point());
    });
  } else{
    hashes.modify(h_itr, get_self(), [&]( auto& h_row ) {
      h_row.item_count = count + 1;
    });
  }

  // Init items table
  items_table _items(get_self(), hashInt);
  
  // add content to items table
  _items.emplace(get_self(),  [&](auto& new_item) {
    new_item.id = count;
    new_item.hash = hash;
    new_item.username = username;
    new_item.content = content;
    new_item.created_at = time_point_sec(current_time_point());
  });
}

void hashhashhash::clear() {
  require_auth(get_self());

  // Delete all records in _hashes table
  auto hash_itr = _hashes.begin();
  while (hash_itr != _hashes.end()) {
    // Init items table
    items_table _items(get_self(), hash_itr->hashInt);
    auto item_itr = _items.begin();
    while (item_itr != _items.end()) {
      item_itr = _items.erase(item_itr);
    }
    hash_itr = _hashes.erase(hash_itr);
  }
}
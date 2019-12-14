#include <eosio/eosio.hpp>

// Idhash table
struct [[eosio::table("idhash"), eosio::contract("idfile")]] idhash {
    uint64_t    id       = {}; // Non-0
    std::string file_hash  = {};
    eosio::name user     = {};
    std::string file_name  = {};

    uint64_t primary_key() const { return id; }
    std::string get_file_hash() const { return file_hash; }
};

using idhash_table = eosio::multi_index<
    "idhash"_n, idhash, eosio::indexed_by<"by.id"_n, eosio::const_mem_fun<idhash, std::string, &idhash::get_file_hash>>>;

// The contract
class idfile : eosio::contract {
  public:
    // Use contract's constructor
    using contract::contract;

    // Post filehash
    [[eosio::action]] void post(uint64_t  id,const std::string& file_hash, eosio::name user, const std::string& file_name) {
        idhash_table table{get_self(), 0};

        // Check user
        require_auth(user);


        // Create an ID if user didn't specify one
        eosio::check(id < 1'000'000'000ull, "user-specified id is too big");
        if (!id)
            id = std::max(table.available_primary_key(), 1'000'000'000ull);

        // Record the idhash
        table.emplace(get_self(), [&](auto& idhash) {
            idhash.id       = id;
            idhash.file_hash = file_hash;
            idhash.user     = user;
            idhash.file_name  = file_name;
        });
    }
};

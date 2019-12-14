#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("idfilebook")]] idfilebook : public eosio::contract {
  public:
    idfilebook(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}

    [[eosio::action]]
    void upsert(name user, std::string fadada_hash, std::string video_hash) {
        require_auth( user );
        idfile_index idfiles(get_self(), get_first_receiver().value);
        auto iterator = idfiles.find(user.value);
        if( iterator == idfiles.end() )
        {
          idfiles.emplace(user, [&]( auto& row ) {
          row.key = user;
          row.fadada_hash = fadada_hash;
          row.video_hash = video_hash;
          });
        } else {
          idfiles.modify(iterator, user, [&]( auto& row ) {
          row.key = user;
          row.fadada_hash = fadada_hash;
          row.video_hash = video_hash;
          });
        }
    }

    [[eosio::action]]
    void erase(name user) {
      require_auth(user);
      idfile_index idfiles(get_self(), get_first_receiver().value);
      auto iterator = idfiles.find(user.value);
      check(iterator != idfiles.end(), "Record does not exist");
      idfiles.erase(iterator);
    }  

  private: 

    struct [[eosio::table]] person {
        name key;
        std::string fadada_hash;
        std::string video_hash;
 
        uint64_t primary_key() const { return key.value;}
    };
    typedef eosio::multi_index<"people"_n, person> idfile_index;
  
};
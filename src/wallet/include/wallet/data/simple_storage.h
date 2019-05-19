/*!
 * mintex.
 * simple_storage
 *
 * \date 19.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_SIMPLE_STORAGE_H
#define MINTEX_SIMPLE_STORAGE_H

#include <nlohmann/json.hpp>
#include "mintex-tx/minter_public_key.h"
#include "wallet/resp_gen.h"
#include "wallet/kv_storage.h"

namespace wallet {

struct validator_data {
  std::string name;
  mintex::pubkey_t pubkey;
};

void from_json(const nlohmann::json &j, validator_data &v) {
    JSON_GET(j, "name", std::string, v.name);
    JSON_GET(j, "pubkey", mintex::pubkey_t , v.pubkey);
}

void to_json(nlohmann::json &j, const validator_data &v) {
    j = nlohmann::json {
        {"name", v.name},
        {"pubkey", v.pubkey.to_string()}
    };
}

class simple_storage {
 public:
    void init();

    void add_validator(const std::string &name, const validator_data& data);
    void remove_validator(const std::string &name);
 private:
    wallet::db::kv_storage m_storage;
};

}

#endif //MINTEX_SIMPLE_STORAGE_H

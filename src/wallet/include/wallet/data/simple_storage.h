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

#include <map>
#include <nlohmann/json.hpp>
#include <minter/public_key.h>
#include "wallet/resp_gen.h"
#include "wallet/kv_storage.h"

namespace wallet {

struct validator_data {
  std::string name;
  minter::pubkey_t pubkey;
};

void from_json(const nlohmann::json &j, validator_data &v);
void to_json(nlohmann::json &j, const validator_data &v);

class simple_storage {
 public:
    simple_storage() = default;
    simple_storage(wallet::db::kv_storage &&storage);
    void init();

    void set(const std::string &name, const nlohmann::json &j);
    void remove(const std::string &name);
    bool exists(const std::string &name) const;
    nlohmann::json get(const std::string &name) const;

    const wallet::db::kv_storage& get_storage() const;
 private:
    wallet::db::kv_storage m_storage;
};

}

#endif //MINTEX_SIMPLE_STORAGE_H

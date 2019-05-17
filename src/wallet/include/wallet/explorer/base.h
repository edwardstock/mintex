/*!
 * mintex.
 * explorer_result.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_EXPLORER_BASE_H
#define MINTEX_EXPLORER_BASE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include <eth/Common.h>
#include "wallet/resp_gen.h"
#include "mintex-tx/minter_address.h"
#include "mintex-tx/minter_public_key.h"
#include "mintex-tx/minter_hash.h"
#include "mintex-tx/tx_type.h"

namespace wallet {
namespace explorer {

using field_map = std::unordered_map<std::string, std::string>;

NLOHMANN_JSON_SERIALIZE_ENUM(mintex::tx_type_val, {
    { mintex::tx_type_val::send_coin, mintex::tx_type_val::send_coin },
    { mintex::tx_type_val::sell_coin, mintex::tx_type_val::sell_coin },
    { mintex::tx_type_val::buy_coin, mintex::tx_type_val::buy_coin },
    { mintex::tx_type_val::create_coin, mintex::tx_type_val::create_coin },
    { mintex::tx_type_val::declare_candidacy, mintex::tx_type_val::declare_candidacy },
    { mintex::tx_type_val::delegate, mintex::tx_type_val::delegate },
    { mintex::tx_type_val::unbond, mintex::tx_type_val::unbond },
    { mintex::tx_type_val::redeem_check, mintex::tx_type_val::redeem_check },
    { mintex::tx_type_val::set_candidate_on, mintex::tx_type_val::set_candidate_on },
    { mintex::tx_type_val::set_candidate_off, mintex::tx_type_val::set_candidate_off },
    { mintex::tx_type_val::create_multisig, mintex::tx_type_val::create_multisig },
    { mintex::tx_type_val::multisend, mintex::tx_type_val::multisend },
    { mintex::tx_type_val::edit_candidate, mintex::tx_type_val::edit_candidate },
})

CREATE_RESP3(error_result,
    int, code,
    std::string, message,
    field_map, fields
    )

CREATE_RESP4(links_result,
    std::string, first,
    std::string, last,
    std::string, prev,
    std::string, next
)

CREATE_RESP5(meta_result,
    uint64_t, current_page,
    uint64_t, last_page,
    std::string, path,
    uint32_t, per_page,
    uint64_t, total
)

template<class T>
struct base_result {
  T data;
  error_result error;
  links_result links;
  meta_result meta;
};

struct tx_result {};

}
}

namespace nlohmann {
template<typename T>
struct adl_serializer<wallet::explorer::base_result<T>> {
  static void from_json(const json &j, wallet::explorer::base_result<T> &res) {
      if(j.find("data") != j.end()) j.at("data").get_to<T>(res.data);
      if(j.find("error") != j.end()) j.at("error").get_to<wallet::explorer::error_result>(res.error);
      if(j.find("links") != j.end()) j.at("links").get_to<wallet::explorer::links_result>(res.links);
      if(j.find("meta") != j.end()) j.at("meta").get_to<wallet::explorer::meta_result>(res.meta);
  }

  static void to_json(json &j, const wallet::explorer::base_result<T> &res) {

  }
};
}

#endif //MINTEX_EXPLORER_BASE_H

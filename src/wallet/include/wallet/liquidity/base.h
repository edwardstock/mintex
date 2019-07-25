/*!
 * mintex.
 * base
 *
 * \date 16.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_LIQUIDITY_BASE_H
#define MINTEX_LIQUIDITY_BASE_H

#include "wallet/resp_gen.h"

namespace wallet {
namespace liquidity {

CREATE_RESP2(
    error_result,
    int, code,
    std::string, message
)

template<class T>
struct base_result {
  T data;
  error_result error;
};

} // gate
} // wallet

namespace nlohmann {
template<typename T>
struct adl_serializer<wallet::liquidity::base_result<T>> {
  static void from_json(const json &j, wallet::liquidity::base_result<T> &res) {
      if (j.find("data") != j.end()) j.at("data").get_to<T>(res.data);
      if (j.find("error") != j.end()) j.at("error").get_to<wallet::liquidity::error_result>(res.error);
  }

  static void to_json(json &j, const wallet::liquidity::base_result<T> &res) {

  }
};
}

#endif //MINTEX_LIQUIDITY_BASE_H

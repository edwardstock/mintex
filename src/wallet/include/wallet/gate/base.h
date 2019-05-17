/*!
 * mintex.
 * base
 *
 * \date 16.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_GATE_BASE_H
#define MINTEX_GATE_BASE_H

#include "wallet/resp_gen.h"

namespace wallet {
namespace gate {

CREATE_RESP4(
    error_result,
    int, code,
    std::string, log,
    std::string, value,
    std::string, coin
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
struct adl_serializer<wallet::gate::base_result<T>> {
  static void from_json(const json &j, wallet::gate::base_result<T> &res) {
      if (j.find("data") != j.end()) j.at("data").get_to<T>(res.data);
      if (j.find("error") != j.end()) j.at("error").get_to<wallet::gate::error_result>(res.error);
  }

  static void to_json(json &j, const wallet::gate::base_result<T> &res) {

  }
};
}

#endif //MINTEX_GATE_BASE_H

/*!
 * mintex.
 * explorer_repository.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_EXPLORER_REPOSITORY_H
#define MINTEX_EXPLORER_REPOSITORY_H

#include "wallet/base/base_repository.h"
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <iostream>

namespace mintex {
namespace explorer {

class explorer_repository : public mintex::net::base_repository {
public:
    explorer_repository() : base_repository() {

    }
    ~explorer_repository() override {

    }
    std::string getBaseUrl() override {
        return "https://explorer-api.testnet.minter.network/api/v1/";
    }

    void getBalance(const std::string &address,
                    const std::function<void(std::unordered_map<std::string, std::string>)> &cb) {
        auto req = newRequest();
        req.addPath("addresses");
        req.addPath(address);

        getClient()->executeAsync(req, [cb](httb::response resp) {

          nlohmann::json val = nlohmann::json::parse(resp.data);

          auto data = val.at("data");
          auto balances = data.at("balances");

          std::unordered_map<std::string, std::string> out(balances.size());

          size_t sz = balances.size();

          for (int i = 0; i < sz; i++) {
              auto balanceItem = balances.at(i);
              out[balanceItem.at("coin")] = balanceItem.at("amount");
          }

          cb(out);
        });
    }
};

}
}

#endif //MINTEX_EXPLORER_REPOSITORY_H

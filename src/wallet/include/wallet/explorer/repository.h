/*!
 * mintex.
 * explorer_repository.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_REPOSITORY_H
#define MINTEX_REPOSITORY_H

#include <unordered_map>
#include <thread>
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>
#include "wallet/base_repository.h"
#include "wallet/explorer/results.hpp"

using namespace wallet::net;
using namespace mintex;
using namespace mintex::utils;

namespace wallet {
namespace explorer {

using delegations_result_t = std::vector<delegated_item>;
using tx_list_t = std::vector<transaction_item>;

class repository : public wallet::net::base_repository {
public:
    repository() : base_repository() {

    }
    ~repository() override {

    }
    std::string getBaseUrl() const override {
        return "https://explorer-api.testnet.minter.network/api/v1/";
    }

    auto get_balance(const std::string &address) const {
        auto req = newRequest();
        req.addPath("addresses");
        req.addPath(address);

        return std::make_shared<response_task<base_result<balance_items>>>(std::move(req));
    }

    auto get_transactions(const address_t &address, uint32_t limit = 10, uint32_t page = 1) const {
        auto req = newRequest();
        req.addPath("transactions");
        req.addParam({"addresses[]", address.to_string()});
        req.addParam(httb::kvd{"page", page});
        req.addParam(httb::kvd{"limit", limit});

        return std::make_shared<response_task<base_result<tx_list_t>>>(std::move(req));
    }

    auto get_delegated(const std::string &address) const {
        auto req = newRequest();
        req.addPath("addresses");
        req.addPath(address);
        req.addPath("delegations");

        return std::make_shared<response_task<base_result<delegations_result_t>>>(std::move(req));
    }
};

}
}

#endif //MINTEX_REPOSITORY_H

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

#include <unordered_map>
#include <thread>
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>
#include <minter/address.h>
#include "wallet/base_repository.h"
#include "wallet/explorer/results.hpp"

using namespace wallet::net;
using namespace minter;
using namespace minter::utils;

namespace wallet {
namespace explorer {

using delegations_result_t = std::vector<delegated_item>;
using tx_list_t = std::vector<transaction_item>;

class repository : public wallet::net::base_repository {
public:
    enum reward_period {
      none,
      minute,
      hour,
      day
    };

    repository() : base_repository() {

    }
    ~repository() override {

    }
    std::string getBaseUrl() const override {
        return std::string(MINTER_EXPLORER_API);
    }

    auto get_balance(const std::string &address) const {
        auto req = newRequest();
        req.addPath("addresses");
        req.addPath(address);

        return std::make_shared<response_task<base_result<balance_items>>>(std::move(req));
    }

    auto get_balance(const minter::address_t &address) const {
        return get_balance(address.to_string());
    }

    auto get_transactions(const address_t &address, uint32_t page = 1, uint32_t limit = 10) const {
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



    /// \brief
    /// \param address
    /// \param period  Chart scale. Possible variants: minute | hour | day. Default is day
    /// \param start_time Chart start time. Formats: YYYY-MM-DD | YYYY-MM-DD HH:MM:SS
    /// \param end_time Chart end time. Formats: YYYY-MM-DD | YYYY-MM-DD HH:MM:SS
    /// \return
    auto get_rewards(const address_t &address, reward_period period = day, const std::string &start_time = "", const std::string &end_time = "") {
        auto req = newRequest();
        req.addPath("addresses");
        req.addPath(address.to_string());
        req.addPath("statistics/rewards");

        if(period != none) {
            req.addParam({"scale", m_reward_scales[period]});
        }
        if(start_time != "") {
            req.addParam({"startTime", start_time});
        }
        if(end_time != "") {
            req.addParam({"endTime", end_time});
        }

        return make_task_ptr<base_result<std::vector<reward_item>>>(std::move(req));
    }

 private:
    std::unordered_map<reward_period, std::string> m_reward_scales = {
        {minute, "minute"},
        {hour, "hour"},
        {day, "day"},
    };
};

}
}

#endif //MINTEX_REPOSITORY_H

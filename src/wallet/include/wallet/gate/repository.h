/*!
 * mintex.
 * repository
 *
 * \date 15.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_GATE_REPOSITORY_H
#define MINTEX_GATE_REPOSITORY_H

#include <memory>
#include <sstream>
#include <nlohmann/json.hpp>
#include <bip39/utils.h>
#include "mintex-tx/utils.h"
#include "wallet/base_repository.h"
#include "wallet/gate/results.hpp"


using namespace wallet::net;
using namespace mintex;
using namespace mintex::utils;

namespace wallet {
namespace gate {

struct tx_init_data {
  dev::bigint nonce;
  dev::bigint gas = dev::bigint("1");
};

class repository : public wallet::net::base_repository {
 public:

    std::string getBaseUrl() const override {
        return std::string(MINTER_GATE_API);
    }

    task_ptr<base_result<gas_value>> get_min_gas() {
        auto req = newRequest();
        req.addPath("min-gas");

        return make_task_ptr<base_result<gas_value>>(std::move(req));
    }

    task_ptr<base_result<tx_count_value>> get_tx_count(const address_t &address) {
        auto req = newRequest();
        req.addPath("nonce");
        req.addPath(address.to_string());

        return make_task_ptr<base_result<tx_count_value>>(std::move(req));
    }

    task_ptr<base_result<commission_value>> get_tx_commission_value(const minter::Data &tx_sign) {
        auto req = newRequest();
        req.addPath("estimate");
        req.addPath("tx-commission");
        req.addParam({"transaction", tx_sign.toHex()});

        return make_task_ptr<base_result<commission_value>>(std::move(req));
    }

    task_ptr<base_result<exchange_buy_value>> get_exchange_buy_currency(const std::string &coin_to_sell, const dev::bigdec18 &value_to_buy, const std::string &coin_to_buy) {
        auto req = newRequest();
        req.addPath("estimate/coin-buy");
        req.addParam({"coinToSell", coin_to_sell});
        req.addParam({"valueToBuy", utils::to_string(mintex::utils::normalize_value(value_to_buy))});
        req.addParam({"coinToBuy", coin_to_buy});

        return make_task_ptr<base_result<exchange_buy_value>>(std::move(req));
    }

    task_ptr<base_result<exchange_sell_value>> get_exchange_sell_currency(const std::string &coin_to_sell, const dev::bigdec18 &value_to_sell, const std::string &coin_to_buy) {
        auto req = newRequest();
        req.addPath("estimate/coin-sell");
        req.addParam({"coinToSell", coin_to_sell});
        req.addParam({"valueToSell", utils::to_string(mintex::utils::normalize_value(value_to_sell))});
        req.addParam({"coinToBuy", coin_to_buy});

        return make_task_ptr<base_result<exchange_sell_value>>(std::move(req));
    }

    task_ptr<base_result<tx_send_result>> send_tx(const minter::Data &tx_sign) {
        auto req = newRequest();
        req.setMethod(httb::request::method::post);
        req.addPath("transaction/push");

        nlohmann::json j;
        j["transaction"] = tx_sign.toHex();
        std::stringstream ss;
        ss << j;
        httb::body_string b(ss.str());

        req.setBody(b);

        return make_task_ptr<base_result<tx_send_result>>(std::move(req));
    }

    tx_init_data get_tx_init_data(const mintex::address_t &address, bool &error) {
        using namespace wallet::gate;

        error = false;
        dev::bigint nonce = dev::bigint("1");
        dev::bigint gas = dev::bigint("1");

        auto nonce_task =
            get_tx_count(address)
                     ->success([&nonce](base_result<tx_count_value> result) {
                       nonce = result.data.count + dev::bigint("1");
                     })
                     ->error([&error](httb::response resp, base_result<tx_count_value> result) {
                       error = true;
                       std::cerr << resp.statusMessage << std::endl;
                       std::cerr << resp.getBody() << std::endl;
                     });
        auto gas_task =
            get_min_gas()
                     ->success([&gas](base_result<gas_value> result) {
                       gas = result.data.gas;
                     })
                     ->error([&error](httb::response resp, base_result<gas_value> result) {
                       error = true;
                       std::cerr << resp.statusMessage << std::endl;
                       std::cerr << resp.getBody() << std::endl;
                     });

        wallet::net::pair_task<
            wallet::gate::base_result<tx_count_value>,
            base_result<gas_value>
        > executor(nonce_task, gas_task);

        executor.execute();

        tx_init_data out{nonce, gas};

        return out;
    }

};

} // gate
} // wallet

#endif //MINTEX_GATE_REPOSITORY_H

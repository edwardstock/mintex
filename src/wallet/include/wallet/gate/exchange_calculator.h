/*!
 * mintex.
 * exchange_calculator.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_EXCHANGE_CALCULATOR_H
#define MINTEX_EXCHANGE_CALCULATOR_H
#include <string>
#include <functional>
#include <httb/httb.h>
#include <minter/eth/Common.h>
#include <wallet/data/secret_storage.h>
#include "wallet/gate/repository.h"
#include <minter/tx.hpp>

namespace wallet {
namespace gate {

struct exchange_calc_result {
  dev::bigdec18 amount;
  dev::bigdec18 commission;
  std::string calculation;
};

class exchange_calculator {
 public:
    enum op_type {
      buy,
      sell
    };

    using on_error = std::function<void(httb::response)>;
    using on_complete = std::function<void(exchange_calc_result)>;

    exchange_calculator(const std::string &source, const std::string &target, const dev::bigdec18 &amount):
        source_coin(source),
        target_coin(target),
        amount(amount) {

    }

    void calculate(op_type type, on_complete complete_listener, on_error err_listener) {
        wallet::gate::repository repo;

        if(type == buy) {
            auto task = repo.get_exchange_buy_currency(source_coin, amount, target_coin);
            task
            ->error([&err_listener](httb::response resp, base_result<exchange_buy_value> res) {
                err_listener(resp);
            })
            ->success([&complete_listener](base_result<exchange_buy_value> res) {
                exchange_calc_result out;
                out.amount = minter::utils::humanize_value(res.data.will_pay);
                out.commission = minter::utils::humanize_value(res.data.commission);

                complete_listener(out);
            })
            ->execute()->handle().run();
        } else {
            auto task = repo.get_exchange_sell_currency(source_coin, amount, target_coin);
            task
                ->error([&err_listener](httb::response resp, base_result<exchange_sell_value> res) {
                  err_listener(resp);
                })
                ->success([&complete_listener](base_result<exchange_sell_value> res) {
                  exchange_calc_result out;
                  out.amount = minter::utils::humanize_value(res.data.will_get);
                  out.commission = minter::utils::humanize_value(res.data.commission);

                  complete_listener(out);
                })
                ->execute()->handle().run();
        }
    }

 private:
    std::string source_coin, target_coin;
    dev::bigdec18 amount;
};

} // gate
} // wallet

#endif //MINTEX_EXCHANGE_CALCULATOR_H

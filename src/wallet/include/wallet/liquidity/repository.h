/*!
 * mintex.
 * repository.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_LIQUIDITY_REPOSITORY_H
#define MINTEX_LIQUIDITY_REPOSITORY_H

#include <minter/tx/utils.h>
#include <minter/address.h>
#include <minter/eth/Common.h>
#include "wallet/base_repository.h"
#include "wallet/liquidity/base.h"
#include "wallet/liquidity/results.hpp"

using namespace wallet::net;
using namespace minter;
using namespace minter::utils;

namespace wallet {
namespace liquidity {

class repository : public wallet::net::base_repository {
 public:
    std::string getBaseUrl() const override {
        return std::string(MINTER_LP_API);
    }

    task_ptr<base_result<bip_price>> get_price() {
        auto req = newRequest();
        req.addPath("price");
        return make_task_ptr<base_result<bip_price>>(std::move(req));
    }

};

}
}

#endif //MINTEX_LIQUIDITY_REPOSITORY_H

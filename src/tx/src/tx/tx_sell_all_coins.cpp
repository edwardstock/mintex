/*!
 * mintex.
 * tx_sell_all_coins.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_sell_all_coins.h"
#include "mintex-tx/tx_type.h"
mintex::tx_sell_all_coins::tx_sell_all_coins(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}

uint16_t mintex::tx_sell_all_coins::type() const {
    return mintex::tx_sell_all_coins_type::type;
}
dev::bytes mintex::tx_sell_all_coins::encode() {
    return dev::bytes();
}

void mintex::tx_sell_all_coins::decode_internal(dev::RLP rlp) {
    tx_data::decode_internal(rlp);
}

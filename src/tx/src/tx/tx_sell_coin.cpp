/*!
 * mintex.
 * tx_sell_coin.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_sell_coin.h"
#include "mintex-tx/tx_type.h"
mintex::tx_sell_coin::tx_sell_coin(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}
uint16_t mintex::tx_sell_coin::type() const {
    return mintex::tx_sell_coin_type::type;
}
dev::bytes mintex::tx_sell_coin::encode() {
    return dev::bytes();
}

void mintex::tx_sell_coin::decode_internal(dev::RLP rlp) {
    tx_data::decode_internal(rlp);
}


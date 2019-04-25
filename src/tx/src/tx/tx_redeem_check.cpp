/*!
 * mintex.
 * tx_redeem_check.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_redeem_check.h"
#include "mintex-tx/tx_type.h"
mintex::tx_redeem_check::tx_redeem_check(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}

uint16_t mintex::tx_redeem_check::type() const {
    return mintex::tx_redeem_check_type::type;
}
dev::bytes mintex::tx_redeem_check::encode() {
    return dev::bytes();
}

void mintex::tx_redeem_check::decode_internal(dev::RLP rlp) {
    tx_data::decode_internal(rlp);
}


/*!
 * mintex.
 * tx_multisend.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_multisend.h"
#include "mintex-tx/tx_type.h"
mintex::tx_multisend::tx_multisend(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}
uint16_t mintex::tx_multisend::type() const {
    return mintex::tx_multisend_type::type;
}
dev::bytes mintex::tx_multisend::encode() {
    return dev::bytes();
}

void mintex::tx_multisend::decode_internal(dev::RLP rlp) {
    tx_data::decode_internal(rlp);
}


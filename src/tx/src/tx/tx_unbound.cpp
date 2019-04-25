/*!
 * mintex.
 * tx_unbound.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_unbound.h"
#include "mintex-tx/tx_type.h"
mintex::tx_unbound::tx_unbound(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}

void mintex::tx_unbound::decode_internal(dev::RLP rlp) {
    tx_data::decode_internal(rlp);
}

uint16_t mintex::tx_unbound::type() const {
    return mintex::tx_unbound_type::type;
}
dev::bytes mintex::tx_unbound::encode() {
    return dev::bytes();
}
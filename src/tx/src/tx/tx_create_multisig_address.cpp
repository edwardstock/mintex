/*!
 * mintex.
 * tx_create_multisig_address.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_create_multisig_address.h"
#include "mintex-tx/tx_type.h"
mintex::tx_create_multisig_address::tx_create_multisig_address(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}
uint16_t mintex::tx_create_multisig_address::type() const {
    return mintex::tx_create_multisig_address_type::type;
}
dev::bytes mintex::tx_create_multisig_address::encode() {
    return dev::bytes();
}

void mintex::tx_create_multisig_address::decode_internal(dev::RLP rlp) {
    tx_data::decode_internal(rlp);
}


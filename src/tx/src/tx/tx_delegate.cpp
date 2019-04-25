/*!
 * mintex.
 * tx_delegate.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_delegate.h"
#include "mintex-tx/tx_type.h"
mintex::tx_delegate::tx_delegate(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}
uint16_t mintex::tx_delegate::type() const {
    return mintex::tx_delegate_type::type;
}
dev::bytes mintex::tx_delegate::encode() {
    return dev::bytes();
}

void mintex::tx_delegate::decode_internal(dev::RLP rlp) {
    tx_data::decode_internal(rlp);
}


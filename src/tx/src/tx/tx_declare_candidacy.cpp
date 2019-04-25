/*!
 * mintex.
 * tx_declare_candidacy.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_declare_candidacy.h"
#include "mintex-tx/tx_type.h"
mintex::tx_declare_candidacy::tx_declare_candidacy(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}
uint16_t mintex::tx_declare_candidacy::type() const {
    return mintex::tx_declare_candidacy_type::type;
}
dev::bytes mintex::tx_declare_candidacy::encode() {
    return dev::bytes();
}

void mintex::tx_declare_candidacy::decode_internal(dev::RLP rlp) {
    tx_data::decode_internal(rlp);
}


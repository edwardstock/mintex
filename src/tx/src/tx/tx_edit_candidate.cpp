/*!
 * mintex.
 * tx_edit_candidate.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_edit_candidate.h"
#include "mintex-tx/tx_type.h"
mintex::tx_edit_candidate::tx_edit_candidate(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}
uint16_t mintex::tx_edit_candidate::type() const {
    return mintex::tx_edit_candidate_type::type;
}
dev::bytes mintex::tx_edit_candidate::encode() {
    return dev::bytes();
}

void mintex::tx_edit_candidate::decode_internal(dev::RLP rlp) {
    tx_data::decode_internal(rlp);
}


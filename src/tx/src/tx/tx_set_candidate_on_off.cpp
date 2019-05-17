/*!
 * mintex.
 * tx_set_candidate_on_off.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_type.h"
#include "mintex-tx/tx_set_candidate_on_off.h"
#include "mintex-tx/tx_type.h"

// BASE
mintex::tx_set_candidate_on_off::tx_set_candidate_on_off(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}

dev::bytes mintex::tx_set_candidate_on_off::encode() {
    dev::RLPStream out;
    dev::RLPStream lst;
    {
        lst.append((dev::bytes)m_pub_key);
        out.appendList(lst);
    }

    return out.out();
}

void mintex::tx_set_candidate_on_off::decode_internal(dev::RLP rlp) {
    m_pub_key = (dev::bytes)rlp[0];
}

mintex::tx_set_candidate_on_off& mintex::tx_set_candidate_on_off::set_pub_key(const dev::bytes& pub_key) {
    m_pub_key = pub_key;
    return *this;
}

mintex::tx_set_candidate_on_off& mintex::tx_set_candidate_on_off::set_pub_key(const mintex::pubkey_t &pub_key) {
    m_pub_key = pub_key;
    return *this;
}

const mintex::pubkey_t& mintex::tx_set_candidate_on_off::get_pub_key() const {
    return m_pub_key;
}

// ON
uint16_t mintex::tx_set_candidate_on::type() const {
    return mintex::tx_set_candidate_on_type::type();
}
mintex::tx_set_candidate_on::tx_set_candidate_on(std::shared_ptr<mintex::tx> tx) : tx_set_candidate_on_off(tx), tx_data(tx) {

}
void mintex::tx_set_candidate_on::decode_internal(dev::RLP rlp) {
    tx_set_candidate_on_off::decode_internal(rlp);
}
dev::bytes mintex::tx_set_candidate_on::encode() {
    return tx_set_candidate_on_off::encode();
}

// OFF

uint16_t mintex::tx_set_candidate_off::type() const {
    return mintex::tx_set_candidate_off_type::type();
}
mintex::tx_set_candidate_off::tx_set_candidate_off(std::shared_ptr<mintex::tx> tx) : tx_set_candidate_on_off(tx), tx_data(tx) {

}
void mintex::tx_set_candidate_off::decode_internal(dev::RLP rlp) {
    tx_set_candidate_on_off::decode_internal(rlp);
}

dev::bytes mintex::tx_set_candidate_off::encode() {
    return tx_set_candidate_on_off::encode();
}

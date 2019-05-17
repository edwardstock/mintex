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
    return mintex::tx_edit_candidate_type::type();
}

dev::bytes mintex::tx_edit_candidate::encode() {
    dev::RLPStream out;
    dev::RLPStream lst;
    {
        lst.append((dev::bytes)m_pub_key);
        lst.append(m_reward_address.get());
        lst.append(m_owner_address.get());

        out.appendList(lst);
    }

    return out.out();
}

void mintex::tx_edit_candidate::decode_internal(dev::RLP rlp) {
    m_pub_key = (dev::bytes)rlp[0];
    m_reward_address = (dev::bytes)rlp[1];
    m_owner_address = (dev::bytes)rlp[2];
}

mintex::tx_edit_candidate &mintex::tx_edit_candidate::set_pub_key(const mintex::pubkey_t &pub_key) {
    m_pub_key = pub_key;
    return *this;
}

mintex::tx_edit_candidate &mintex::tx_edit_candidate::set_pub_key(const dev::bytes &pub_key) {
    m_pub_key = pub_key;
    return *this;
}

mintex::tx_edit_candidate &mintex::tx_edit_candidate::set_reward_address(const mintex::data::minter_address &address) {
    m_reward_address = address;
    return *this;
}

mintex::tx_edit_candidate &mintex::tx_edit_candidate::set_owner_address(const mintex::data::minter_address &address) {
    m_owner_address = address;
    return *this;
}

const mintex::pubkey_t& mintex::tx_edit_candidate::get_pub_key() const {
    return m_pub_key;
}

const mintex::data::minter_address mintex::tx_edit_candidate::get_reward_address() const {
    return m_reward_address;
}

const mintex::data::minter_address mintex::tx_edit_candidate::get_owner_address() const {
    return m_owner_address;
}


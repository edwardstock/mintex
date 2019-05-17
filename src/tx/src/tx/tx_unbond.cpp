/*!
 * mintex.
 * tx_unbound.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_unbond.h"
#include "mintex-tx/tx_type.h"
#include "mintex-tx/utils.h"
mintex::tx_unbond::tx_unbond(std::shared_ptr<mintex::tx> tx)
        :tx_data(std::move(tx)) {

}

void mintex::tx_unbond::decode_internal(dev::RLP rlp) {
    m_pub_key = (dev::bytes)rlp[0];
    m_coin = mintex::utils::to_string_clear((dev::bytes)rlp[1]);
    m_value = (dev::bigint) rlp[2];
}

uint16_t mintex::tx_unbond::type() const {
    return mintex::tx_unbond_type::type();
}

#include <iostream>
dev::bytes mintex::tx_unbond::encode() {
    dev::RLPStream out;
    dev::RLPStream lst;
    {
        lst.append((dev::bytes)m_pub_key);
        lst.append(mintex::utils::to_bytes_fixed(m_coin, 10));
        lst.append(m_value);
        out.appendList(lst);
    }

    return out.out();
}

mintex::tx_unbond &mintex::tx_unbond::set_pub_key(const dev::bytes &pub_key) {
    m_pub_key = pub_key;
    return *this;
}
mintex::tx_unbond &mintex::tx_unbond::set_pub_key(const mintex::pubkey_t &pub_key) {
    m_pub_key = pub_key;
    return *this;
}
mintex::tx_unbond &mintex::tx_unbond::set_coin(const char* coin) {
    m_coin = std::string(coin);
    return *this;
}
mintex::tx_unbond &mintex::tx_unbond::set_value(const char* value) {
    m_value = mintex::utils::normalize_value(value);
    return *this;
}
mintex::tx_unbond &mintex::tx_unbond::set_value(const dev::bigdec18 &value) {
    m_value = mintex::utils::normalize_value(value);
    return *this;
}
const mintex::pubkey_t &mintex::tx_unbond::get_pub_key() const {
    return m_pub_key;
}
std::string mintex::tx_unbond::get_coin() const {
    return m_coin;
}
dev::bigdec18 mintex::tx_unbond::get_value() const {
    return mintex::utils::humanize_value(m_value);
}

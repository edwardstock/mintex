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
#include "mintex-tx/utils.h"
mintex::tx_delegate::tx_delegate(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}
uint16_t mintex::tx_delegate::type() const {
    return mintex::tx_delegate_type::type();
}
dev::bytes mintex::tx_delegate::encode() {
    dev::RLPStream out;
    dev::RLPStream lst;
    {
        lst.append((dev::bytes)m_pub_key);
        lst.append(mintex::utils::to_bytes_fixed(m_coin));
        lst.append(m_stake);

        out.appendList(lst);
    }

    return out.out();
}

void mintex::tx_delegate::decode_internal(dev::RLP rlp) {
    m_pub_key = (dev::bytes)rlp[0];
    m_coin = mintex::utils::to_string_clear((dev::bytes)rlp[1]);
    m_stake = (dev::bigint)rlp[2];
}

mintex::tx_delegate &mintex::tx_delegate::set_pub_key(const dev::bytes &pub_key) {
    m_pub_key = pub_key;
    return *this;
}

mintex::tx_delegate &mintex::tx_delegate::set_pub_key(const mintex::pubkey_t &pub_key) {
    m_pub_key = pub_key;
    return *this;
}

mintex::tx_delegate &mintex::tx_delegate::set_coin(const char *coin) {
    m_coin = std::string(coin);
    return *this;
}

mintex::tx_delegate &mintex::tx_delegate::set_stake(const char *amount) {
    m_stake = mintex::utils::normalize_value(amount);
    return *this;
}

mintex::tx_delegate &mintex::tx_delegate::set_stake(const dev::bigdec18 &amount) {
    m_stake = mintex::utils::normalize_value(amount);
    return *this;
}

const mintex::pubkey_t& mintex::tx_delegate::get_pub_key() const {
    return m_pub_key;
}

std::string mintex::tx_delegate::get_coin() const {
    return m_coin;
}

dev::bigdec18 mintex::tx_delegate::get_stake() const {
    return mintex::utils::humanize_value(m_stake);
}


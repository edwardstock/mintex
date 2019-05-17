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
#include "mintex-tx/utils.h"
mintex::tx_declare_candidacy::tx_declare_candidacy(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}
uint16_t mintex::tx_declare_candidacy::type() const {
    return mintex::tx_declare_candidacy_type::type();
}
dev::bytes mintex::tx_declare_candidacy::encode() {
    dev::RLPStream out;
    dev::RLPStream lst;
    {
        lst.append(m_address.get());
        lst.append((dev::bytes)m_pub_key);
        lst.append(m_commission);
        lst.append(mintex::utils::to_bytes_fixed(m_coin, 10));
        lst.append(m_stake);

        out.appendList(lst);
    }

    return out.out();
}

void mintex::tx_declare_candidacy::decode_internal(dev::RLP rlp) {
    m_address = (dev::bytes)rlp[0];
    m_pub_key = (dev::bytes)rlp[1];
    m_commission = (dev::bigint)rlp[2];
    m_coin = mintex::utils::to_string_clear((dev::bytes)rlp[3]);
    m_stake = (dev::bigint)rlp[4];
}

mintex::tx_declare_candidacy &mintex::tx_declare_candidacy::set_address(const mintex::data::minter_address &address) {
    m_address = address;
    return *this;
}

mintex::tx_declare_candidacy &mintex::tx_declare_candidacy::set_pub_key(const mintex::pubkey_t &pub_key) {
    m_pub_key = pub_key;
    return *this;
}

mintex::tx_declare_candidacy &mintex::tx_declare_candidacy::set_pub_key(const dev::bytes &pub_key) {
    m_pub_key = pub_key;
    return *this;
}

mintex::tx_declare_candidacy &mintex::tx_declare_candidacy::set_commission(unsigned commission) {
    m_commission = dev::bigint(commission);
    return *this;
}

mintex::tx_declare_candidacy &mintex::tx_declare_candidacy::set_coin(const char *coin) {
    m_coin = std::string(coin);
    return *this;
}

mintex::tx_declare_candidacy &mintex::tx_declare_candidacy::set_stake(const char *amount) {
    m_stake = mintex::utils::normalize_value(amount);
    return *this;
}

mintex::tx_declare_candidacy &mintex::tx_declare_candidacy::set_stake(const dev::bigdec18 &amount) {
    m_stake = mintex::utils::normalize_value(amount);
    return *this;
}

const mintex::data::minter_address &mintex::tx_declare_candidacy::get_address() const {
    return m_address;
}

const mintex::pubkey_t& mintex::tx_declare_candidacy::get_pub_key() const {
    return m_pub_key;
}

unsigned mintex::tx_declare_candidacy::get_commission() const {
    return static_cast<unsigned>(m_commission);
}

std::string mintex::tx_declare_candidacy::get_coin() const {
    return m_coin;
}

dev::bigdec18 mintex::tx_declare_candidacy::get_stake() const {
    return mintex::utils::humanize_value(m_stake);
}


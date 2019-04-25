/*!
 * mintex.
 * tx_send_coin.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/utils.h"
#include "mintex-tx/tx_send_coin.h"
#include "mintex-tx/tx_type.h"

mintex::tx_send_coin::tx_send_coin(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {
}

mintex::tx_send_coin &mintex::tx_send_coin::set_coin(const std::string &coin) {
    m_coin = coin;
    return *this;
}
mintex::tx_send_coin &mintex::tx_send_coin::set_coin(std::string &&coin) {
    m_coin = std::move(coin);
    return *this;
}
mintex::tx_send_coin &mintex::tx_send_coin::set_coin(const char *coin) {
    m_coin = std::string(coin);
    return *this;
}

mintex::tx_send_coin &mintex::tx_send_coin::set_to(const char *address) {
    return set_to(mintex::data::minter_address(address));
}

mintex::tx_send_coin &mintex::tx_send_coin::set_to(const std::string &address) {
    return set_to(address.c_str());
}

mintex::tx_send_coin &mintex::tx_send_coin::set_to(const mintex::data::minter_address &address) {
    m_to = address;
    return *this;
}

mintex::tx_send_coin &mintex::tx_send_coin::set_value(const std::string &normalized) {
    m_value = mintex::utils::normalize_value(normalized);
    return *this;
}

mintex::tx_send_coin &mintex::tx_send_coin::set_value(const dev::bigdec18 &normalized) {
    m_value = mintex::utils::normalize_value(normalized);
    return *this;
}

mintex::tx_send_coin &mintex::tx_send_coin::set_value(const dev::bigint &raw) {
    m_value = raw;
    return *this;
}

uint16_t mintex::tx_send_coin::type() const {
    return mintex::tx_send_coin_type::type;
}

std::string mintex::tx_send_coin::get_coin() const {
    return mintex::utils::strip_null_bytes(m_coin.c_str());
}

mintex::data::minter_address mintex::tx_send_coin::get_to() const {
    return m_to;
}

dev::bigdec18 mintex::tx_send_coin::get_value() const {
    return mintex::utils::humanize_value(m_value);
}

dev::bytes mintex::tx_send_coin::encode() {
    dev::RLPStream out;
    dev::RLPStream lst;
    {
        lst.append(mintex::utils::to_bytes_fixed(m_coin));
        lst.append(m_to.get());
        lst.append(m_value);
        out.appendList(lst);
    }

    return out.out();
}

void mintex::tx_send_coin::decode_internal(dev::RLP rlp) {
    m_coin = (std::string)rlp[0];
    m_to = mintex::data::minter_address((dev::bytes)rlp[1]);
    m_value = (dev::bigint)rlp[2];
}


/*!
 * mintex.
 * tx_create_coin.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/utils.h"
#include "mintex-tx/tx_create_coin.h"
#include "mintex-tx/tx_type.h"
mintex::tx_create_coin::tx_create_coin(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {
}

uint16_t mintex::tx_create_coin::type() const {
    return mintex::tx_create_coin_type::type;
}
dev::bytes mintex::tx_create_coin::encode() {
    dev::RLPStream out;
    dev::RLPStream lst;
    {
        lst.append(mintex::utils::to_bytes(m_name));
        lst.append(mintex::utils::to_bytes_fixed(m_ticker));
        lst.append(m_initial_amount);
        lst.append(m_initial_reserve);
        lst.append(m_crr);

        out.appendList(lst);
    }

    return out.out();
}

void mintex::tx_create_coin::decode_internal(dev::RLP rlp) {
    m_name = mintex::utils::to_string((dev::bytes)rlp[0]);
    m_ticker = mintex::utils::to_string_clear((dev::bytes)rlp[1]);
    m_initial_amount = (dev::bigint)rlp[2];
    m_initial_reserve = (dev::bigint)rlp[3];
    m_crr = (dev::bigint)rlp[4];
}

mintex::tx_create_coin& mintex::tx_create_coin::set_name(const char* name) {
    m_name = std::string(name);
    return *this;
}

mintex::tx_create_coin& mintex::tx_create_coin::set_ticker(const char *coin_symbol) {
    m_ticker = std::string(coin_symbol);
    return *this;
}

mintex::tx_create_coin& mintex::tx_create_coin::set_initial_amount(const char* amount) {
    m_initial_amount = mintex::utils::normalize_value(amount);
    return *this;
}

mintex::tx_create_coin& mintex::tx_create_coin::set_initial_amount(const dev::bigdec18 &amount) {
    m_initial_amount = mintex::utils::normalize_value(amount);
    return *this;
}

mintex::tx_create_coin& mintex::tx_create_coin::set_initial_amount(const dev::bigint &amount) {
    m_initial_amount = amount;
    return *this;
}

mintex::tx_create_coin& mintex::tx_create_coin::set_initial_reserve(const char* amount) {
    m_initial_reserve = mintex::utils::normalize_value(amount);
    return *this;
}

mintex::tx_create_coin& mintex::tx_create_coin::set_initial_reserve(const dev::bigdec18 &amount) {
    m_initial_reserve = mintex::utils::normalize_value(amount);
    return *this;
}

mintex::tx_create_coin& mintex::tx_create_coin::set_initial_reserve(const dev::bigint &amount) {
    m_initial_reserve = amount;
    return *this;
}

mintex::tx_create_coin& mintex::tx_create_coin::set_crr(unsigned crr) {
    m_crr = dev::bigint(crr);
    return *this;
}

std::string mintex::tx_create_coin::get_name() const {
    return m_name;
}

std::string mintex::tx_create_coin::get_ticker() const {
    return m_ticker;
}

dev::bigdec18 mintex::tx_create_coin::get_initial_amount() const {
    return mintex::utils::humanize_value(m_initial_amount);
}

dev::bigdec18 mintex::tx_create_coin::get_initial_reserve() const {
    return mintex::utils::humanize_value(m_initial_reserve);
}

unsigned mintex::tx_create_coin::get_crr() const {
    return static_cast<unsigned>(m_crr);
}
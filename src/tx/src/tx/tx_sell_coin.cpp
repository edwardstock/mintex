/*!
 * mintex.
 * tx_sell_coin.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_sell_coin.h"
#include "mintex-tx/tx_type.h"
#include "mintex-tx/utils.h"
mintex::tx_sell_coin::tx_sell_coin(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}
uint16_t mintex::tx_sell_coin::type() const {
    return mintex::tx_sell_coin_type::type;
}
dev::bytes mintex::tx_sell_coin::encode() {
    dev::RLPStream out;
    dev::RLPStream lst;
    {
        lst.append(mintex::utils::to_bytes_fixed(m_coin_to_sell));
        lst.append(m_value_to_sell);
        lst.append(mintex::utils::to_bytes_fixed(m_coin_to_buy));
        lst.append(m_min_value_to_buy);
        out.appendList(lst);
    }

    return out.out();
}

void mintex::tx_sell_coin::decode_internal(dev::RLP rlp) {
    m_coin_to_sell = mintex::utils::to_string_clear((dev::bytes)rlp[0]);
    m_value_to_sell = (dev::bigint)rlp[1];
    m_coin_to_buy = mintex::utils::to_string_clear((dev::bytes)rlp[2]);
    m_min_value_to_buy = (dev::bigint)rlp[3];
}

mintex::tx_sell_coin& mintex::tx_sell_coin::set_coin_to_sell(const char* coin) {
    m_coin_to_sell = std::string(coin);
    return *this;
}
mintex::tx_sell_coin& mintex::tx_sell_coin::set_value_to_sell(const char* amount) {
    m_value_to_sell = mintex::utils::normalize_value(amount);
    return *this;
}
mintex::tx_sell_coin& mintex::tx_sell_coin::set_value_to_sell(const dev::bigdec18& amount) {
    m_value_to_sell = mintex::utils::normalize_value(amount);
    return *this;
}
mintex::tx_sell_coin& mintex::tx_sell_coin::set_coin_to_buy(const char* coin) {
    m_coin_to_buy = std::string(coin);
    return *this;
}
mintex::tx_sell_coin& mintex::tx_sell_coin::set_min_value_to_buy(const char* amount) {
    m_min_value_to_buy = mintex::utils::normalize_value(amount);
    return *this;
}
mintex::tx_sell_coin& mintex::tx_sell_coin::set_min_value_to_buy(const dev::bigdec18& amount) {
    m_min_value_to_buy = mintex::utils::normalize_value(amount);
    return *this;
}
std::string mintex::tx_sell_coin::get_coin_to_sell() const {
    return m_coin_to_sell;
}
std::string mintex::tx_sell_coin::get_coin_to_buy() const {
    return m_coin_to_buy;
}
dev::bigdec18 mintex::tx_sell_coin::get_value_to_sell() const {
    return mintex::utils::humanize_value(m_value_to_sell);
}
dev::bigdec18 mintex::tx_sell_coin::get_min_value_to_buy() const {
    return mintex::utils::humanize_value(m_min_value_to_buy);
}


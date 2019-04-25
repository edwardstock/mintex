/*!
 * mintex.
 * tx_buy_coin.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_buy_coin.h"
#include "mintex-tx/utils.h"
#include "mintex-tx/tx_type.h"

mintex::tx_buy_coin::tx_buy_coin(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}
uint16_t mintex::tx_buy_coin::type() const {
    return mintex::tx_buy_coin_type::type;
}
dev::bytes mintex::tx_buy_coin::encode() {
    dev::RLPStream out;
    dev::RLPStream lst;
    {
        lst.append(mintex::utils::to_bytes_fixed(m_coin_to_buy));
        lst.append(m_value_to_buy);
        lst.append(mintex::utils::to_bytes_fixed(m_coin_to_sell));
        lst.append(m_max_value_to_sell);
        out.appendList(lst);
    }

    return out.out();
}
void mintex::tx_buy_coin::decode_internal(dev::RLP rlp) {
    m_coin_to_buy = mintex::utils::to_string_clear((dev::bytes)rlp[0]);
    m_value_to_buy = mintex::utils::to_bigint((dev::bytes)rlp[1]);
    m_coin_to_sell = mintex::utils::to_string_clear((dev::bytes)rlp[2]);
    m_max_value_to_sell = mintex::utils::to_bigint((dev::bytes)rlp[3]);
}

std::string mintex::tx_buy_coin::get_coin_to_buy() const {
    return m_coin_to_buy;
}

std::string mintex::tx_buy_coin::get_coin_to_sell() const {
    return m_coin_to_sell;
}

dev::bigdec18 mintex::tx_buy_coin::get_value_to_buy() const {
    return mintex::utils::humanize_value(m_value_to_buy);
}

dev::bigdec18 mintex::tx_buy_coin::get_max_value_to_sell() const {
    return mintex::utils::humanize_value(m_max_value_to_sell);
}

mintex::tx_buy_coin& mintex::tx_buy_coin::set_coin_to_buy(const char *coin) {
    m_coin_to_buy = std::string(coin);
    return *this;
}

mintex::tx_buy_coin& mintex::tx_buy_coin::set_value_to_buy(const char* valueDec) {
    m_value_to_buy = mintex::utils::normalize_value(valueDec);
    return *this;
}

mintex::tx_buy_coin& mintex::tx_buy_coin::set_value_to_buy(const dev::bigdec18 &valueDec) {
    m_value_to_buy = mintex::utils::normalize_value(valueDec);
    return *this;
}

mintex::tx_buy_coin& mintex::tx_buy_coin::set_value_to_buy(const dev::bigint &value) {
    m_value_to_buy = value;
    return *this;
}

mintex::tx_buy_coin& mintex::tx_buy_coin::set_coin_to_sell(const char *coin) {
    m_coin_to_sell = std::string(coin);
    return *this;
}

mintex::tx_buy_coin& mintex::tx_buy_coin::set_max_value_to_sell(const char *valueDec) {
    m_max_value_to_sell = mintex::utils::normalize_value(valueDec);
    return *this;
}

mintex::tx_buy_coin& mintex::tx_buy_coin::set_max_value_to_sell(const dev::bigdec18 &valueDec) {
    m_max_value_to_sell = mintex::utils::normalize_value(valueDec);
    return *this;
}

mintex::tx_buy_coin& mintex::tx_buy_coin::set_max_value_to_sell(const dev::bigint &value) {
    m_max_value_to_sell = value;
    return *this;
}





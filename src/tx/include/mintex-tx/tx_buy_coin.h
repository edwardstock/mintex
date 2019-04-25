/*!
 * mintex.
 * tx_buy_coin.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_BUY_COIN_H
#define MINTEX_TX_BUY_COIN_H

#include "tx_data.h"

namespace mintex {

class tx_buy_coin: public virtual mintex::tx_data {
public:
    explicit tx_buy_coin(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;

    std::string get_coin_to_buy() const;
    dev::bigdec18 get_value_to_buy() const;
    std::string get_coin_to_sell() const;
    dev::bigdec18 get_max_value_to_sell() const;

    tx_buy_coin& set_coin_to_buy(const char* coin);
    tx_buy_coin& set_value_to_buy(const char* valueDec);
    tx_buy_coin& set_value_to_buy(const dev::bigdec18 &valueDec);
    tx_buy_coin& set_value_to_buy(const dev::bigint &valueRaw);
    tx_buy_coin& set_coin_to_sell(const char* coin);
    tx_buy_coin& set_max_value_to_sell(const char* valueDec);
    tx_buy_coin& set_max_value_to_sell(const dev::bigdec18 &valueDec);
    tx_buy_coin& set_max_value_to_sell(const dev::bigint &value);
protected:
    void decode_internal(dev::RLP rlp) override;

private:
    std::string m_coin_to_buy;
    dev::bigint m_value_to_buy;
    std::string m_coin_to_sell;
    dev::bigint m_max_value_to_sell;
};

}

#endif //MINTEX_TX_BUY_COIN_H
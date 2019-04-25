/*!
 * mintex.
 * tx_sell_all_coins.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_SELL_ALL_COINS_H
#define MINTEX_TX_SELL_ALL_COINS_H

#include "tx_data.h"

namespace mintex {

class tx_sell_all_coins: public virtual mintex::tx_data {
public:
    explicit tx_sell_all_coins(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;
protected:
    void decode_internal(dev::RLP rlp) override;
};

}

#endif //MINTEX_TX_SELL_ALL_COINS_H

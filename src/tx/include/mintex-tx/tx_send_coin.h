/*!
 * mintex.
 * tx_send_coin.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_SEND_COIN_H
#define MINTEX_TX_SEND_COIN_H

#include <eth/RLP.h>
#include "minter_address.h"
#include "tx_data.h"

namespace mintex {

class tx_send_coin: public virtual mintex::tx_data {
public:
    explicit tx_send_coin(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;
    tx_send_coin& set_coin(const std::string &coin);

    tx_send_coin& set_coin(std::string &&coin);
    tx_send_coin& set_coin(const char* coin);
    tx_send_coin& set_to(const mintex::data::minter_address &address);
    tx_send_coin& set_to(const std::string &address);
    tx_send_coin& set_to(const char* address);
    tx_send_coin& set_value(const dev::bigdec18 &normalized);
    tx_send_coin& set_value(const std::string &normalized);
    tx_send_coin& set_value(const dev::bigint &raw);
    std::string get_coin() const;

    mintex::data::minter_address get_to() const;
    dev::bigdec18 get_value() const;

protected:
    void decode_internal(dev::RLP rlp) override;

private:
    std::string m_coin;
    mintex::data::minter_address m_to;
    dev::bigint m_value;
};

}

#endif //MINTEX_TX_SEND_COIN_H

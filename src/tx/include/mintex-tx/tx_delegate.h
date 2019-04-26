/*!
 * mintex.
 * tx_delegate.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_DELEGATE_H
#define MINTEX_TX_DELEGATE_H

#include <string>
#include "tx_data.h"
namespace mintex {

class tx_delegate: public virtual mintex::tx_data {
public:
    explicit tx_delegate(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;

    tx_delegate& set_pub_key(const minter::Data32 &pub_key);
    tx_delegate& set_coin(const char* coin);
    tx_delegate& set_stake(const char* amount);
    tx_delegate& set_stake(const dev::bigdec18 &amount);

    const minter::Data32 get_pub_key() const;
    std::string get_coin() const;
    dev::bigdec18 get_stake() const;

protected:
    void decode_internal(dev::RLP rlp) override;

private:
    minter::Data32 m_pub_key;
    std::string m_coin;
    dev::bigint m_stake;
};

}

#endif //MINTEX_TX_DELEGATE_H

/*!
 * mintex.
 * tx_set_candidate_on_off.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_SET_CANDIDATE_ON_OFF_H
#define MINTEX_TX_SET_CANDIDATE_ON_OFF_H

#include "tx_data.h"
#include "mintex-tx/minter_public_key.h"

namespace mintex {

class tx_set_candidate_on_off: public virtual mintex::tx_data {
public:
    explicit tx_set_candidate_on_off(std::shared_ptr<mintex::tx> tx);

    dev::bytes encode() override;

    tx_set_candidate_on_off& set_pub_key(const dev::bytes &pub_key);
    tx_set_candidate_on_off& set_pub_key(const mintex::pubkey_t &pub_key);
    const mintex::pubkey_t& get_pub_key() const;

protected:
    void decode_internal(dev::RLP rlp) override;
    mintex::pubkey_t m_pub_key;

};

class tx_set_candidate_on: public mintex::tx_set_candidate_on_off {
public:
    explicit tx_set_candidate_on(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;
protected:
    void decode_internal(dev::RLP rlp) override;
};

class tx_set_candidate_off: public mintex::tx_set_candidate_on_off {
public:
    explicit tx_set_candidate_off(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;
protected:
    void decode_internal(dev::RLP rlp) override;
};



}

#endif //MINTEX_TX_SET_CANDIDATE_ON_OFF_H

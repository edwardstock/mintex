/*!
 * mintex.
 * tx_edit_candidate.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_EDIT_CANDIDATE_H
#define MINTEX_TX_EDIT_CANDIDATE_H

#include "tx_data.h"
#include "minter_public_key.h"

namespace mintex {

class tx_edit_candidate: public virtual mintex::tx_data {
public:
    explicit tx_edit_candidate(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;

    tx_edit_candidate& set_pub_key(const mintex::pubkey_t &pub_key);
    tx_edit_candidate& set_pub_key(const dev::bytes &pub_key);
    tx_edit_candidate& set_reward_address(const mintex::data::minter_address &address);
    tx_edit_candidate& set_owner_address(const mintex::data::minter_address &address);

    const mintex::pubkey_t& get_pub_key() const;
    const mintex::data::minter_address get_reward_address() const;
    const mintex::data::minter_address get_owner_address() const;

protected:
    void decode_internal(dev::RLP rlp) override;

private:
    mintex::pubkey_t m_pub_key;
    mintex::data::minter_address m_reward_address;
    mintex::data::minter_address m_owner_address;
};

}

#endif //MINTEX_TX_EDIT_CANDIDATE_H

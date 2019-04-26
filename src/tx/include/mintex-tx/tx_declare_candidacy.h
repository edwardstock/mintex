/*!
 * mintex.
 * tx_declare_candidacy.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_DECLARE_CANDIDACY_H
#define MINTEX_TX_DECLARE_CANDIDACY_H

#include "tx_data.h"

namespace mintex {

class tx_declare_candidacy: public virtual mintex::tx_data {
public:
    explicit tx_declare_candidacy(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;


    tx_declare_candidacy& set_address(const mintex::data::minter_address &address);
    tx_declare_candidacy& set_pub_key(const minter::Data32 &pub_key);
    /// validator reward
    /// \param commission from 10 to 100 percent
    /// \return
    tx_declare_candidacy& set_commission(unsigned commission);
    tx_declare_candidacy& set_coin(const char* coin);
    tx_declare_candidacy& set_stake(const char* amount);
    tx_declare_candidacy& set_stake(const dev::bigdec18 &amount);

    const mintex::data::minter_address& get_address() const;
    const minter::Data32& get_pub_key() const;
    unsigned get_commission() const;
    std::string get_coin() const;
    dev::bigdec18 get_stake() const;


protected:
    void decode_internal(dev::RLP rlp) override;

private:
    mintex::data::minter_address m_address;
    minter::Data32 m_pub_key;
    dev::bigint m_commission;
    std::string m_coin;
    dev::bigint m_stake;
};

}

#endif //MINTEX_TX_DECLARE_CANDIDACY_H

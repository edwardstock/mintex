/*!
 * mintex.
 * tx_unbound.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_UNBOND_H
#define MINTEX_TX_UNBOND_H
#include "tx_data.h"
namespace mintex {

class tx_unbond: public virtual mintex::tx_data {
public:
    explicit tx_unbond(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;

    tx_unbond& set_pub_key(const dev::bytes &pub_key);
    tx_unbond& set_coin(const char* coin);
    tx_unbond& set_value(const char* value);
    tx_unbond& set_value(const dev::bigdec18 &value);

    const dev::bytes& get_pub_key() const;
    std::string get_coin() const;
    dev::bigdec18 get_value() const;

protected:
    void decode_internal(dev::RLP rlp) override;

private:
    dev::bytes m_pub_key;
    std::string m_coin;
    dev::bigint m_value;
};

}

#endif //MINTEX_TX_UNBOND_H

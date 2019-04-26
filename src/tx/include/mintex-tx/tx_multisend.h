/*!
 * mintex.
 * tx_multisend.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_MULTISEND_H
#define MINTEX_TX_MULTISEND_H

#include <vector>
#include "tx_send_coin.h"
#include "tx_data.h"
namespace mintex {

    struct send_target {
        std::string coin;
        mintex::data::minter_address to;
        dev::bigint amount;
    };

class tx_multisend: public virtual mintex::tx_data {
public:
    explicit tx_multisend(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;

    tx_multisend& add_item(const char* coin, const mintex::data::minter_address &to, const char* amount);
    tx_multisend& add_item(const char* coin, const mintex::data::minter_address &to, const dev::bigdec18 &amount);

    const std::vector<mintex::send_target>& get_items() const;

protected:
    void decode_internal(dev::RLP rlp) override;

private:
    std::vector<send_target> m_items;
};

}

#endif //MINTEX_TX_MULTISEND_H

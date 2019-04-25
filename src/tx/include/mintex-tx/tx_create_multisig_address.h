/*!
 * mintex.
 * tx_create_multisig_address.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_CREATE_MULTISIG_ADDRESS_H
#define MINTEX_TX_CREATE_MULTISIG_ADDRESS_H

#include <vector>
#include "tx_data.h"
namespace mintex {

class tx_create_multisig_address: public mintex::tx_data {
public:
    explicit tx_create_multisig_address(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;
protected:
    void decode_internal(dev::RLP rlp) override;

private:
    dev::bigint m_threshold;
    std::vector<dev::bigint> m_weights;
    std::vector<mintex::data::minter_address> m_addresses;
};

}

#endif //MINTEX_TX_CREATE_MULTISIG_ADDRESS_H

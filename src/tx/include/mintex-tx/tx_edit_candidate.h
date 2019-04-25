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

namespace mintex {

class tx_edit_candidate: public virtual mintex::tx_data {
public:
    explicit tx_edit_candidate(std::shared_ptr<mintex::tx> tx);
    uint16_t type() const override;
    dev::bytes encode() override;
protected:
    void decode_internal(dev::RLP rlp) override;
};

}

#endif //MINTEX_TX_EDIT_CANDIDATE_H

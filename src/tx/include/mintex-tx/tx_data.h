/*!
 * mintex.
 * tx_data.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_DATA_H
#define MINTEX_TX_DATA_H

#include <memory>
#include "tx.h"

namespace mintex {

// Data for concrete transaction
class tx_data: public std::enable_shared_from_this<mintex::tx_data> {
public:
    tx_data() = default;
    explicit tx_data(std::shared_ptr<mintex::tx> tx) : m_tx(std::move(tx)) { }
    virtual ~tx_data() = default;
    virtual uint16_t type() const = 0;
    virtual dev::bytes encode() = 0;

    void decode(const char* hexEncoded) {
        minter::Data given(hexEncoded);
        decode_internal(dev::RLP(given.cget()));
    }
    void decode(const dev::bytes &data) {
        decode_internal(dev::RLP(data));
    }

    std::shared_ptr<mintex::tx> build() {
        tx()->m_data = encode();
        tx()->m_data_raw = shared_from_this();
        tx()->m_type = type();

        return tx();
    }

protected:
    virtual void decode_internal(dev::RLP rlp) {};

    std::shared_ptr<mintex::tx> tx() {
        return m_tx;
    }
private:
    std::shared_ptr<mintex::tx> m_tx;
};


}

#endif //MINTEX_TX_DATA_H
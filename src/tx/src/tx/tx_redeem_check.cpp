/*!
 * mintex.
 * tx_redeem_check.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_redeem_check.h"
#include "mintex-tx/tx_type.h"
mintex::tx_redeem_check::tx_redeem_check(std::shared_ptr<mintex::tx> tx)
        :tx_data(std::move(tx)) {

}

uint16_t mintex::tx_redeem_check::type() const {
    return mintex::tx_redeem_check_type::type();
}
dev::bytes mintex::tx_redeem_check::encode() {
    dev::RLPStream out;
    dev::RLPStream lst;
    {
        lst.append(m_check);
        lst.append(m_proof);
        out.appendList(lst);
    }

    return out.out();
}

void mintex::tx_redeem_check::decode_internal(dev::RLP rlp) {
    m_check = (dev::bytes)rlp[0];
    m_proof = (dev::bytes)rlp[1];
}

mintex::tx_redeem_check& mintex::tx_redeem_check::set_check(const dev::bytes& data) {
    m_check = data;
    return *this;
}
mintex::tx_redeem_check& mintex::tx_redeem_check::set_proof(const dev::bytes& data) {
    m_proof = data;
    return *this;
}
const dev::bytes mintex::tx_redeem_check::get_check() const {
    return m_check;
}
const dev::bytes mintex::tx_redeem_check::get_proof() const {
    return m_proof;
}


/*!
 * mintex.
 * signature_data.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <algorithm>
#include "mintex-tx/signature_data.h"
#include "mintex-tx/utils.h"

dev::bytes mintex::signature_single_data::encode() {
    dev::RLPStream txData;
    {
        dev::RLPStream txDataInner;
        txDataInner.append(m_v);
        txDataInner.append(m_r);
        txDataInner.append(m_s);
        txData.appendList(txDataInner);
    }

    return txData.out();
}

void mintex::signature_single_data::set_signature(const mintex::signature &sig) {
    m_r = sig.r;
    m_s = sig.s;
    m_v = sig.v;
}

void mintex::signature_single_data::set_signature(mintex::signature &&sig) {
    auto tmp = std::move(sig);

    m_r = std::move(sig.r);
    m_s = std::move(sig.s);
    m_v = std::move(sig.v);
}

void mintex::signature_single_data::set_signature(const uint8_t *data) {
    m_r.resize(32);
    m_s.resize(32);
    m_v.resize(1);

    m_r = dev::bytes(data + 00, data + 32);
    m_s = dev::bytes(data + 32, data + 64);
    m_v[0] = data[64];

}

void mintex::signature_single_data::set_signature(dev::bytes &&v, dev::bytes &&r, dev::bytes &&s) {
    m_r = std::move(r);
    m_s = std::move(s);
    m_v = std::move(v);
}

void mintex::signature_single_data::set_signature(dev::bytes &&data) {
    m_r.resize(32);
    m_s.resize(32);
    m_v.resize(1);

    std::move(data.begin() + 00, data.begin() + 32, m_r.begin());
    std::move(data.begin() + 32, data.begin() + 64, m_s.begin());
    m_v[0] = std::move(data[64]);
}

void mintex::signature_single_data::set_signature(const dev::bytes &data) {
    m_r.resize(32);
    m_s.resize(32);
    m_v.resize(1);

    std::copy(data.begin() + 00, data.begin() + 32, m_r.begin());
    std::copy(data.begin() + 32, data.begin() + 64, m_s.begin());
    m_v[0] = std::move(data[64]);
}

const dev::bytes &mintex::signature_single_data::get_v() const {
    return m_v;
}

const dev::bytes &mintex::signature_single_data::get_r() const {
    return m_r;
}

const dev::bytes &mintex::signature_single_data::get_s() const {
    return m_s;
}

void mintex::signature_single_data::decode(const dev::RLP &data) {
    m_v = (dev::bytes)data[0];
    m_r = (dev::bytes)data[1];
    m_s = (dev::bytes)data[2];
}


dev::bytes mintex::signature_multi_data::encode() {
    dev::RLPStream signList;
    for (const auto &item: m_signs) {
        dev::RLPStream signItem;
        signItem.append(item.get_v());
        signItem.append(item.get_r());
        signItem.append(item.get_s());

        signList.appendList(signItem);
    }

    dev::RLPStream out;
    out.append(m_address.get());
    out.appendList(signList);

    return out.out();
}

mintex::signature_multi_data &mintex::signature_multi_data::set_signatures(const mintex::data::minter_address &address,
                                                                           std::vector<mintex::signature_single_data> &&signs) {
    m_address = address;
    m_signs = std::move(signs);

    return *this;
}

void mintex::signature_multi_data::decode(const dev::RLP &data) {
    dev::RLPStream out;
//    m_address = (dev::b)
}

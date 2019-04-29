/*!
 * mintex.
 * tx_multisend.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_multisend.h"
#include "mintex-tx/tx_type.h"
#include "mintex-tx/utils.h"
mintex::tx_multisend::tx_multisend(std::shared_ptr<mintex::tx> tx) : tx_data(std::move(tx)) {

}
uint16_t mintex::tx_multisend::type() const {
    return mintex::tx_multisend_type::type;
}

#include <iostream>
dev::bytes mintex::tx_multisend::encode() {
    dev::RLPStream out;
    dev::RLPStream items_out;
    dev::RLPStream items;
    {
        for(const auto& item: m_items) {
            dev::RLPStream elements;
            elements.append(mintex::utils::to_bytes_fixed(item.coin, 10));
            elements.append(item.to.get());
            elements.append(item.amount);
            items.appendList(elements);
        }

        items_out.appendList(items);
    }
    out.appendList(items_out);

    return out.out();
}

void mintex::tx_multisend::decode_internal(dev::RLP rlp) {
    if(rlp.itemCount() == 0) {
        return;
    }

    m_items.reserve(rlp[0].itemCount());

    for(size_t i = 0; i < rlp[0].itemCount(); i++) {
        dev::RLP els = rlp[0][i];
        send_target t{
            mintex::utils::to_string_clear((dev::bytes)els[0]),
            (dev::bytes)els[1],
            (dev::bigint)els[2]
        };

        m_items.push_back(std::move(t));
    }
}

mintex::tx_multisend &
mintex::tx_multisend::add_item(const char *coin, const mintex::data::minter_address &to, const char *amount) {
    m_items.push_back(mintex::send_target{std::string(coin), to, mintex::utils::normalize_value(amount)});
    return *this;
}

mintex::tx_multisend &
mintex::tx_multisend::add_item(const char *coin, const mintex::data::minter_address &to, const dev::bigdec18 &amount) {
    m_items.push_back(mintex::send_target{std::string(coin), to, mintex::utils::normalize_value(amount)});
    return *this;
}

const std::vector<mintex::send_target> &mintex::tx_multisend::get_items() const {
    return m_items;
}

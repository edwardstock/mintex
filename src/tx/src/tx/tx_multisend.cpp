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
dev::bytes mintex::tx_multisend::encode() {
    dev::RLPStream out;
    dev::RLPStream lst;
    {
        for(const auto& item: m_items) {
            dev::RLPStream sub_list;
            sub_list.append(mintex::utils::to_bytes_fixed(item.coin));
            sub_list.append(item.to.get());
            sub_list.append(item.amount);
            lst.appendList(sub_list);
        }

        out.appendList(lst);
    }

    return out.out();
}

void mintex::tx_multisend::decode_internal(dev::RLP rlp) {
    tx_data::decode_internal(rlp);
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

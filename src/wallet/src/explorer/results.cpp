/*!
 * mintex.
 * results.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "wallet/explorer/results.hpp"

::nlohmann::json wallet::explorer::transaction_item::_data_encode(nlohmann::json &j,
                                                                  const ::wallet::explorer::transaction_item &resp) {
    return nlohmann::json();
}
void wallet::explorer::transaction_item::_data_decode(const nlohmann::json &j,
                                                      ::wallet::explorer::transaction_item &resp) {

    using namespace mintex;

    switch (resp.type) {
        case tx_type_val::send_coin: {
            tx_send_result result = j.at("data").get<tx_send_result>();
            resp.data = std::make_shared<tx_send_result>(std::move(result));
            break;
        }
        case tx_type_val::sell_coin:
        case tx_type_val::sell_all_coins:
        case tx_type_val::buy_coin: {
            tx_convert_result result = j.at("data").get<tx_convert_result>();
            resp.data = std::make_shared<tx_convert_result>(std::move(result));
            break;
        }
        case tx_type_val::create_coin: {
            tx_create_coin_result result = j.at("data").get<tx_create_coin_result>();
            resp.data = std::make_shared<tx_create_coin_result>(std::move(result));
            break;
        }
        case tx_type_val::declare_candidacy: {
            tx_declare_candidacy_result result = j.at("data").get<tx_declare_candidacy_result>();
            resp.data = std::make_shared<tx_declare_candidacy_result>(std::move(result));
            break;
        }
        case tx_type_val::delegate:
        case tx_type_val::unbond: {
            tx_delegate_unbond_result result = j.at("data").get<tx_delegate_unbond_result>();
            resp.data = std::make_shared<tx_delegate_unbond_result>(std::move(result));
            break;
        }
        case tx_type_val::redeem_check: {
            tx_redeem_check_result result = j.at("data").get<tx_redeem_check_result>();
            resp.data = std::make_shared<tx_redeem_check_result>(std::move(result));
            break;
        }
        case tx_type_val::set_candidate_on:
        case tx_type_val::set_candidate_off: {
            tx_set_candidate_on_off_result result = j.at("data").get<tx_set_candidate_on_off_result>();
            resp.data = std::make_shared<tx_set_candidate_on_off_result>(std::move(result));
            break;
        }
        case tx_type_val::create_multisig: {
            tx_create_multisig_result result = j.at("data").get<tx_create_multisig_result>();
            resp.data = std::make_shared<tx_create_multisig_result>(std::move(result));
            break;
        }
        case tx_type_val::multisend: {
            tx_multisend_result result = j.at("data").get<tx_multisend_result>();
            resp.data = std::make_shared<tx_multisend_result>(std::move(result));
            break;
        }
        case tx_type_val::edit_candidate: {
            tx_edit_candidate_result result = j.at("data").get<tx_edit_candidate_result>();
            resp.data = std::make_shared<tx_edit_candidate_result>(std::move(result));
            break;
        }
    }
}
 
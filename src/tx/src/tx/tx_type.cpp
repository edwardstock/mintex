/*!
 * mintex.
 * tx_type.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/tx_type.h"
#include "mintex-tx/tx_send_coin.h"
#include "mintex-tx/tx_sell_coin.h"
#include "mintex-tx/tx_sell_all_coins.h"
#include "mintex-tx/tx_buy_coin.h"
#include "mintex-tx/tx_create_coin.h"
#include "mintex-tx/tx_declare_candidacy.h"
#include "mintex-tx/tx_delegate.h"
#include "mintex-tx/tx_unbond.h"
#include "mintex-tx/tx_redeem_check.h"
#include "mintex-tx/tx_set_candidate_on_off.h"
#include "mintex-tx/tx_create_multisig_address.h"
#include "mintex-tx/tx_multisend.h"
#include "mintex-tx/tx_edit_candidate.h"

//define_tx_type_funcs(tx_send_coin)
std::shared_ptr<mintex::tx_send_coin> mintex::tx_type<mintex::tx_send_coin>::create(std::shared_ptr<mintex::tx> ptr,
                                                                                    const std::vector<uint8_t> &encodedData) {
    auto data = std::make_shared<mintex::tx_send_coin>(ptr);
    data->decode(encodedData);
    return data;
}
define_tx_type_funcs(tx_sell_coin)
define_tx_type_funcs(tx_sell_all_coins)
define_tx_type_funcs(tx_buy_coin)
define_tx_type_funcs(tx_create_coin)
define_tx_type_funcs(tx_declare_candidacy)
define_tx_type_funcs(tx_delegate)
define_tx_type_funcs(tx_unbond)
define_tx_type_funcs(tx_redeem_check)
define_tx_type_funcs(tx_set_candidate_on)
define_tx_type_funcs(tx_set_candidate_off)
define_tx_type_funcs(tx_create_multisig_address)
define_tx_type_funcs(tx_multisend)
define_tx_type_funcs(tx_edit_candidate)
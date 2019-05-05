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

const std::unordered_map<mintex::tx_type_val, std::string> mintex::tx_type_names = {
    {mintex::tx_type_val::send_coin,         "send"},
    {mintex::tx_type_val::sell_coin,         "sell"},
    {mintex::tx_type_val::sell_all_coins,    "sell all"},
    {mintex::tx_type_val::buy_coin,          "buy"},
    {mintex::tx_type_val::create_coin,       "create coin"},
    {mintex::tx_type_val::declare_candidacy, "declare candidacy"},
    {mintex::tx_type_val::delegate,          "delegate"},
    {mintex::tx_type_val::unbond,            "unbond"},
    {mintex::tx_type_val::redeem_check,      "redeem check"},
    {mintex::tx_type_val::set_candidate_on,  "set candidate on"},
    {mintex::tx_type_val::set_candidate_off, "set candidate off"},
    {mintex::tx_type_val::create_multisig,   "create multisig address"},
    {mintex::tx_type_val::multisend,         "multisend"},
    {mintex::tx_type_val::edit_candidate,    "edit candidate"},
};


define_tx_type_funcs(tx_send_coin)
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
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


define_tx_type_funcs(tx_send_coin, tx_type_val::send_coin, 10u)
define_tx_type_funcs(tx_sell_coin, tx_type_val::sell_coin, 100)
define_tx_type_funcs(tx_sell_all_coins, tx_type_val::sell_all_coins, 100)
define_tx_type_funcs(tx_buy_coin, tx_type_val::buy_coin, 100)
define_tx_type_funcs(tx_create_coin, tx_type_val::create_coin, 1000)
define_tx_type_funcs(tx_declare_candidacy, tx_type_val::declare_candidacy, 10000)
define_tx_type_funcs(tx_delegate, tx_type_val::delegate, 200)
define_tx_type_funcs(tx_unbond, tx_type_val::unbond, 200)
define_tx_type_funcs(tx_redeem_check, tx_type_val::redeem_check, 30)
define_tx_type_funcs(tx_set_candidate_on, tx_type_val::set_candidate_on, 100)
define_tx_type_funcs(tx_set_candidate_off, tx_type_val::set_candidate_off, 100)
define_tx_type_funcs(tx_create_multisig_address, tx_type_val::create_multisig, 100)
define_tx_type_funcs(tx_multisend, tx_type_val::multisend, 100)
define_tx_type_funcs(tx_edit_candidate, tx_type_val::edit_candidate, 10000)
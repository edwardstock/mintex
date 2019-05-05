/*!
 * mintex.
 * tx_type.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_TYPE_H
#define MINTEX_TX_TYPE_H

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <eth/Common.h>
#include <vector>
#include "mintex-tx/tx_fwd.h"
#include "mintex-tx/tx.h"

namespace mintex {

static constexpr double FEE_BASE = 0.001;

enum tx_type_val {
  send_coin = (uint8_t)0x01,
  sell_coin,
  sell_all_coins,
  buy_coin,
  create_coin,
  declare_candidacy,
  delegate,
  unbond,
  redeem_check,
  set_candidate_on,
  set_candidate_off,
  create_multisig,
  multisend,
  edit_candidate
};

extern const std::unordered_map<tx_type_val, std::string> tx_type_names;

template<class ref_type>
struct tx_type;

#define create_tx_type(_T, type_byte, fee) \
template<> struct tx_type<mintex::_T> { \
  static const uint16_t type = type_byte; \
  static constexpr const double fee_units = fee; \
  typedef mintex::_T ref_type; \
\
  static dev::bigdec18 get_fee() { \
      return dev::bigdec18(fee_units) * dev::bigdec18(FEE_BASE); \
  } \
  static std::shared_ptr<mintex::_T> create(std::shared_ptr<mintex::tx> ptr, const std::vector<uint8_t>& encodedData); \
}; \
using _T##_type = tx_type<mintex::_T>

#define define_tx_type_funcs(_T) \
    std::shared_ptr<mintex::_T> mintex::tx_type<mintex::_T>::create(std::shared_ptr<mintex::tx> ptr, const std::vector<uint8_t> &encodedData) { \
        auto data = std::make_shared<mintex::_T>(ptr); \
        data->decode(encodedData); \
        return data; \
    } \

create_tx_type(tx_send_coin, tx_type_val::send_coin, 10);
create_tx_type(tx_sell_coin, tx_type_val::sell_coin, 100);
create_tx_type(tx_sell_all_coins, tx_type_val::sell_all_coins, 100);
create_tx_type(tx_buy_coin, tx_type_val::buy_coin, 100);
create_tx_type(tx_create_coin, tx_type_val::create_coin, 1000);
create_tx_type(tx_declare_candidacy, tx_type_val::declare_candidacy, 10000);
create_tx_type(tx_delegate, tx_type_val::delegate, 200);
create_tx_type(tx_unbond, tx_type_val::unbond, 200);
create_tx_type(tx_redeem_check, tx_type_val::redeem_check, 30);
create_tx_type(tx_set_candidate_on, tx_type_val::set_candidate_on, 100);
create_tx_type(tx_set_candidate_off, tx_type_val::set_candidate_off, 100);
create_tx_type(tx_create_multisig_address, tx_type_val::create_multisig, 100);
create_tx_type(tx_multisend, tx_type_val::multisend, 100);
create_tx_type(tx_edit_candidate, tx_type_val::edit_candidate, 10000);

}

#endif //MINTEX_TX_TYPE_H

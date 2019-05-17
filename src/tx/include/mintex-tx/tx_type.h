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
  send_coin = (uint8_t) 0x01,
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

#define create_tx_type(_T) \
template<> struct tx_type<mintex::_T> { \
  typedef mintex::_T ref_type; \
\
  static const uint16_t type(); \
  static dev::bigdec18 get_fee(); \
  static dev::bigdec18 get_fee(const dev::bigint &gas); \
  static std::shared_ptr<mintex::_T> create(std::shared_ptr<mintex::tx> ptr, const std::vector<uint8_t>& encodedData); \
}; \
using _T##_type = tx_type<mintex::_T>

#define define_tx_type_funcs(_T, type_byte, fee) \
    std::shared_ptr<mintex::_T> mintex::tx_type<mintex::_T>::create(std::shared_ptr<mintex::tx> ptr, const std::vector<uint8_t> &encodedData) { \
        auto data = std::make_shared<mintex::_T>(ptr); \
        data->decode(encodedData); \
        return data; \
    } \
    dev::bigdec18 mintex::tx_type<mintex::_T>::get_fee() { \
        return dev::bigdec18(#fee) * dev::bigdec18(mintex::FEE_BASE); \
    } \
    dev::bigdec18 mintex::tx_type<mintex::_T>::get_fee(const dev::bigint &gas) { \
        return dev::bigdec18(gas) * get_fee(); \
    } \
    const uint16_t mintex::tx_type<mintex::_T>::type() { \
        return static_cast<uint16_t>(type_byte); \
    }

create_tx_type(tx_send_coin);
create_tx_type(tx_sell_coin);
create_tx_type(tx_sell_all_coins);
create_tx_type(tx_buy_coin);
create_tx_type(tx_create_coin);
create_tx_type(tx_declare_candidacy);
create_tx_type(tx_delegate);
create_tx_type(tx_unbond);
create_tx_type(tx_redeem_check);
create_tx_type(tx_set_candidate_on);
create_tx_type(tx_set_candidate_off);
create_tx_type(tx_create_multisig_address);
create_tx_type(tx_multisend);
create_tx_type(tx_edit_candidate);

}

#endif //MINTEX_TX_TYPE_H

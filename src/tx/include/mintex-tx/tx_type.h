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
#include <eth/Common.h>
#include <vector>
#include "mintex-tx/tx_fwd.h"
#include "mintex-tx/tx.h"

namespace mintex {

static constexpr double FEE_BASE = 0.001;

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

create_tx_type(tx_send_coin, 0x01, 10);
create_tx_type(tx_sell_coin, 0x02, 100);
create_tx_type(tx_sell_all_coins, 0x03, 100);
create_tx_type(tx_buy_coin, 0x04, 100);
create_tx_type(tx_create_coin, 0x05, 1000);
create_tx_type(tx_declare_candidacy, 0x06, 10000);
create_tx_type(tx_delegate, 0x07, 200);
create_tx_type(tx_unbond, 0x08, 200);
create_tx_type(tx_redeem_check, 0x09, 30);
create_tx_type(tx_set_candidate_on, 0x0A, 100);
create_tx_type(tx_set_candidate_off, 0x0B, 100);
create_tx_type(tx_create_multisig_address, 0x0C, 100);
create_tx_type(tx_multisend, 0x0D, 100);
create_tx_type(tx_edit_candidate, 0x0E, 10000);

}

#endif //MINTEX_TX_TYPE_H

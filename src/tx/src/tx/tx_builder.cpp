/*!
 * mintex.
 * tx_builder.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <memory>

#include "mintex-tx/tx_builder.h"
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
#include "mintex-tx/utils.h"

// Out of line
std::shared_ptr<mintex::tx_builder> mintex::new_tx() {
    auto out = std::make_shared<mintex::tx_builder>(std::make_shared<mintex::tx>());
    return out;
}

// setters
mintex::tx_builder &mintex::tx_builder::set_nonce(const dev::bigint &nonce) {
    m_tx->m_nonce = nonce;
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_nonce(const char *num) {
    return set_nonce(dev::bigint(num));
}

mintex::tx_builder &mintex::tx_builder::set_chain_id(uint8_t id) {
    m_tx->m_chain_id = id;
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_gas_price(const std::string &amount) {
    m_tx->m_gas_price = dev::bigint(amount);
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_gas_price(const dev::bigdec18 &amount) {
    m_tx->m_gas_price = dev::bigint(amount);
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_gas_price(const dev::bigint &amount) {
    m_tx->m_gas_price = amount;
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_gas_coin(const std::string &coin) {
    m_tx->m_gas_coin = coin;
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_gas_coin(const char *coin) {
    m_tx->m_gas_coin = std::string(coin);
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_payload(const dev::bytes &payload) {
    std::copy(payload.begin(), payload.end(), m_tx->m_payload.begin());
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_payload(dev::bytes &&payload) {
    m_tx->m_payload = std::move(payload);
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_payload(const std::string &payload) {
    m_tx->m_payload = mintex::utils::to_bytes(payload);
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_payload(std::string &&payload) {
    m_tx->m_payload = mintex::utils::to_bytes(std::move(payload));
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_payload(const char *payload) {
    return set_payload(std::string(payload));
}

mintex::tx_builder &mintex::tx_builder::set_service_data(const dev::bytes &payload) {
    std::copy(payload.begin(), payload.end(), m_tx->m_service_data.begin());
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_service_data(dev::bytes &&payload) {
    m_tx->m_service_data = std::move(payload);
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_service_data(const std::string &payload) {
    m_tx->m_service_data = std::move(mintex::utils::to_bytes(payload));
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_service_data(std::string &&payload) {
    m_tx->m_service_data = mintex::utils::to_bytes(std::move(payload));
    return *this;
}

mintex::tx_builder &mintex::tx_builder::set_service_data(const char *payload) {
    return set_service_data(std::string(payload));
}



// Detailed data builders
std::shared_ptr<mintex::tx_send_coin> mintex::tx_builder::tx_send_coin() {
    return std::make_shared<mintex::tx_send_coin>(m_tx);
}

std::shared_ptr<mintex::tx_sell_coin> mintex::tx_builder::tx_sell_coin() {
    return std::make_shared<mintex::tx_sell_coin>(m_tx);
}

std::shared_ptr<mintex::tx_sell_all_coins> mintex::tx_builder::tx_sell_all_coins() {
    return std::make_shared<mintex::tx_sell_all_coins>(m_tx);
}

std::shared_ptr<mintex::tx_buy_coin> mintex::tx_builder::tx_buy_coin() {
    return std::make_shared<mintex::tx_buy_coin>(m_tx);
}

std::shared_ptr<mintex::tx_create_coin> mintex::tx_builder::tx_create_coin() {
    return std::make_shared<mintex::tx_create_coin>(m_tx);
}

std::shared_ptr<mintex::tx_declare_candidacy> mintex::tx_builder::tx_declare_candidacy() {
    return std::make_shared<mintex::tx_declare_candidacy>(m_tx);
}

std::shared_ptr<mintex::tx_delegate> mintex::tx_builder::tx_delegate() {
    return std::make_shared<mintex::tx_delegate>(m_tx);
}

std::shared_ptr<mintex::tx_unbond> mintex::tx_builder::tx_unbound() {
    return std::make_shared<mintex::tx_unbond>(m_tx);
}

std::shared_ptr<mintex::tx_redeem_check> mintex::tx_builder::tx_redeem_check() {
    return std::make_shared<mintex::tx_redeem_check>(m_tx);
}

std::shared_ptr<mintex::tx_set_candidate_on> mintex::tx_builder::tx_set_candidate_on() {
    return std::make_shared<mintex::tx_set_candidate_on>(m_tx);
}

std::shared_ptr<mintex::tx_set_candidate_off> mintex::tx_builder::tx_set_candidate_off() {
    return std::make_shared<mintex::tx_set_candidate_off>(m_tx);
}

std::shared_ptr<mintex::tx_create_multisig_address> mintex::tx_builder::tx_create_multisig_address() {
    return std::make_shared<mintex::tx_create_multisig_address>(m_tx);
}

std::shared_ptr<mintex::tx_multisend> mintex::tx_builder::tx_multisend() {
    return std::make_shared<mintex::tx_multisend>(m_tx);
}

std::shared_ptr<mintex::tx_edit_candidate> mintex::tx_builder::tx_edit_candidate() {
    return std::make_shared<mintex::tx_edit_candidate>(m_tx);
}
/*!
 * mintex.
 * transaction.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <secp256k1/include/secp256k1.h>
#include <secp256k1/include/secp256k1_recovery.h>
#include <secp256k1/include/secp256k1_ecdh.h>
#include <mintex-tx/secp256k1_raii.h>


#include "mintex-tx/tx.h"
#include "mintex-tx/utils.h"
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
#include "mintex-tx/tx_type.h"

mintex::tx::tx():
 m_chain_id(dev::bigint(chain_id::testnet)),
 m_gas_price(dev::bigint("1")),
 m_gas_coin(mintex::utils::to_bytes_fixed("MNT", 10)),
 m_payload(dev::bytes(0)),
 m_service_data(dev::bytes(0)) {

}

std::shared_ptr<mintex::tx> mintex::tx::create() {
    return std::make_shared<mintex::tx>();
}

std::shared_ptr<mintex::tx> mintex::tx::decode(const dev::bytes &tx) {
    dev::RLP s(tx);

    if (s.itemCount() != 10) {
        throw std::runtime_error("Invalid RLP length: required 10 elements");
    }

    auto out = create();

    out->m_nonce = (dev::bigint) s[0];
    out->m_chain_id = (dev::bigint) s[1];
    out->m_gas_price = (dev::bigint) s[2];
    out->m_gas_coin = (dev::bytes) s[3];
    out->m_type = (dev::bigint) s[4];

    out->m_data = (dev::bytes) s[5];
    out->create_data_from_type();

    out->m_payload = (dev::bytes)s[6];
    out->m_service_data = (dev::bytes)s[7];
    out->m_signature_type = (dev::bigint) s[8];

    if(out->m_signature_type == mintex::signature_type::single) {
        out->m_signature = std::make_shared<mintex::signature_single_data>();
        out->m_signature->decode(s[9]);
    } else if(out->m_signature_type == mintex::signature_type::multi) {
        out->m_signature = std::make_shared<mintex::signature_multi_data>();
        out->m_signature->decode(s[9]);
    }

    return out;
}

std::shared_ptr<mintex::tx> mintex::tx::decode(const char *hexEncoded) {
    return decode(minter::Data(hexEncoded).cget());
}

/// \todo make static map, remove switch-case
void mintex::tx::create_data_from_type() {

    switch (get_type()) {
        case mintex::tx_send_coin_type::type:
            m_data_raw = mintex::tx_send_coin_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_sell_coin_type::type:
            m_data_raw = mintex::tx_sell_coin_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_sell_all_coins_type::type:
            m_data_raw = mintex::tx_sell_all_coins_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_buy_coin_type::type:
            m_data_raw = mintex::tx_buy_coin_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_create_coin_type::type:
            m_data_raw = mintex::tx_create_coin_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_declare_candidacy_type::type:
            m_data_raw = mintex::tx_declare_candidacy_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_delegate_type::type:
            m_data_raw = mintex::tx_delegate_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_unbond_type::type:
            m_data_raw = mintex::tx_unbond_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_redeem_check_type::type:
            m_data_raw = mintex::tx_redeem_check_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_set_candidate_on_type::type:
            m_data_raw = mintex::tx_set_candidate_on_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_set_candidate_off_type::type:
            m_data_raw = mintex::tx_set_candidate_off_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_create_multisig_address_type::type:
            m_data_raw = mintex::tx_create_multisig_address_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_multisend_type::type:
            m_data_raw = mintex::tx_multisend_type::create(shared_from_this(), get_data_raw());
            break;
        case mintex::tx_edit_candidate_type::type:
            m_data_raw = mintex::tx_edit_candidate_type::create(shared_from_this(), get_data_raw());
            break;
    }

}

minter::Data mintex::tx::sign_single(const mintex::minter_private_key &pk) {
    m_signature_type = mintex::signature_type::single;

    minter::Data raw_tx_data = minter::Data(encode(true));
    minter::Data32 hash(mintex::utils::sha3k(raw_tx_data));

    mintex::secp256k1_raii secp(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    auto sig = sign_with_private(secp, hash.cget(), pk.cget());

    if (!sig.success) {
        return minter::Data("0x0");
    }

    std::shared_ptr<mintex::signature_single_data> sig_data = std::make_shared<mintex::signature_single_data>();
    sig_data->set_signature(sig);
    m_signature = std::move(sig_data);

    return minter::Data(encode(false));
}

mintex::signature mintex::tx::sign_with_private(const mintex::secp256k1_raii &ctx,
                                                const dev::bytes &hash,
                                                const dev::bytes &pk) {
    secp256k1_ecdsa_recoverable_signature sig;

    int ret = secp256k1_ecdsa_sign_recoverable(ctx.get(), &sig, &hash[0], &pk[0], NULL, NULL);

    uint8_t outputSer[65];
    mintex::signature outSig;
    int recoveryId = 0;

    if (ret) {
        int serializeRet = secp256k1_ecdsa_recoverable_signature_serialize_compact(
            ctx.get(),
            outputSer,
            &recoveryId,
            &sig);
        outputSer[64] = ((uint8_t) recoveryId) + (uint8_t) 27;
    } else {
        return outSig;
    }

    uint8_t r[32], s[32], v[1];
    memcpy(r, outputSer + 0, 32);
    memcpy(s, outputSer + 32, 32);
    v[0] = outputSer[64];
    memset(outputSer, 0, 65);

    outSig.r = dev::bytes(r, r + 32);
    outSig.s = dev::bytes(s, s + 32);
    outSig.v = dev::bytes(v, v + 1);
    outSig.success = true;

    return outSig;
}

dev::bytes mintex::tx::encode(bool include_signature) {
    dev::RLPStream output;
    dev::RLPStream list;
    list.append(m_nonce);
    list.append(m_chain_id);
    list.append(m_gas_price);
    list.append(m_gas_coin);
    list.append(m_type);
    list.append(m_data);
    list.append(m_payload);
    list.append(m_service_data);
    list.append(m_signature_type);

    if (include_signature) {
        output.appendList(list);
        return output.out();
    }

    list.append(m_signature->encode());
    output.appendList(list);
    return output.out();
}

minter::Data mintex::tx::sign_multiple(const mintex::data::minter_address &address, const mintex::minter_private_key &pk) {
    m_signature_type = mintex::signature_type::multi;
    return minter::Data("0x0");
}


// GETTERS
dev::bigint mintex::tx::get_nonce() const {
    return m_nonce;
}

uint8_t mintex::tx::get_chain_id() const {
    return static_cast<uint8_t>(m_chain_id);
}

dev::bigint mintex::tx::get_gas_price() const {
    return m_gas_price;
}

std::string mintex::tx::get_gas_coin() const {
    const auto tmp = mintex::utils::to_string(m_gas_coin);
    return mintex::utils::strip_null_bytes(tmp.c_str());
}

uint16_t mintex::tx::get_type() const {
    return static_cast<uint16_t>(m_type);
}

const dev::bytes& mintex::tx::get_data_raw() const {
    return m_data;
}

const dev::bytes& mintex::tx::get_payload() const {
    return m_payload;
}

const dev::bytes & mintex::tx::get_service_data() const {
    return m_service_data;
}

uint8_t mintex::tx::get_signature_type() const {
    return static_cast<uint8_t>(m_signature_type);
}

/*
    tx &set_signature_type(uint8_t type);
 */

//void coin_name(const char* name, uint8_t *bts) {
//    size_t len = strlen(name);
//    memcpy(bts, name, len);
//    for (int i = len; i < 10; i++) {
//        bts[i] = (uint8_t)0x00;
//    }
//}
//
//int copy_address(const char* address, uint8_t *out) {
//    size_t len = strlen(address);
//    if (len != 40 && len != 42) {
//        return MINTER_ERR;
//    }
//
//    try {
//        auto add = minter::hexToBytes(address);
//        if (add.size() != 20) {
//            return MINTER_ERR;
//        }
//
//        memcpy(out, &add[0], 20);
//    } catch (const std::exception &e) {
//        return MINTER_ERR;
//    }
//
//    return MINTEX_OK;
//}
//
//struct tx_send_data tx_send_init(const char *coin, const char *address, const char *amount) {
//    struct tx_send_data out{};
//    coin_name(coin, out.coin);
//    copy_address(address, out.to);
//
//
//    readu256BE(&out.value);
//
//    return out;
//}
//
//void tx_send_free(struct tx_send_data* data) {
//    free(data->coin);
//}
//
//void tx_send_encode(struct tx_send_data *data, uint8_t *out) {
//
//}



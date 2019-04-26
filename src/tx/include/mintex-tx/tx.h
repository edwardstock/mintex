/*!
 * mintex.
 * transaction.hpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_H
#define MINTEX_TX_H

//#include <uint256/uint256.h>
#include <memory>
#include <secp256k1/include/secp256k1.h>
#include <secp256k1/include/secp256k1_ecdh.h>
#include <secp256k1/include/secp256k1_recovery.h>
#include <bip39/src/minter/utils.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <eth/RLP.h>
#include <eth/vector_ref.h>
#include "minter_address.h"
#include "minter_private_key.h"
#include "signature_data.h"
#include "signature.h"
#include "tx_fwd.h"
#include "mintex-tx/secp256k1_raii.h"

namespace mintex {

typedef enum {
  mainnet = (uint8_t) 0x01,
  testnet = (uint8_t) 0x02,
} chain_id;

class tx_builder;

class tx : public std::enable_shared_from_this<mintex::tx> {
    friend class tx_builder;
    friend class tx_data;
public:
    static std::shared_ptr<mintex::tx> create();
    static std::shared_ptr<mintex::tx> decode(const char *encodedHex);
    static std::shared_ptr<mintex::tx> decode(const dev::bytes &tx);
    /// \brief DON'T use it directly, otherwise bad_weak_ptr exception will threw
    tx();
    virtual ~tx() = default;

    dev::bigint get_nonce() const;
    uint8_t get_chain_id() const;
    dev::bigint get_gas_price() const;
    std::string get_gas_coin() const;
    uint16_t get_type() const;
    template<typename T>
    std::shared_ptr<T> get_data() const {
        return std::dynamic_pointer_cast<T>(m_data_raw);
    }
    const dev::bytes &get_data_raw() const;
    const dev::bytes &get_payload() const;
    const dev::bytes &get_service_data() const;
    uint8_t get_signature_type() const;
    template<typename T>
    std::shared_ptr<T> get_signature_data() const {
        return std::dynamic_pointer_cast<T>(m_signature);
    }

    minter::Data sign_single(const mintex::minter_private_key &pk);
    minter::Data sign_multiple(const mintex::data::minter_address &address, const mintex::minter_private_key &pk);

protected:
    dev::bytes encode(bool include_signature);
    mintex::signature sign_with_private(const mintex::secp256k1_raii &ctx,
                                        const dev::bytes &hash,
                                        const dev::bytes &pk);
    void create_data_from_type();

private:
    dev::bigint m_nonce;
    dev::bigint m_chain_id;
    dev::bigint m_gas_price;
    dev::bytes m_gas_coin;
    dev::bigint m_type;
    dev::bytes m_data;
    std::shared_ptr<mintex::tx_data> m_data_raw;
    dev::bytes m_payload;
    dev::bytes m_service_data;
    dev::bigint m_signature_type;
    std::shared_ptr<mintex::signature_data> m_signature;
};

}

#endif //MINTEX_TX_H

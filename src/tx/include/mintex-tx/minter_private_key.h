/*!
 * mintex.
 * minter_private_key.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <fmt/format.h>
#include <bip39/utils.h>
#include <bip39/HDKeyEncoder.h>
#include <secp256k1/include/secp256k1.h>
#include <secp256k1/include/secp256k1_recovery.h>
#include <secp256k1/include/secp256k1_ecdh.h>

#include "minter_public_key.h"
#include "mintex-tx/secp256k1_raii.h"

#ifndef MINTEX_MINTER_PRIVATE_KEY_H
#define MINTEX_MINTER_PRIVATE_KEY_H

namespace mintex {
namespace data {

class minter_private_key : public minter::FixedData<32> {
public:
    static minter_private_key from_mnemonic(const char *mnemonic, uint32_t derive_index = 0);

    minter_private_key();
    minter_private_key(const char *hexString);
    minter_private_key(const uint8_t *data);
    minter_private_key(std::vector<uint8_t> &&data);
    minter_private_key(const std::vector<uint8_t> &data);
    minter_private_key(const uint8_t *data, size_t len);
    mintex::pubkey_t get_public_key(bool compressed = false) const;

    bool operator==(const minter_private_key& other) const noexcept;
    bool operator!=(const minter_private_key& other) const noexcept;
};

} // data

using privkey_t = mintex::data::minter_private_key;
} // mintex

#endif //MINTEX_MINTER_PRIVATE_KEY_H

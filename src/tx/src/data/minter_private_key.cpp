/*!
 * mintex.
 * minter_private_key.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "mintex-tx/minter_private_key.h"

mintex::data::minter_private_key mintex::data::minter_private_key::from_mnemonic(const char *mnemonic,
                                                                                 uint32_t derive_index) {
    minter_private_key out;

    minter::Data64 seed = minter::HDKeyEncoder::makeBip39Seed(std::string(mnemonic));
    minter::HDKey root_key = minter::HDKeyEncoder::makeBip32RootKey(seed);
    minter::HDKey ext_key = minter::HDKeyEncoder::makeExtendedKey(root_key, fmt::format("m/44'/60'/0'/0/{0}", derive_index));

    std::copy(ext_key.privateKey.get().begin(), ext_key.privateKey.get().end(), out.get().begin());

    return out;
}
mintex::data::minter_private_key::minter_private_key() : FixedData() {
}
mintex::data::minter_private_key::minter_private_key(const char *hexString) : FixedData() {
    m_data = std::move(minter::hexToBytes(hexString));
}
mintex::data::minter_private_key::minter_private_key(const uint8_t *data) : FixedData(data) {

}
mintex::data::minter_private_key::minter_private_key(std::vector<uint8_t> &&data) : FixedData(data) {

}
mintex::data::minter_private_key::minter_private_key(const std::vector<uint8_t> &data) : FixedData(data) {

}
mintex::data::minter_private_key::minter_private_key(const uint8_t *data, size_t len) : FixedData(data, len) {

}
mintex::pubkey_t mintex::data::minter_private_key::get_public_key(bool compressed) const {
    secp256k1_raii secp;
    secp256k1_pubkey pubkey;
    uint8_t ints_array[2];

    auto ret = secp256k1_ec_pubkey_create(secp.get(), &pubkey, cdata());

    uint8_t output_ser[65];
    memset(output_ser, 0, 65);
    size_t output_len = 65;

    unsigned int compFlag =
        compressed == ((uint8_t) 1) ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED;

    if (ret) {
        int ret2 = secp256k1_ec_pubkey_serialize(secp.get(), output_ser, &output_len, &pubkey, compFlag);
    }

    ints_array[0] = output_len;
    ints_array[1] = ret;

    dev::bytes tmp(output_ser, output_ser + output_len);
    mintex::pubkey_t out(tmp);

    return out;
}

bool mintex::data::minter_private_key::operator==(const mintex::data::minter_private_key &other) const noexcept {
    return m_data == other.m_data;
}
bool mintex::data::minter_private_key::operator!=(const mintex::data::minter_private_key &other) const noexcept {
    return !(operator==(other));
}
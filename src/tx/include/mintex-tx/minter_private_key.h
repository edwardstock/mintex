/*!
 * mintex.
 * minter_private_key.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <minter/utils.h>
#include <minter/HDKeyEncoder.h>
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

    static minter_private_key from_mnemonic(const char *mnemonic) {
        minter_private_key out;

        minter::Data64 seed = minter::HDKeyEncoder::makeBip39Seed(std::string(mnemonic));
        minter::HDKey root_key = minter::HDKeyEncoder::makeBip32RootKey(seed);
        minter::HDKey ext_key = minter::HDKeyEncoder::makeExtendedKey(root_key, "m/44'/60'/0'/0/0");

        std::copy(ext_key.privateKey.cget().begin(), ext_key.privateKey.cget().end(), out.get().begin());

        return out;
    }

    minter_private_key() : FixedData() {
    }
    minter_private_key(const char *hexString) : FixedData() {
        m_data = std::move(minter::hexToBytes(hexString));
    }
    minter_private_key(const uint8_t *data) : FixedData(data) {

    }
    minter_private_key(std::vector<uint8_t> &&data) : FixedData(data) {

    }
    minter_private_key(const std::vector<uint8_t> &data) : FixedData(data) {

    }
    minter_private_key(const uint8_t *data, size_t len) : FixedData(data, len) {

    }

    mintex::pubkey_t get_public_key(bool compressed = false) const {
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

        //    intsarray[0] = outputLen;
        //    intsarray[1] = ret;
        //
        //    retArray = (env)->NewObjectArray(2, (env)->FindClass("[B"), (env)->NewByteArray(1));
        //
        //    pubkeyArray = (env)->NewByteArray(outputLen);
        //    (env)->SetByteArrayRegion(pubkeyArray, 0, outputLen, (jbyte *) outputSer);
        //    (env)->SetObjectArrayElement(retArray, 0, pubkeyArray);
        //
        //    intsByteArray = (env)->NewByteArray(2);
        //    (env)->SetByteArrayRegion(intsByteArray, 0, 2, (jbyte *) intsarray);
        //    (env)->SetObjectArrayElement(retArray, 1, intsByteArray);
        //
        //    return retArray;
    }
};

} // data

using privkey_t = mintex::data::minter_private_key;
} // mintex

#endif //MINTEX_MINTER_PRIVATE_KEY_H

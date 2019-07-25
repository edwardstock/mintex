/*!
 * mintex.
 * crypto.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <sodium.h>
#include "wallet/crypto.h"

std::string wallet::crypto::hash_argon2i(const std::string &data) {
    std::vector<char> hashed(crypto_pwhash_argon2i_STRBYTES);

    if (crypto_pwhash_argon2i_str
        (&hashed[0], data.c_str(), data.length(),
         crypto_pwhash_argon2i_OPSLIMIT_SENSITIVE,
         crypto_pwhash_argon2i_MEMLIMIT_INTERACTIVE) != 0) {
        throw std::runtime_error("Out of memory");
    }

    return minter::utils::to_string(hashed);
}

bool wallet::crypto::verify_argon2i(const std::string &data, const std::string &hash) {
    std::vector<char> hashed(hash.begin(), hash.end());

    if (crypto_pwhash_argon2i_str_verify
        (&hashed[0], data.c_str(), data.length()) != 0) {
        return false;
    }

    return true;
}
std::string wallet::crypto::encrypt_data(const minter::Data &key, const std::string &data) {
    minter::Data key_inner = minter::utils::sha3k(key.get());
    mlock_guard key_lock(key_inner);

    minter::Data output;
    Aes256::encrypt(key_inner.get(), minter::utils::to_bytes(data), output.get());
    return output.toHex();
}
std::string wallet::crypto::decrypt_data(const minter::Data &key, const std::string &data) {
    minter::Data key_inner = minter::utils::sha3k(key.get());
    mlock_guard key_lock(key_inner);
    minter::Data input(data.c_str());
    minter::Data decrypted;
    Aes256::decrypt(key_inner.get(), input.get(), decrypted.get());

    return minter::utils::to_string_clear(decrypted.get());
}
std::string wallet::crypto::encrypt_data_string(const std::string &key, const std::string &data) {
    minter::Data key_inner = minter::utils::sha3k(minter::Data(minter::utils::to_bytes(key)));
    minter::Data output;
    Aes256::encrypt(key_inner.get(), minter::utils::to_bytes(data), output.get());
    return output.toHex();
}
std::string wallet::crypto::decrypt_data_string(const std::string &key, const std::string &data) {
    minter::Data key_inner = minter::utils::sha3k(minter::Data(minter::utils::to_bytes(key)));
    minter::Data input(data.c_str());
    minter::Data decrypted;
    Aes256::decrypt(key_inner.get(), input.get(), decrypted.get());

    return minter::utils::to_string_clear(decrypted.get());
}

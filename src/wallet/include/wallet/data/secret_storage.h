/*!
 * mintex.
 * secret_storage.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_SECRET_STORAGE_H
#define MINTEX_SECRET_STORAGE_H

#include <string>

#include <memory>
#include <sodium.h>
#include <nlohmann/json.hpp>
#include <minter/bip39/HDKeyEncoder.h>
#include <minter/bip39/utils.h>
#include <minter/bip39/Bip39Mnemonic.h>
#include <minter/bip39/PCGRand.hpp>
#include <aes256.hpp>
#include <random>
#include <vector>
#include <rang.hpp>
#include <toolboxpp.hpp>
#include <minter/tx.hpp>
#include <minter/tx/utils.h>
#include <minter/private_key.h>
#include <minter/public_key.h>
#include <minter/address.h>

#include "wallet/kv_storage.h"
#include "wallet/settings.h"
#include "wallet/crypto.h"

namespace wallet {

struct secret_data {
  std::string mnemonic;
  minter::privkey_t priv_key;
  uint32_t derive_index = 0;

  static secret_data from_mnemonic(const std::string &mnemonic, uint32_t derive_index = 0) {
      secret_data sd;
      sd.mnemonic = std::move(mnemonic);
      sd.priv_key = minter::privkey_t::from_mnemonic(sd.mnemonic.c_str(), derive_index);
      sd.derive_index = derive_index;

      return sd;
  }

  minter::pubkey_t get_pub_key(bool compressed = false) const {
      return priv_key.get_public_key(compressed);
  }

  minter::address_t get_address() const {
      return minter::address_t(get_pub_key());
  }

  void cleanup() {
      mnemonic = std::string("");
      priv_key.clear();
  }

  minter::Data sign(std::shared_ptr<minter::tx> tx) {
      return tx->sign_single(priv_key);
  }

};

void from_json(const nlohmann::json &j, secret_data &data);
void to_json(nlohmann::json &j, const secret_data &data);

class secret_storage {
 private:
    static constexpr const char *KEY_SECRETS = "secret_storage_mnemonic_secret_list";
    static constexpr const char *KEY_ADDRESSES = "secret_storage_addresses_list";
 public:
    static std::unique_ptr<secret_storage> create(bool encrypted, const std::shared_ptr<wallet::settings> sets);
    secret_storage();
    secret_storage(wallet::db::kv_storage &&storage);
    ~secret_storage();

    // init without encryption
    void init();
    void init(const std::string &pass);
    static secret_data generate(uint32_t derive_index = 0);

    void add(const secret_data &data);
    void remove(const secret_data &data);
    void remove(size_t index);
    void purge();
    std::vector<secret_data> get_secrets() const;
    secret_data get_secret(size_t index) const;
    std::vector<minter::address_t> get_addresses() const;
    minter::address_t get_address(size_t index) const;

 private:
    std::string encrypt_data(const std::string &decrypted);
    std::string decrypt_data(const std::string &encrypted);

    wallet::crypto::mlock_guard m_pass_mlock;
    minter::Data m_passkey;
    wallet::db::kv_storage m_storage;
};

}

#endif //MINTEX_SECRET_STORAGE_H

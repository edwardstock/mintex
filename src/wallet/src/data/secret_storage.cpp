/*!
 * mintex.
 * secret_storage
 *
 * \date 06.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <minter/bip39/utils.h>
#include "wallet/settings.h"
#include "wallet/resp_gen.h"
#include "wallet/crypto.h"
#include "wallet/data/secret_storage.h"
#include "wallet/except.h"
#include "wallet/term.h"

void wallet::from_json(const nlohmann::json &j, wallet::secret_data &data) {
    JSON_GET(j, "mnemonic", std::string, data.mnemonic);
    JSON_GET(j, "priv_key", minter::privkey_t, data.priv_key);
    JSON_GET(j, "derive_index", uint32_t, data.derive_index);
}

void wallet::to_json(nlohmann::json &j, const wallet::secret_data &data) {
    j = nlohmann::json{
        {"mnemonic",     data.mnemonic},
        {"priv_key",     data.priv_key},
        {"derive_index", data.derive_index},
    };
}

wallet::secret_storage::secret_storage() :
    m_storage() {
}
wallet::secret_storage::secret_storage(wallet::db::kv_storage &&storage) :
    m_storage(std::move(storage)) {

}
wallet::secret_storage::~secret_storage() {
    size_t sz = m_passkey.size();
    uint8_t *ptr = m_passkey.data();
    m_passkey.clear();
    sodium_munlock(ptr, sz);
}

void wallet::secret_storage::init() {
    auto status = m_storage.open();
    if (!status.ok()) {
        throw std::runtime_error(status.ToString());
    }
}
void wallet::secret_storage::init(const std::string &pass) {

    m_passkey = minter::Data(minter::utils::to_bytes(pass));
    m_pass_mlock = wallet::crypto::mlock_guard(m_passkey);

    m_storage.add_keybased_interceptor(KEY_SECRETS, {
        std::bind(&secret_storage::decrypt_data, this, std::placeholders::_1),
        std::bind(&secret_storage::encrypt_data, this, std::placeholders::_1)
    });

    auto status = m_storage.open();
    if (!status.ok()) {
        throw std::runtime_error(status.ToString());
    }
}

wallet::secret_data wallet::secret_storage::generate(uint32_t derive_index) {
    secret_data data;
    auto mnemonic = minter::Bip39Mnemonic::generate();
    data.mnemonic = mnemonic.raw;
    data.priv_key = minter::privkey_t::from_mnemonic(data.mnemonic.c_str(), derive_index);
    data.derive_index = derive_index;

    return data;
}
std::string wallet::secret_storage::encrypt_data(const std::string &decrypted) {
    return wallet::crypto::encrypt_data(m_passkey, decrypted);
}
std::string wallet::secret_storage::decrypt_data(const std::string &encrypted) {
    return wallet::crypto::decrypt_data(m_passkey, encrypted);
}

void wallet::secret_storage::add(const wallet::secret_data &data) {
    auto secrets = get_secrets();
    auto addresses = get_addresses();
    for (const auto &item: secrets) {
        if (item.priv_key == data.priv_key) {
            throw std::runtime_error(fmt::format("This private {0} already exists",
                                                 minter::address_t(data.priv_key).to_string()));
        }
    }

    secrets.push_back(data);
    addresses.push_back(data.get_address());

    {
        nlohmann::json j = secrets;
        std::stringstream ss;
        ss << j;
        leveldb::Status status = m_storage.set(KEY_SECRETS, ss.str());
        if (!status.ok()) {
            throw std::runtime_error(status.ToString());
        }
    }

    {
        nlohmann::json j = addresses;
        std::stringstream ss;
        ss << j;
        leveldb::Status status = m_storage.set(KEY_ADDRESSES, ss.str());
        if (!status.ok()) {
            throw std::runtime_error(status.ToString());
        }
    }
}

void wallet::secret_storage::remove(const wallet::secret_data &data) {
    auto secrets = get_secrets();

    bool found = false;
    size_t idx = 0;
    for (const auto &item: secrets) {
        if (item.priv_key == data.priv_key) {
            found = true;
            break;
        }
        idx++;
    }
    if (!found) {
        return;
    }

    remove(idx);
}

void wallet::secret_storage::remove(size_t index) {
    auto secrets = get_secrets();
    auto addresses = get_addresses();

    if (index < 0 || (index > (secrets.size() - 1))) {
        return;
    }

    secrets.erase(secrets.begin() + index);
    addresses.erase(addresses.begin() + index);

    {
        nlohmann::json jsecrets = secrets;
        std::stringstream ss;
        ss << jsecrets;
        leveldb::Status status = m_storage.set(KEY_SECRETS, ss.str());
        if (!status.ok()) {
            throw std::runtime_error(status.ToString());
        }
    }

    {
        nlohmann::json jaddresses = addresses;
        std::stringstream ss;
        ss << jaddresses;
        leveldb::Status status = m_storage.set(KEY_ADDRESSES, ss.str());
        if (!status.ok()) {
            throw std::runtime_error(status.ToString());
        }
    }
}

void wallet::secret_storage::purge() {
    auto sec_rem_status = m_storage.remove(KEY_SECRETS);
    if (!sec_rem_status.ok()) {
        throw new std::runtime_error(sec_rem_status.ToString());
    }

    auto add_rem_status = m_storage.remove(KEY_ADDRESSES);
    if (!add_rem_status.ok()) {
        throw new std::runtime_error(add_rem_status.ToString());
    }
}

std::vector<wallet::secret_data> wallet::secret_storage::get_secrets() const {
    leveldb::Status status;
    std::string res = m_storage.get(KEY_SECRETS, &status);
    std::vector<secret_data> data;
    if (status.ok() && !res.empty()) {
        auto j = nlohmann::json::parse(res);
        j.get_to<std::vector<secret_data>>(data);
    }

    return data;
}

wallet::secret_data wallet::secret_storage::get_secret(size_t index) const {
    auto secrets = get_secrets();
    if (index < 0 || index > (secrets.size() - 1)) {
        throw wallet::out_of_bound_error("Invalid account index");
    }

    return secrets[index];
}

std::vector<minter::address_t> wallet::secret_storage::get_addresses() const {
    leveldb::Status status;
    std::string res = m_storage.get(KEY_ADDRESSES, &status);
    std::vector<minter::address_t> data;
    if (status.ok() && !res.empty()) {
        auto j = nlohmann::json::parse(res);
        j.get_to<std::vector<minter::address_t>>(data);
    }

    return data;
}

minter::address_t wallet::secret_storage::get_address(size_t index) const {
    auto addresses = get_addresses();
    if (index < 0 || index > (addresses.size() - 1)) {
        throw wallet::out_of_bound_error("Invalid account index");
    }

    return addresses[index];
}

std::unique_ptr<wallet::secret_storage> wallet::secret_storage::create(bool encrypted,
                                                                       const std::shared_ptr<wallet::settings> sets) {
    std::unique_ptr<secret_storage> res = std::make_unique<secret_storage>();
    if (!encrypted || !sets->pwd_secured) {
        res->init();
        return res;
    }

    std::string pw;
    if (sets->has_saved_pass()) {
        pw = sets->passkey;
    }

    pw = toolboxpp::console::promptPassword("Please, type your password to work with encrypted data", 32);
    std::cout << std::endl;

    while (!wallet::crypto::verify_argon2i(pw, sets->passhash)) {
        wallet::term::print_colored_message("Password incorrect", rang::fg::black, rang::bg::red);
        std::cout << std::endl;
        pw = toolboxpp::console::promptPassword("Please, type your password to work with encrypted data", 32);
    }
    std::cout << std::endl;

    res->init(pw);
    return res;
}

/*!
 * mintex.
 * simple_storage.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "wallet/data/simple_storage.h"

const wallet::db::kv_storage &wallet::simple_storage::get_storage() const {
    return m_storage;
}

void wallet::simple_storage::init() {
    auto status = m_storage.open();
    if (!status.ok()) {
        throw std::runtime_error(status.ToString());
    }
}
void wallet::simple_storage::set(const std::string &name, const nlohmann::json &j) {
    std::stringstream ss;
    ss << j;
    leveldb::Status status = m_storage.set(name, ss.str());
    if (!status.ok()) {
        throw std::runtime_error(status.ToString());
    }
}
void wallet::simple_storage::remove(const std::string &name) {
    leveldb::Status status = m_storage.remove(name);
    if (!status.ok()) {
        throw std::runtime_error(status.ToString());
    }
}

bool wallet::simple_storage::exists(const std::string &name) const {
    leveldb::Iterator* it = get_storage().get_db()->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        if(it->key().ToString() == name) {
            return true;
        }
    }
    assert(it->status().ok());  // Check for any errors found during the scan
    return false;
}

nlohmann::json wallet::simple_storage::get(const std::string &name) const {
    leveldb::Status status;
    std::string res = m_storage.get(name, &status);
    nlohmann::json j = nlohmann::json::parse(res);
    if (!status.ok()) {
        throw std::runtime_error(status.ToString());
    }

    return j;
}
wallet::simple_storage::simple_storage(wallet::db::kv_storage &&storage) : m_storage(std::move(storage)) {}

void wallet::from_json(const nlohmann::json &j, wallet::validator_data &v) {
    JSON_GET(j, "name", std::string, v.name);
    JSON_GET(j, "pubkey", minter::pubkey_t , v.pubkey);
}
void wallet::to_json(nlohmann::json &j, const wallet::validator_data &v) {
    j = nlohmann::json {
        {"name", v.name},
        {"pubkey", v.pubkey.to_string()}
    };
}

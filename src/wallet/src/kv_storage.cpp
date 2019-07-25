/*!
 * mintex.
 * kv_storage.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "wallet/kv_storage.h"

wallet::db::kv_storage::kv_storage() :
    m_is_valid(false),
    m_path(std::string(getenv("HOME"))+"/.mintex/"+wallet::settings::DB_FILE),
    m_rw_interceptors(0) {
    m_write_opts.sync = true;
    m_read_opts.fill_cache = true;
}
wallet::db::kv_storage::kv_storage(const std::string &path, const leveldb::Options &opts) :
    m_is_valid(false),
    m_path(path),
    m_opts(opts),
    m_rw_interceptors(0) {
    m_write_opts.sync = true;
    m_read_opts.fill_cache = true;
}
wallet::db::kv_storage::~kv_storage() {
    if(m_is_valid) {
        delete m_db;
    }
}
wallet::db::kv_storage *wallet::db::kv_storage::add_interceptor(wallet::db::rw_interceptor &&interceptor) {
    m_rw_interceptors.push_back(std::move(interceptor));
    return this;
}
wallet::db::kv_storage *wallet::db::kv_storage::add_keybased_interceptor(const std::string &key,
                                                                         wallet::db::rw_interceptor &&interceptor) {
    m_rw_interceptors_map.emplace(key, std::move(interceptor));
    return this;
}
wallet::db::kv_storage *wallet::db::kv_storage::add_read_interceptor(wallet::db::interceptor &&interceptor) {
    m_rw_interceptors.push_back(rw_interceptor{std::move(interceptor), nullptr});
    return this;
}
wallet::db::kv_storage *wallet::db::kv_storage::add_write_interceptor(wallet::db::interceptor &&interceptor) {
    m_rw_interceptors.push_back(rw_interceptor{nullptr, std::move(interceptor)});
    return this;
}
leveldb::ReadOptions &wallet::db::kv_storage::get_read_options() {
    return m_read_opts;
}
leveldb::WriteOptions &wallet::db::kv_storage::get_write_options() {
    return m_write_opts;
}
leveldb::Status wallet::db::kv_storage::open() {
    if(is_valid_state()) {
        return leveldb::Status::OK();
    }

    namespace fs = boost::filesystem;

    bool created = false;
    fs::path p(m_path);
    if(!fs::exists(p.parent_path())) {
        fs::create_directories(p.parent_path());
        created = true;
    }

    m_opts.create_if_missing = true;
    auto status = leveldb::DB::Open(m_opts, m_path, &m_db);
    if(status.ok()) {
        m_is_valid = true;
        if(created) {
            fs::permissions(p, fs::owner_read | fs::owner_write);
        }
    }
    return status;
}
std::string wallet::db::kv_storage::get(const leveldb::Slice &key, leveldb::Status *result) const {
    if(!is_valid_state()) {
        *result = leveldb::Status::IOError("Invalid DB State", "Database not opened");
        return std::string();
    }
    std::string out;
    auto status = m_db->Get(m_read_opts, key, &out);
    if (!status.ok()) {
        *result = status;
        return std::string();
    }

    for (const auto &item: m_rw_interceptors) {
        if (item.on_read) {
            out = item.on_read(out);
        }
    }
    for(const auto &item: m_rw_interceptors_map) {
        if(toolboxpp::strings::equalsIgnoreCase(item.first, key.ToString())) {
            out = item.second.on_read(out);
        }
    }

    return out;
}
leveldb::Status wallet::db::kv_storage::set(const leveldb::Slice &key, const std::string value) {
    if(!is_valid_state()) {
        return leveldb::Status::IOError("Invalid DB State", "Database not opened");
    }
    // prevent origin modifying
    auto copy = value;
    for (const auto &item: m_rw_interceptors) {
        if (item.on_write) {
            copy = item.on_write(copy);
        }
    }
    for(const auto &item: m_rw_interceptors_map) {
        if(toolboxpp::strings::equalsIgnoreCase(item.first, key.ToString())) {
            copy = item.second.on_write(copy);
        }
    }
    return m_db->Put(m_write_opts, key, copy);
}
bool wallet::db::kv_storage::exists(const std::string &key) {
    if(!is_valid_state()) {
        return false;
    }
    leveldb::Status status;
    auto res = get(key, &status);
    return !(status.IsNotFound());
}
leveldb::Status wallet::db::kv_storage::remove(const leveldb::Slice &key) {
    if(!is_valid_state()) {
        return leveldb::Status::IOError("Invalid DB State", "Database not opened");
    }
    return m_db->Delete(m_write_opts, key);
}
bool wallet::db::kv_storage::is_valid_state() const {
    return m_is_valid;
}
leveldb::DB *wallet::db::kv_storage::get_db() const {
    return m_db;
}

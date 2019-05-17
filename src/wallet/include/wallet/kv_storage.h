/*!
 * mintex.
 * kv_storage.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_KV_STORAGE_H
#define MINTEX_KV_STORAGE_H

#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <cstdlib>

#include <boost/filesystem.hpp>

#include <fmt/format.h>
#include <leveldb/db.h>
#include <leveldb/env.h>
#include <wallet/settings.h>
#include <toolboxpp.hpp>

namespace wallet {
namespace db {

using interceptor = std::function<std::string(const std::string &value)>;

struct rw_interceptor {
  interceptor on_read;
  interceptor on_write;
};

class kv_storage {
 public:
    kv_storage() :
        m_is_valid(false),
        m_path(std::string(getenv("HOME"))+"/.mintex/"+wallet::settings::DB_FILE),
        m_rw_interceptors(0) {
        m_write_opts.sync = true;
        m_read_opts.fill_cache = true;
    }

    kv_storage(const std::string &path, const leveldb::Options &opts) :
        m_is_valid(false),
        m_path(path),
        m_opts(opts),
        m_rw_interceptors(0) {
        m_write_opts.sync = true;
        m_read_opts.fill_cache = true;
    }

    ~kv_storage() {
        delete m_db;
    }

    kv_storage *add_interceptor(rw_interceptor &&interceptor) {
        m_rw_interceptors.push_back(std::move(interceptor));
        return this;
    }

    kv_storage *add_keybased_interceptor(const std::string &key, rw_interceptor &&interceptor) {
        m_rw_interceptors_map.emplace(key, std::move(interceptor));
        return this;
    }

    kv_storage *add_read_interceptor(interceptor &&interceptor) {
        m_rw_interceptors.push_back(rw_interceptor{std::move(interceptor), nullptr});
        return this;
    }

    kv_storage *add_write_interceptor(interceptor &&interceptor) {
        m_rw_interceptors.push_back(rw_interceptor{nullptr, std::move(interceptor)});
        return this;
    }

    leveldb::ReadOptions &get_read_options() {
        return m_read_opts;
    }

    leveldb::WriteOptions &get_write_options() {
        return m_write_opts;
    }

    leveldb::Status open() {
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

    std::string get(const leveldb::Slice &key, leveldb::Status *result = nullptr) const {
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

    leveldb::Status set(const leveldb::Slice &key, const std::string value) {
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

    bool exists(const std::string &key) {
        if(!is_valid_state()) {
            return false;
        }
        leveldb::Status status;
        auto res = get(key, &status);
        return !(status.IsNotFound());
    }

    leveldb::Status remove(const leveldb::Slice &key) {
        if(!is_valid_state()) {
            return leveldb::Status::IOError("Invalid DB State", "Database not opened");
        }
        return m_db->Delete(m_write_opts, key);
    }

    bool is_valid_state() const {
        return m_is_valid;
    }

 private:
    bool m_is_valid;
    std::string m_path;
    leveldb::Options m_opts;
    std::vector<wallet::db::rw_interceptor> m_rw_interceptors;
    std::unordered_map<std::string, wallet::db::rw_interceptor> m_rw_interceptors_map;

    leveldb::WriteOptions m_write_opts;
    leveldb::ReadOptions m_read_opts;
    leveldb::DB *m_db;
};

}
}

#endif //MINTEX_KV_STORAGE_H

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
    kv_storage();
    kv_storage(const std::string &path, const leveldb::Options &opts);
    ~kv_storage();

    kv_storage *add_interceptor(rw_interceptor &&interceptor);
    kv_storage *add_keybased_interceptor(const std::string &key, rw_interceptor &&interceptor);
    kv_storage *add_read_interceptor(interceptor &&interceptor);
    kv_storage *add_write_interceptor(interceptor &&interceptor);

    leveldb::ReadOptions &get_read_options();
    leveldb::WriteOptions &get_write_options();

    leveldb::Status open();

    std::string get(const leveldb::Slice &key, leveldb::Status *result = nullptr) const;
    leveldb::Status set(const leveldb::Slice &key, const std::string value);
    bool exists(const std::string &key);
    leveldb::Status remove(const leveldb::Slice &key);

    bool is_valid_state() const;
    leveldb::DB * get_db() const;

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

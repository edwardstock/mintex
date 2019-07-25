/*!
 * mintex.
 * address_book.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "wallet/data/address_book.h"

wallet::address_book::address_book(wallet::db::kv_storage &&storage) :
    m_storage(std::move(storage)) {
}
wallet::address_book::address_book() :
    m_storage() {
}
void wallet::address_book::set(const wallet::book_record &record) {
    nlohmann::json j = record;
    auto key = get_key(record.name);
    m_storage.set(key, j);
}
wallet::book_record wallet::address_book::get(const std::string &key) const {
    book_record r = m_storage.get(get_key(key));
    return r;
}
std::string wallet::address_book::get_key(const std::string &suffix) const {
    return fmt::format("address_book::record::{0}", suffix);
}

void wallet::address_book::remove() {

}
bool wallet::address_book::exists(const std::string &key) const {
    return false;
}

void wallet::to_json(nlohmann::json &j, const wallet::book_record &r) {
    j = nlohmann::json{
        {"type",  r.type},
        {"name",  r.name},
        {"value", r.value},
    };
}
void wallet::from_json(const nlohmann::json &j, wallet::book_record &r) {
    j.at("type").get_to<wallet::record_type>(r.type);
    j.at("name").get_to<std::string>(r.name);
    j.at("value").get_to<std::string>(r.value);
}

/*!
 * mintex.
 * address_book.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_ADDRESS_BOOK_H
#define MINTEX_ADDRESS_BOOK_H

#include <string>
#include <memory>
#include <fmt/format.h>
#include "wallet/resp_gen.h"
#include "wallet/data/simple_storage.h"

namespace wallet {

enum record_type {
  wallet,
  validator,
};

struct book_record {
  record_type type;
  std::string name;
  std::string value;
};

NLOHMANN_JSON_SERIALIZE_ENUM(wallet::record_type, {
    { wallet::record_type::wallet, "wallet" },
    { wallet::record_type::validator, "validator" },
})

void from_json(const nlohmann::json &j, wallet::book_record &r);
void to_json(nlohmann::json &j, const wallet::book_record &r);

class address_book {
 public:
    static std::unique_ptr<address_book> create(wallet::db::kv_storage &&storage) {
        std::unique_ptr<address_book> out = std::make_unique<address_book>(std::move(storage));
        out->m_storage.init();

        return out;
    }
    static std::unique_ptr<address_book> create() {
        std::unique_ptr<address_book> out = std::make_unique<address_book>();
        out->m_storage.init();

        return out;
    }

    address_book(wallet::db::kv_storage &&storage);
    address_book();
    ~address_book() = default;

    void set(const book_record &record);
    book_record get(const std::string &key) const;
    void remove();
    bool exists(const std::string &key) const;

 protected:
    std::string get_key(const std::string &suffix) const;

 private:
    wallet::simple_storage m_storage;
};

}

#endif //MINTEX_ADDRESS_BOOK_H

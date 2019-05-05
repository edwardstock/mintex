/*!
 * mintex.
 * minter_hash.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_MINTER_HASH_H
#define MINTEX_MINTER_HASH_H

#include <bip39/utils.h>
#include <eth/Common.h>

namespace mintex {
namespace data {

class minter_hash {
private:
    using data_t = minter::Data;
public:
public:
    minter_hash() = default;
    minter_hash(const char* hex);
    minter_hash(const std::string& hex);
    minter_hash(const std::vector<uint8_t> &data);
    minter_hash(std::vector<uint8_t> &&data);

    bool operator==(const mintex::data::minter_hash& other) const noexcept ;
    bool operator==(const minter::Data& other) const noexcept ;
    bool operator!=(const mintex::data::minter_hash& other) const noexcept ;
    bool operator!=(const minter::Data& other) const noexcept ;
    uint8_t operator[](size_t element) const noexcept;

    uint8_t at(size_t idx) const;

    const dev::bytes& get() const;
    dev::bytes& get();

    explicit operator const std::vector<uint8_t>&() const;
    explicit operator std::string() const;

    std::string to_string() const;
    std::string to_string_no_prefix() const;

private:
    data_t m_data;
};

}

using hash_t = mintex::data::minter_hash;

}

std::ostream& operator << (std::ostream &os, const mintex::hash_t &hash);

#endif //MINTEX_MINTER_HASH_H

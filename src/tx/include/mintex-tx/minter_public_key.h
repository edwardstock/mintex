/*!
 * mintex.
 * minter_public_key.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_MINTER_PUBLIC_KEY_H
#define MINTEX_MINTER_PUBLIC_KEY_H

#include <minter/utils.h>
#include <eth/Common.h>
#include <vector>

namespace mintex{
namespace data{

class minter_public_key {
private:
    using data_t = minter::Data;
public:
    minter_public_key() = default;
    minter_public_key(const char* hexString);
    minter_public_key(const std::vector<uint8_t> &data);
    minter_public_key(std::vector<uint8_t> &&data);

    bool operator==(const mintex::data::minter_public_key& other) const noexcept ;
    bool operator==(const minter::Data& other) const noexcept ;
    bool operator!=(const mintex::data::minter_public_key& other) const noexcept ;
    bool operator!=(const minter::Data& other) const noexcept ;
    uint8_t operator[](size_t element) const noexcept;

    uint8_t at(size_t idx) const;

    const dev::bytes& get() const;
    dev::bytes& get();

    explicit operator const std::vector<uint8_t>&() const;
    explicit operator std::string() const;

private:
    data_t m_data;
};

}

using pubkey_t = mintex::data::minter_public_key;
}



#endif //MINTEX_MINTER_PUBLIC_KEY_H

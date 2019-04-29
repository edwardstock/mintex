/*!
 * mintex.
 * minter_address.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_MINTER_ADDRESS_H
#define MINTEX_MINTER_ADDRESS_H

#include <minter/utils.h>
#include <vector>
#include <cstdint>
#include "mintex-tx/minter_public_key.h"
#include "mintex-tx/minter_private_key.h"

namespace mintex {

namespace data {
using address_data = minter::FixedData<20>;

class minter_address {
public:
    minter_address() = default;
    minter_address(const char *hex);
    minter_address(const std::vector<uint8_t> &data);
    minter_address(std::vector<uint8_t> &&data);
    minter_address(const mintex::pubkey_t &pub_key);
    minter_address(const mintex::privkey_t &priv_key);
    const std::string to_string();
    const std::string to_string_no_prefix();

    const uint8_t *data() const;
    uint8_t *data();
    const std::vector<uint8_t> &get() const;
    std::vector<uint8_t> &get();

    bool operator==(const mintex::data::minter_address& other) const noexcept ;
    bool operator==(const minter::Data& other) const noexcept ;
    bool operator!=(const mintex::data::minter_address& other) const noexcept ;
    bool operator!=(const minter::Data& other) const noexcept ;
private:
    address_data m_data;
};


} // data

// Aliases
using address_t = mintex::data::minter_address;
} // mintex

#endif //MINTEX_MINTER_ADDRESS_H

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

#include <bip39/src/minter/utils.h>
#include <vector>
#include <cstdint>
namespace mintex {

namespace data {
using address_data = minter::FixedData<20>;

class minter_address {
public:
    minter_address() = default;
    minter_address(const char *hexString);
    minter_address(const std::vector<uint8_t> &data);
    const std::string toString();
    const std::string toStringWithoutPrefix();

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
} // mintex

#endif //MINTEX_MINTER_ADDRESS_H

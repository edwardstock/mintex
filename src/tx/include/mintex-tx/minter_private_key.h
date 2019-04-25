/*!
 * mintex.
 * minter_private_key.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <bip39/src/minter/utils.h>


#ifndef MINTEX_MINTER_PRIVATE_KEY_H
#define MINTEX_MINTER_PRIVATE_KEY_H

namespace mintex {

class minter_private_key: public minter::FixedData<32> {
public:
    minter_private_key() : FixedData() {
    }
    minter_private_key(const char *hexString) : FixedData() {
        m_data = std::move(minter::hexToBytes(hexString));
    }
    minter_private_key(const uint8_t *data) : FixedData(data) {

    }
    minter_private_key(std::vector<uint8_t> &&data) : FixedData(data) {

    }
    minter_private_key(const std::vector<uint8_t> &data) : FixedData(data) {

    }
    minter_private_key(const uint8_t *data, size_t len) : FixedData(data, len) {

    }


};


}

#endif //MINTEX_MINTER_PRIVATE_KEY_H

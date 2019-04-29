/*!
 * mintex.
 * minter_public_key.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <toolboxpp.hpp>
#include "mintex-tx/minter_public_key.h"

mintex::data::minter_public_key::minter_public_key(const char *hexString) {
    std::string pk = toolboxpp::strings::substringReplaceAll(
        std::vector<std::string>{"Mp", "mp"},
        std::vector<std::string>{"", ""},
        hexString
    );

    m_data = data_t(pk.c_str());
}
mintex::data::minter_public_key::minter_public_key(const std::vector<uint8_t> &data) :
    m_data(data) {
}
mintex::data::minter_public_key::minter_public_key(std::vector<uint8_t> &&data) :
    m_data(std::move(data)) {
}

bool mintex::data::minter_public_key::operator==(const mintex::data::minter_public_key &other) const noexcept {
    return m_data == other.m_data;
}
bool mintex::data::minter_public_key::operator==(const minter::Data &other) const noexcept {
    return m_data == other.cget();
}
bool mintex::data::minter_public_key::operator!=(const mintex::data::minter_public_key &other) const noexcept {
    return m_data == other.m_data;
}
bool mintex::data::minter_public_key::operator!=(const minter::Data &other) const noexcept {
    return m_data == other.cget();
}

mintex::data::minter_public_key::operator std::string() const {
    return m_data.toHex();
}

mintex::data::minter_public_key::operator const std::vector<uint8_t> &() const {
    return m_data.cget();
}

const dev::bytes &mintex::data::minter_public_key::get() const {
    return m_data.cget();
}
dev::bytes &mintex::data::minter_public_key::get() {
    return m_data.get();
}

uint8_t mintex::data::minter_public_key::operator[](size_t element) const noexcept {
    return at(element);
}

uint8_t mintex::data::minter_public_key::at(size_t idx) const {
    return m_data.cget().at(idx);
}


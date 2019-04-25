/*!
 * mintex.
 * minter_address.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <string>
#include <toolboxpp.hpp>
#include <regex>
#include "mintex-tx/minter_address.h"

mintex::data::minter_address::minter_address(const char *addressHex) {
    std::string tmp(addressHex);
    if(tmp.length() != 40 && tmp.length() != 42) {
        throw std::runtime_error("address length is not valid");
    }

    std::string address = toolboxpp::strings::substringReplaceAll(
        std::vector<std::string>{"Mx", "mx"},
        std::vector<std::string>{"", ""},
        addressHex
        );

    m_data = address_data(address.c_str());
}

mintex::data::minter_address::minter_address(const std::vector<uint8_t> &data) {
    m_data = data;
}

const uint8_t *mintex::data::minter_address::data() const {
    return m_data.cdata();
}

uint8_t *mintex::data::minter_address::data() {

    return m_data.data();
}

const std::vector<uint8_t> &mintex::data::minter_address::get() const {
    return m_data.cget();
}
std::vector<uint8_t> &mintex::data::minter_address::get() {
    return m_data.get();
}

const std::string mintex::data::minter_address::toString() {
    return "Mx" + m_data.toHex();
}

const std::string mintex::data::minter_address::toStringWithoutPrefix() {
    return m_data.toHex();
}

bool mintex::data::minter_address::operator==(const mintex::data::minter_address &other) const noexcept {
    return m_data.cget() == other.m_data.cget();
}
bool mintex::data::minter_address::operator==(const minter::Data &other) const noexcept {
    return m_data.cget() == other.cget();
}
bool mintex::data::minter_address::operator!=(const mintex::data::minter_address &other) const noexcept {
    return !(operator==(other));
}
bool mintex::data::minter_address::operator!=(const minter::Data &other) const noexcept {
    return !(operator==(other));
}

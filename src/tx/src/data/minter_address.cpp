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
#include "mintex-tx/utils.h"

mintex::data::minter_address::minter_address(const char *hex) {
    std::string tmp(hex);
    if(tmp.length() != 40 && tmp.length() != 42) {
        throw std::runtime_error("address length is not valid");
    }

    std::string address = toolboxpp::strings::substringReplaceAll(
        std::vector<std::string>{"Mx", "mx"},
        std::vector<std::string>{"", ""},
        hex
        );

    m_data = address_data(address.c_str());
}

mintex::data::minter_address::minter_address(const std::vector<uint8_t> &data) {
    m_data = data;
}

mintex::data::minter_address::minter_address(std::vector<uint8_t> &&data) {
    m_data = std::move(data);
}

mintex::data::minter_address::minter_address(const mintex::pubkey_t &pub_key) {
    //new BytesData(data.dropFirst())
    //                        .sha3Mutable()
    //                        .takeLastMutable(20)

    minter::Data d = pub_key.get();
    dev::bytes dropped_first(pub_key.get().size()-1);
    std::copy(pub_key.get().begin()+1, pub_key.get().end(), dropped_first.begin());
    dev::bytes hashed = mintex::utils::sha3k(dropped_first);
    dev::bytes last20(20);
    std::copy(hashed.end()-20, hashed.end(), last20.begin());

    m_data = std::move(last20);
}

mintex::data::minter_address::minter_address(const mintex::privkey_t &priv_key):
    minter_address(priv_key.get_public_key()) {

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

const std::string mintex::data::minter_address::to_string() {
    return "Mx" + m_data.toHex();
}

const std::string mintex::data::minter_address::to_string_no_prefix() {
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

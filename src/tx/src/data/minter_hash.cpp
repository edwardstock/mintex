/*!
 * mintex.
 * minter_hash.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <toolboxpp.hpp>
#include "mintex-tx/minter_hash.h"

mintex::data::minter_hash::minter_hash(const char *hex) {
    std::string pk = toolboxpp::strings::substringReplaceAll(
        std::vector<std::string>{"Mt", "mt"},
        std::vector<std::string>{"", ""},
        hex
    );

    m_data = data_t(pk.c_str());
}
mintex::data::minter_hash::minter_hash(const std::string &hex): minter_hash(hex.c_str()) {

}
mintex::data::minter_hash::minter_hash(const std::vector<uint8_t> &data) {
    m_data = data;
}
mintex::data::minter_hash::minter_hash(std::vector<uint8_t> &&data) {
    m_data = std::move(data);
}
bool mintex::data::minter_hash::operator==(const mintex::data::minter_hash &other) const noexcept {
    return m_data == other.m_data;
}
bool mintex::data::minter_hash::operator==(const minter::Data &other) const noexcept {
    return m_data == other.get();
}
bool mintex::data::minter_hash::operator!=(const mintex::data::minter_hash &other) const noexcept {
    return m_data == other.m_data;
}
bool mintex::data::minter_hash::operator!=(const minter::Data &other) const noexcept {
    return m_data == other.get();
}
uint8_t mintex::data::minter_hash::operator[](size_t element) const noexcept {
    return at(element);
}
uint8_t mintex::data::minter_hash::at(size_t idx) const {
    return m_data.get().at(idx);
}
const dev::bytes &mintex::data::minter_hash::get() const {
    return m_data.get();
}
dev::bytes &mintex::data::minter_hash::get() {
    return m_data.get();
}
mintex::data::minter_hash::operator const std::vector<uint8_t> &() const {
    return get();
}
mintex::data::minter_hash::operator std::string() const {
    return to_string();
}
std::string mintex::data::minter_hash::to_string() const {
    return "Mt" + m_data.toHex();
}
std::string mintex::data::minter_hash::to_string_no_prefix() const {
    return m_data.toHex();
}

std::ostream& operator << (std::ostream &os, const mintex::hash_t &hash) {
    os << hash.to_string();
    return os;
}
 
 
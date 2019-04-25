/*!
 * mintex.
 * utils.hpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_UTILS_H
#define MINTEX_UTILS_H

#include <eth/vector_ref.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <eth/RLP.h>
#include <string>
#include <bip39/src/minter/utils.h>

namespace mintex {
namespace utils {

static const dev::bigint normalized_value_int = dev::bigint("1000000000000000000");
static const dev::bigdec18 normalized_value_dec = dev::bigdec18("1000000000000000000");

dev::bytes to_bytes_fixed(const std::string &input, size_t fixed_size = 10);
dev::bytes to_bytes(std::string &&input);
dev::bytes to_bytes(const std::string &input);
dev::bytes to_bytes(const dev::bigint &num);
dev::bytes to_bytes(const dev::u256 &num);
dev::bytes to_bytes(const dev::bigdec18 &num);
dev::bigint to_bigint(const dev::bytes &bytes);
dev::bigint to_bigint(const uint8_t *bytes, size_t len);
dev::bytes sha3k(const dev::bytes &message);
dev::bytes sha3k(const minter::Data &message);
std::string strip_null_bytes(const char* input);
std::string to_string(const dev::bytes &src);
std::string to_string_clear(const dev::bytes &src);

dev::bigint normalize_value(const char* input);
dev::bigint normalize_value(const std::string &input);
dev::bigint normalize_value(const dev::bigdec18 &value);
dev::bigdec18 humanize_value(const dev::bigint &value);


} // utils
} // mintex

std::ostream &operator << (std::ostream &out, const minter::Data &d);
std::ostream &operator << (std::ostream &out, const dev::bytes &d);
std::ostream &operator << (std::ostream &out, const dev::RLPStream &rlp);


#endif //MINTEX_UTILS_H

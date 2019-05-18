/*!
 * mintex.
 * data_validator
 *
 * \date 19.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_DATA_VALIDATOR_H
#define MINTEX_DATA_VALIDATOR_H

#include <regex>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <functional>
#include <boost/program_options.hpp>
#include <eth/Common.h>
#include "wallet/term.h"


namespace wallet {

extern const std::regex HEX_PATTERN;
extern const std::regex ADDRESS_PATTERN;
extern const std::regex PUBKEY_PATTERN;
extern const std::regex CHECK_PATTERN;
extern const std::regex HASH_PATTERN;
extern const std::regex COIN_PATTERN;
extern const std::regex DECIMAL_PATTERN;

class base_validator {
 public:
    base_validator();
    base_validator(const std::string &error_message);
    base_validator(std::string &&error_message);
    virtual ~base_validator() = default;

    virtual bool validate(const std::string &value) = 0;

    std::string get_error() const;
 protected:
    std::string error_message;

};

class address_validator: public base_validator {
 public:
    address_validator();
    address_validator(const std::string &error_message);
    address_validator(std::string &&error_message);
    bool validate(const std::string &value) override;
};

class pubkey_validator: public base_validator {
 public:
    pubkey_validator();
    pubkey_validator(const std::string &error_message);
    pubkey_validator(std::string &&error_message);
    bool validate(const std::string &value) override;
};

class coin_validator: public base_validator {
 public:
    coin_validator();
    coin_validator(const std::string &error_message);
    coin_validator(std::string &&error_message);
    bool validate(const std::string &value) override;
};

class amount_validator: public base_validator {
 public:
    amount_validator();
    amount_validator(const std::string &error_message);
    amount_validator(std::string &&error_message);
    bool validate(const std::string &value) override;
};

class payload_validator: public base_validator {
 public:
    payload_validator();
    payload_validator(const std::string &error_message);
    payload_validator(std::string &&error_message);
    bool validate(const std::string &value) override;
};

static void print_validation_error(std::unordered_map<std::string, std::vector<std::string>> err_map);

class options_validator {
 public:
    using error_map = std::unordered_map<std::string, std::vector<std::string>>;
    /// \brief result: std::string field, std::string error
    using validate_function = std::function<void(error_map)>;

    options_validator(const boost::program_options::variables_map *opts);
    options_validator* add_option(const std::string &field, base_validator* validator);

    bool validate(const validate_function &error_func = nullptr);

    ~options_validator();
 private:
    std::unordered_map<std::string, std::vector<base_validator*>> m_validators;
    const boost::program_options::variables_map* m_options;
};



}

#endif //MINTEX_DATA_VALIDATOR_H

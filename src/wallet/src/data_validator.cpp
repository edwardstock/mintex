/*!
 * mintex.
 * data_validator
 *
 * \date 19.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include <exception>
#include <stdexcept>
#include <toolboxpp.hpp>
#include <rang.hpp>
#include "wallet/data_validator.h"

const std::regex wallet::HEX_PATTERN = std::regex(R"(^([a-fA-F0-9])$)", std::regex::icase);
const std::regex wallet::ADDRESS_PATTERN = std::regex(R"(^(Mx)?([a-fA-F0-9]{40})$)", std::regex::icase);
const std::regex wallet::PUBKEY_PATTERN = std::regex(R"(^(Mp)?([a-fA-F0-9]{64})$)", std::regex::icase);
const std::regex wallet::CHECK_PATTERN = std::regex(R"(^(Mc)?([a-fA-F0-9]+)$)", std::regex::icase);
const std::regex wallet::HASH_PATTERN = std::regex(R"(^(Mt)?([a-fA-F0-9]+)$)", std::regex::icase);
const std::regex wallet::COIN_PATTERN = std::regex(R"(^[a-zA-Z0-9]{3,10}$)", std::regex::icase);
const std::regex wallet::DECIMAL_PATTERN = std::regex(R"(^(\d*)([.])?(\d{1,18})$)", std::regex::icase);



wallet::base_validator::base_validator() :
    error_message("Incorrect field") {
}
wallet::base_validator::base_validator(const std::string &error_message) :
    error_message(error_message) {
}

wallet::base_validator::base_validator(std::string &&error_message) :
    error_message(std::move(error_message)) {
}

std::string wallet::base_validator::get_error() const {
    return error_message;
}

wallet::address_validator::address_validator() : base_validator() {

}
wallet::address_validator::address_validator(const std::string &error_message) : base_validator(error_message) {

}
wallet::address_validator::address_validator(std::string &&error_message) : base_validator(error_message) {

}

bool wallet::address_validator::validate(const std::string &value) {
    return toolboxpp::strings::hasRegex(ADDRESS_PATTERN, value);
}

wallet::pubkey_validator::pubkey_validator() : base_validator("Invalid public key") {

}
wallet::pubkey_validator::pubkey_validator(const std::string &error_message) : base_validator(error_message) {

}
wallet::pubkey_validator::pubkey_validator(std::string &&error_message) : base_validator(error_message) {

}
bool wallet::pubkey_validator::validate(const std::string &value) {
    return toolboxpp::strings::hasRegex(PUBKEY_PATTERN, value);
}
wallet::coin_validator::coin_validator() : base_validator("Invalid coin format: length must be >= 3 and <= 10, should contain only letters and digits") {

}
wallet::coin_validator::coin_validator(const std::string &error_message) : base_validator(error_message) {

}
wallet::coin_validator::coin_validator(std::string &&error_message) : base_validator(error_message) {

}
bool wallet::coin_validator::validate(const std::string &value) {
    return toolboxpp::strings::hasRegex(COIN_PATTERN, value);
}
wallet::amount_validator::amount_validator() : base_validator(), m_all(false) {
}
wallet::amount_validator::amount_validator(bool use_all): base_validator(), m_all(use_all) {
}
wallet::amount_validator::amount_validator(const std::string &error_message) : base_validator(error_message) {

}
wallet::amount_validator::amount_validator(std::string &&error_message) : base_validator(error_message) {

}
bool wallet::amount_validator::validate(const std::string &value) {
    if(m_all && toolboxpp::strings::equalsIgnoreCase(value, "all")) {
        return true;
    }
    if (!toolboxpp::strings::hasRegex(DECIMAL_PATTERN, value)) {
        error_message = "Invalid number format";
        return false;
    }

    try {
        dev::bigdec18 num = dev::bigdec18(value);
        bool res = num >= 0;
        if(!res) {
            error_message = "Value must be greater than 0";
        }
        return res;
    } catch (std::exception &e) {
        error_message = "Unable to initialize decimal value";
        return false;
    }
}
wallet::payload_validator::payload_validator() : base_validator("Invalid payload length") {

}
wallet::payload_validator::payload_validator(const std::string &error_message) : base_validator(error_message) {

}
wallet::payload_validator::payload_validator(std::string &&error_message) : base_validator(error_message) {

}
bool wallet::payload_validator::validate(const std::string &value) {
    return value.length() <= 1024;
}

void wallet::print_validation_error(std::unordered_map<std::string, std::vector<std::string>> err_map) {
    std::cerr << rang::fgB::red << "Validation failed:" << rang::fg::reset << std::endl;
    for(auto &kv: err_map) {
        std::cerr << rang::fgB::red << "Option \"" << kv.first << "\"" << rang::fg::reset << std::endl;
        for(const auto& err: kv.second) {
            std::cerr << toolboxpp::strings::repeat(' ', 4) << rang::fg::red << "* " << err << rang::fg::reset << std::endl;
        }
        std::cerr << std::endl;
    }
}
wallet::options_validator::options_validator(const boost::program_options::variables_map *opts) : m_options(opts) {

}
wallet::options_validator *wallet::options_validator::add_option(const std::string &field,
                                                                 wallet::base_validator *validator) {
    if(!m_options->count(field)) {
        return this;
    }

    if(!m_validators.count(field)) {
        m_validators.emplace(field, std::vector<base_validator*>());
        m_validators.at(field).push_back(validator);
    } else {
        m_validators.at(field).push_back(validator);
    }

    return this;
}
bool wallet::options_validator::validate(const wallet::options_validator::validate_function &error_func) {
    error_map err_map;

    for(const auto & kv: m_validators) {
        const auto fname = kv.first;
        std::for_each(kv.second.begin(), kv.second.end(), [this, &fname, &err_map](base_validator* v) {
          if(!v->validate(m_options->at(fname).as<std::string>())) {
              if(!err_map.count(fname)) {
                  err_map.emplace(fname, std::vector<std::string>());
              }

              err_map.at(fname).push_back(v->get_error());
          }
        });
    }

    if(err_map.size()) {
        if(error_func == nullptr) {
            print_validation_error(err_map);
        }
        return false;
    }

    return true;
}
wallet::options_validator::~options_validator() {
    for(auto& list: m_validators) {
        std::for_each(list.second.begin(), list.second.end(), [](base_validator *v) {
          delete v;
        });
    }
}

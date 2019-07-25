/*!
 * mintex.
 * prompt_manager.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include "wallet/prompt_manager.h"

wallet::prompt_manager::~prompt_manager() {
    for(auto& list: m_validators) {
        std::for_each(list.second.begin(), list.second.end(), [](base_validator *v) {
          delete v;
        });
    }
}
wallet::prompt_manager *wallet::prompt_manager::add_field(const std::string &field, wallet::base_validator *validator) {
    if(!m_validators.count(field)) {
        m_validators.emplace(field, std::vector<wallet::base_validator*>());
        m_validators.at(field).push_back(validator);
    } else {
        m_validators.at(field).push_back(validator);
    }

    return this;
}

wallet::prompt_manager *wallet::prompt_manager::set_descr(const std::string &field, const std::string &desc) {
    if(!m_descriptions.count(field)) {
        m_descriptions.emplace(field, desc);
    } else {
        m_descriptions[field] = desc;
    }

    return this;
}

void wallet::prompt_manager::prompt() {
    using namespace toolboxpp;
    auto validate_value = [](const std::vector<wallet::base_validator*> &validators, const std::string &value) -> bool {
      for(const auto& validator: validators) {
          if(!validator->validate(value)) {
              std::cerr << validator->get_error() << std::endl;
              return false;
          }
      }
      return true;
    };

    for(const auto& kv: m_validators) {
        std::string prompt_text = "Type " + kv.first;
        if(m_descriptions.count(kv.first)) {
            prompt_text += " (" + m_descriptions.at(kv.first) + ")";
        }

        std::string val = console::prompt(prompt_text, true);
        while(!validate_value(kv.second, val)) {
            val = console::prompt(prompt_text, true);
        }
        m_values.emplace(kv.first, val);
    }
}
const std::string &wallet::prompt_manager::get(const std::string &field) const {
    return m_values.at(field);
}

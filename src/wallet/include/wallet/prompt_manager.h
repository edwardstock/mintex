/*!
 * mintex.
 * prompt_manager.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_PROMPT_MANAGER_H
#define MINTEX_PROMPT_MANAGER_H

#include <unordered_map>
#include <string>
#include <vector>
#include "data_validator.h"

namespace wallet {

class prompt_manager {
 public:
    prompt_manager() = default;
    virtual ~prompt_manager();

    prompt_manager* add_field(const std::string &field, wallet::base_validator *validator);
    prompt_manager* set_descr(const std::string &field, const std::string &desc);
    const std::string& get(const std::string &field) const;
    void prompt();

 private:
    std::unordered_map<std::string, std::vector<wallet::base_validator *>> m_validators;
    std::unordered_map<std::string, std::string> m_values;
    std::unordered_map<std::string, std::string> m_descriptions;
};

}

#endif //MINTEX_PROMPT_MANAGER_H

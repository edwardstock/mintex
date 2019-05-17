/*!
 * mintex.
 * settings.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_SETTINGS_H
#define MINTEX_SETTINGS_H
#include <fstream>
#include <fmt/format.h>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>


#define JSON_GET(j, field, type, out) if(j.find(field) != j.end() && !j.at(field).is_null()) j.at(field).get_to<type>(out)

namespace wallet {

class settings {
 public:
    static const std::string SETTINGS_FILE;
    static const std::string DB_FILE;

    friend void from_json(const nlohmann::json &j, settings &out);
    friend void to_json(nlohmann::json &j, const settings &in);

    settings();

 public:
    bool initialized;
    bool pwd_secured;
    std::string passkey;
    std::string passhash;
    std::string home_dir;

    std::string get_home_file_path(const std::string &file) const;

    bool has_saved_pass() const;
    settings& load_settings();
    void flush();
};

void from_json(const nlohmann::json &j, settings &out);
void to_json(nlohmann::json &j, const settings &out);


} // wallet

#endif //MINTEX_SETTINGS_H

/*!
 * mintex.
 * settings.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "wallet/settings.h"

const std::string wallet::settings::SETTINGS_FILE = "settings.json";
const std::string wallet::settings::DB_FILE = "data.db";

wallet::settings::settings() {

}

void wallet::to_json(nlohmann::json &j, const wallet::settings &in) {
    j = nlohmann::json{
        {"pwd_secured", in.pwd_secured},
        {"initialized", in.initialized},
        {"home_dir", in.home_dir},
        {"passkey", in.passkey},
        {"passhash", in.passhash},
    };
}

void wallet::from_json(const nlohmann::json &j, wallet::settings &out) {
    JSON_GET(j, "pwd_secured", bool, out.pwd_secured);
    JSON_GET(j, "initialized", bool, out.initialized);
    JSON_GET(j, "home_dir", std::string, out.home_dir);
    JSON_GET(j, "passkey", std::string, out.passkey);
    JSON_GET(j, "passhash", std::string, out.passhash);
}

wallet::settings& wallet::settings::load_settings() {
    home_dir = std::string(getenv("HOME")) + "/.mintex";
    namespace fs = boost::filesystem;
    fs::path p(get_home_file_path(SETTINGS_FILE));
    if(!fs::exists(p)) {
        initialized = false;
        return *this;
    }

    std::ifstream is;
    is.open(p.string().c_str(), std::ios::in);
    if(is.bad()) {
        initialized = false;
        return *this;
    }

    nlohmann::json j;
    is >> j;

    from_json(j, *this);

    return *this;
}

void wallet::settings::flush() {
    if(!initialized) {
        return;
    }

    nlohmann::json j;
    to_json(j, *this);

    std::ofstream of;
    std::string hpath = get_home_file_path(SETTINGS_FILE);
    of.open(hpath, std::ios::out);
    of << j;

    of.flush();
    of.close();
}

std::string wallet::settings::get_home_file_path(const std::string &file) const {
    return fmt::format("{0}/{1}", home_dir, file);
}

bool wallet::settings::has_saved_pass() const {
    return !passkey.empty();
}


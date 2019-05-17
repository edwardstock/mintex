#include <boost/beast/core/string.hpp>
/*!
 * mintex.
 * app_init
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include <boost/filesystem.hpp>
#include <toolboxpp.hpp>
#include <rang.hpp>
#include "wallet/app/init_controller.h"
#include "wallet/crypto.h"


wallet::cmd::init_controller::init_controller(std::shared_ptr<wallet::app> app)
        :command_controller(app) {
    ACTION_BIND(init_controller, init);

}
std::string wallet::cmd::init_controller::usage() const {
    return std::string();
}
std::string wallet::cmd::init_controller::get_command_name() const {
    return "init";
}

bool wallet::cmd::init_controller::before_action(const std::string &action, int argc, char **argv) {
    return true;
}

void wallet::cmd::init_controller::action_init(const boost::program_options::variables_map &options) {
    using namespace toolboxpp;
    namespace fs = boost::filesystem;


    auto sets = get_app()->get_settings();
    sets->load_settings();

    if(sets->initialized) {
        std::cout << "You already initialized app!" << std::endl;
        return;
    }

    std::string pass;
    std::cout << "Welcome to instant Minter Wallet" << std::endl;
    sets->pwd_secured = console::confirm("Do you want encrypt all your data with password?", false);
    if(sets->pwd_secured) {
        pass = console::prompt("Type password to encrypt with", true);
        while(pass.empty()) {
            std::cerr << "Password can't be empty\n";
            pass = console::prompt("Type password to encrypt with", true);
        }

        if(console::confirm("Save password to home directory (UNSAFE and NOT recommended)", false)) {
            sets->passkey = pass;
        }

        sets->passhash = wallet::crypto::hash_argon2i(pass);
    }

    if(!fs::is_directory(fs::path(sets->home_dir))) {
        fs::create_directories(fs::path(sets->home_dir));
    }

    sets->initialized = true;
    sets->flush();
    std::cout << rang::fgB::green << "All done! Now you can add or generate new secret using \"account\" command\n";

}
void wallet::cmd::init_controller::action_init_options(boost::program_options::options_description &options) {
}
void wallet::cmd::init_controller::run_action(const std::string &action, int argc, char **argv) {
    command_controller::run_action("init", argc, argv);
}
bool wallet::cmd::init_controller::verify_action_argument() const {
    return false;
}
std::unordered_map<std::string, std::string> wallet::cmd::init_controller::get_actions_descriptions() const {
    return std::unordered_map<std::string, std::string>();
}

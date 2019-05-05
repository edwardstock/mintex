/*!
 * mintex.
 * app
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <memory>

#include "wallet/app/app.h"
#include "wallet/app/account_controller.h"
#include "wallet/app/init_controller.h"
#include "wallet/app/secret_controller.h"
#include "wallet/app/settings_controller.h"
#include "wallet/app/tx_controller.h"

std::shared_ptr<wallet::app> wallet::app::create() {
    return std::make_shared<wallet::app>();
}

bool wallet::app::run(int argc, char** argv) const {
    if(argc == 1) {
        std::cerr << rang::style::bold << rang::fg::red << "Invalid arguments\n";
        std::cout << rang::style::reset << usage();
        return false;
    }

    app_command cmd = command_by_name(argv[1]);
    if(cmd == unknown) {
        std::cerr << rang::style::bold << rang::fg::red << "Unknown command " << argv[1] << "\n";
        std::cout << usage();
        return false;
    }

    std::unique_ptr<command_controller> controller;

    switch (cmd) {
    case init:
        controller = std::make_unique<wallet::cmd::init_controller>(shared_from_this());
        break;
    case account:
        controller = std::make_unique<wallet::cmd::account_controller>(shared_from_this());
        break;
    case secret:

        break;
    case tx:

        break;
    case settings:

        break;
    case unknown:
    default:
        break;
    }

    if(argc == 2) {

        std::stringstream ss;
        auto act_list = controller->get_actions_list();
        for(const auto &action_name: act_list) {
            ss << "\n";
            ss << action_name;
            ss << "\n";
            po::options_description desc;
            controller->get_func_opt_map().at(action_name)(std::ref(desc));
            ss << desc << std::endl;
        }
        act_list.clear();


        std::string actions_desc = fmt::format(R"(Minter Text Wallet

Command "{0}" action list:
{1}
)", controller->get_command_name(), ss.str());

        std::cout << actions_desc;

        return false;
    }

    controller->run_action(argv[2], argc, argv);

    return true;

}
bool wallet::app::has_command(const std::string &command) {
    return command_by_name(command) != unknown;
}
std::string wallet::app::usage() const {
    return base_usage();
}

std::string wallet::base_usage() {
    return fmt::format(R"(
Minter Text Wallet
{0}

Commands:
    init
    secret
    account
    tx
    settings

    command --help - help for command
    command sub_command --help - help for sub-command
)", std::string(MINTEX_VERSION));
}

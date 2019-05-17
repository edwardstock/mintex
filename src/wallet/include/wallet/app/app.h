/*!
 * mintex.
 * wallet
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <string>
#include <ostream>
#include <unordered_map>
#include <boost/program_options.hpp>
#include <rang.hpp>
#include <fmt/format.h>
#include <toolboxpp.hpp>
#include "wallet/data/secret_storage.h"
#include "wallet/settings.h"

#ifndef MINTEX_WALLET_H
#define MINTEX_WALLET_H

namespace wallet {

static std::string base_usage();

//static const char* usage = R"(
//Sub-commands:
//    Base:
//    init - initialize wallet
//
//    Working with secret keys:
//
//    secret generate - generate new secret key
//    secret add "your mnemonic phrase" - mnemonic phrase contains 16 words
//    secret add _hex_private_key_string_ - 32 bytes hex private key
//    secret list - list secrets
//    secret remove N - remove secret by index from list
//    secret purge - remove all secrets
//    secret export - print mnemonic phrase
//
//    Accounting:
//
//    account balance - show balance for account (default uses 0 account)
//        options:
//            -i [--index] index of secret  (default: 0)
//    account last_transactions - list last transactions
//        options:
//            -n [--number]
//

//)";



enum app_command {
  unknown,
  init,
  secret,
  account,
  tx,
};

static std::unordered_map<std::string, app_command> command_map = {
    {"init",    init},
    {"secret",  secret},
    {"account", account},
    {"tx",      tx},
};

static app_command command_by_name(const std::string &name) {
    if (!command_map.count(name)) {
        return unknown;
    }

    return command_map.at(name);
}

#define ACTION_DECLARE(name) void action_##name(const po::variables_map &options); \
void action_##name##_options(po::options_description &options)

#define ACTION_BIND(cls, name) func_map.emplace(std::pair<std::string, action_func_t>( \
    #name, \
    std::bind( \
            &cls::action_##name, \
            this, \
            std::placeholders::_1 \
            ))); \
func_opts_map.emplace(std::pair<std::string, action_opts_func_t>( \
    #name, \
    std::bind( \
            &cls::action_##name##_options, \
            this, \
            std::placeholders::_1 \
            ))) \


class app : public std::enable_shared_from_this<app> {
 public:
    app();
    static std::shared_ptr<app> create();
    std::string usage() const;
    bool run(int argc, char **argv);

    std::shared_ptr<wallet::settings> get_settings() {
        if (!m_sets) {
            m_sets = std::make_shared<wallet::settings>();
        }

        return m_sets;
    }

    bool is_initialized() {
        auto sets = get_settings();
        sets->load_settings();
        return sets->initialized;
    }

 private:
    std::shared_ptr<wallet::settings> m_sets;
    bool has_command(const std::string &command);

};
namespace po = boost::program_options;
class command_controller {
 public:
    command_controller(std::shared_ptr<wallet::app> app) :
        m_app(app) {
    }

    using action_func_t = std::function<void(const po::variables_map &)>;
    using action_opts_func_t = std::function<void(po::options_description &)>;
    virtual ~command_controller() = default;
    virtual std::string usage() const = 0;
    virtual std::string get_command_name() const = 0;
    virtual std::unordered_map<std::string, std::string> get_actions_descriptions() const = 0;

    virtual bool has_action(const std::string &command) const {
        return func_map.count(command) > 0;
    }

    virtual bool has_action_options(const std::string &command) const {
        return func_opts_map.count(command) > 0;
    }

    virtual bool verify_action_argument() const {
        return true;
    }

    std::vector<std::string> get_actions_list() const {
        std::vector<std::string> out;
        out.reserve(func_map.size());
        for (const auto &item: func_map) {
            out.push_back(item.first);
        }

        return out;
    }

    virtual bool before_action(const std::string &action, int argc, char** argv) {
        if(!get_app()->is_initialized()) {
            std::cout << rang::fg::red << "App didn't initialized yet.\n";
            std::cout << rang::fg::reset << "Run \"mintex-wallet init\" to do it\n";
            return false;
        }

        return true;
    }
    virtual void after_action(const std::string &action, int argc, char** argv) {
    }

    virtual void run_action(const std::string &action, int argc, char **argv) {
        if (!has_action(action)) {
            std::cerr << fmt::format("Invalid action {0} for {1} command", action, get_command_name()) << "\n";
            return;
        }

        if (!has_action_options(action)) {
            throw std::runtime_error(fmt::format("Command action {0} {1} does not have options configuration",
                                                 get_command_name(),
                                                 action));
        }

        po::options_description desc(usage());
        desc.add_options()
                ("help", "print this help");

        func_opts_map[action](std::ref(desc));

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return;
        }

        func_map[action](vm);
    }

    const std::unordered_map<std::string, action_func_t> &get_func_map() {
        return func_map;
    }

    const std::unordered_map<std::string, action_opts_func_t> &get_func_opt_map() {
        return func_opts_map;
    }

 protected:
    std::unordered_map<std::string, action_func_t> func_map;
    std::unordered_map<std::string, action_opts_func_t> func_opts_map;

    std::shared_ptr<app> get_app() {
        return m_app;
    }

 private:
    std::shared_ptr<app> m_app;
};

}

#endif //MINTEX_WALLET_H

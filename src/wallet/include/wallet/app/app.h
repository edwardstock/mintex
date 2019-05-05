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
//    Transactions:
//    transactions global options:
//        -m [--mode] - transaction sign mode (default: send):
//            - send - send transactions to the network
//            - qr - generate transaction qr code and show in terminal
//            - raw - generate hex bytes of signed transaction and prints it
//
//    tx send - send coins to address
//        options:
//            -v [--value] 1000
//            -c [--coin] MNT
//            -a [--address] Mx0000000000000000000000000000000000000000
//
//    tx buy 1 - buy 1 MYCOIN for some amount of MNT
//        options:
//            -t [--target] MNT - coin to buy
//            -s [--source] MYCOIN - coin to sell
//
//    tx sell - sell SOURCE_COINS and get some TARGET_COINS
//        options:
//            -v [--value] 1000 - value to sell
//            -s [--source] MNT - coin to sell
//            -t [--target] MYCOIN - coin to buy
//
//    tx sell_all - sell all SOURCE_COIN and get TARGET_COINS for whole sum (wa-bank)
//        options:
//            -s [--source] MNT - coin to sell
//            -t [--target] MYCOIN - coin to buy
//
//    tx delegate - delegate N COINS to validator
//        options:
//            -v [--value] 1000
//            -c [--coin] MNT
//            -p [--pubkey] Mp0eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a43
//
//    tx unbond 1 MNT Mp0eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a43 - unbond 1 MNT from validator
//    tx declare_candidacy - declare masternode validator candidacy
//        options:
//            --address=Mx0000000000000000000000000000000000000000
//            --pubkey=Mp0eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a43
//            --stake=1000
//            --coin=MNT
//            --commission=10
//)";



enum app_command {
    unknown,
    init,
    secret,
    account,
    tx,
    settings,
};

static std::unordered_map<std::string, app_command> command_map = {
        {"init", init},
        {"secret", secret},
        {"account", account},
        {"tx", tx},
        {"settings", settings}
};

static app_command command_by_name(const std::string &name) {
    if (!command_map.count(name)) {
        return unknown;
    }

    return command_map.at(name);
}

#define CMD_DECLARE(name) void cmd_##name(const po::variables_map &options); \
void cmd_##name##_options(po::options_description &options)

#define CMD_BIND(cls, name) func_map.emplace(std::pair<std::string, cmd_func_t>( \
    #name, \
    std::bind( \
            &cls::cmd_##name, \
            this, \
            std::placeholders::_1 \
            ))); \
func_opts_map.emplace(std::pair<std::string, cmd_opts_func_t>( \
    #name, \
    std::bind( \
            &cls::cmd_##name##_options, \
            this, \
            std::placeholders::_1 \
            ))) \


class app: public std::enable_shared_from_this<app> {
public:
    app() = default;
    static std::shared_ptr<app> create();
    std::string usage() const;
    bool run(int argc, char** argv) const;


private:
    bool has_command(const std::string &command);
};
namespace po = boost::program_options;
class command_controller {
private:

public:
    command_controller(std::shared_ptr<const wallet::app> app):
        m_app(app) {
    }

    using cmd_func_t = std::function<void(const po::variables_map &)>;
    using cmd_opts_func_t = std::function<void(po::options_description &)>;
    virtual ~command_controller() = default;
    virtual std::string usage() const = 0;
    virtual std::string get_command_name() const = 0;

    bool has_command(const std::string &command) {
        return func_map.count(command);
    }

    bool has_command_options(const std::string &command) {
        return func_opts_map.count(command);
    }

    std::vector<std::string> get_actions_list() const {
        std::vector<std::string> out;
        out.reserve(func_map.size());
        for(const auto& item: func_map) {
            out.push_back(item.first);
        }

        return out;
    }


    void run_action(const std::string &action, int argc, char** argv) {
        if(!has_command(action)) {
            std::cerr << fmt::format("Invalid action {0} for {1} command", action, get_command_name()) << "\n";
            return;
        }

        if(!has_command_options(action)) {
            throw std::runtime_error(fmt::format("Command action {0} {1} does not have options configuration", get_command_name(), action));
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

    const std::unordered_map<std::string, cmd_func_t>& get_func_map() {
        return func_map;
    }

    const std::unordered_map<std::string, cmd_opts_func_t>& get_func_opt_map() {
        return func_opts_map;
    }


protected:
    std::unordered_map<std::string, cmd_func_t> func_map;
    std::unordered_map<std::string, cmd_opts_func_t> func_opts_map;

    std::shared_ptr<const app> get_app() const {
        return m_app;
    }

private:
    std::shared_ptr<const app> m_app;
};

}

#endif //MINTEX_WALLET_H

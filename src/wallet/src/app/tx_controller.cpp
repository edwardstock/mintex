/*!
 * mintex.
 * tx_controller
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include <string>
#include <unordered_map>
#include <toolboxpp.hpp>
#include <fort.hpp>
#include "mintex-tx/tx.h"
#include "mintex-tx/tx_builder.h"
#include "wallet/app/tx_controller.h"
#include "wallet/explorer/repository.h"
#include "wallet/gate/repository.h"
#include "wallet/gate/exchange_calculator.h"
#include "wallet/term.h"

wallet::cmd::tx_controller::tx_controller(const std::shared_ptr<wallet::app> &app) : command_controller(app) {

    ACTION_BIND(tx_controller, send);
    ACTION_BIND(tx_controller, buy);
    ACTION_BIND(tx_controller, sell);
    ACTION_BIND(tx_controller, delegate);
    ACTION_BIND(tx_controller, unbond);
}

bool wallet::cmd::tx_controller::before_action(const std::string &action, int argc, char **argv) {
    if (wallet::command_controller::before_action(action, argc, argv)) {
        m_storage = secret_storage::create(true, get_app()->get_settings());
        return true;
    }

    return false;
}

std::string wallet::cmd::tx_controller::usage() const {
    return std::string();
}
std::string wallet::cmd::tx_controller::get_command_name() const {
    return std::string();
}
std::unordered_map<std::string, std::string> wallet::cmd::tx_controller::get_actions_descriptions() const {
    return std::unordered_map<std::string, std::string>{
        {"send",     "Send coins to address"},
        {"buy",      "Buy some coins"},
        {"sell",     "Sell your coins"},
        {"delegate", "Delegate coins to validator (Start Mining)"},
        {"unbond",   "Take delegated coins back from validator (Stop Mining)"},
    };
}

//    Transactions:
//    transactions global options:
//        -m [--mode] - transaction sign mode (default: send):
//            - send - send transactions to the network
//            - qr - generate transaction qr code and show in terminal
//            - raw - generate hex bytes of signed transaction and prints it
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

void wallet::cmd::tx_controller::action_send_options(boost::program_options::options_description &options) {
    options.add_options()
               ("amount,v", po::value<std::string>()->required(), "Amount to send")
               ("coin,c", po::value<std::string>()->default_value("BIP"), "Coin to send")
               ("address,a", po::value<std::string>()->required(), "Recipient address")
               ("index,i", po::value<size_t>()->default_value(0), "Account index");
}

void wallet::cmd::tx_controller::action_send(const boost::program_options::variables_map &options) {
    using namespace mintex;
    using namespace toolboxpp::console;
    using namespace wallet::term;
    using namespace wallet;
    using namespace wallet::gate;

    dev::bigdec18 amount = dev::bigdec18(options.at("amount").as<std::string>());
    std::string coin = options.at("coin").as<std::string>();
    address_t address = options.at("address").as<std::string>();
    size_t index = options.at("index").as<size_t>();

    auto secret_data = m_storage->get_secret(index);

    std::cout << "You are sending:\n";
    fort::table table;
    table << "Account" << secret_data.get_address().to_string() << fort::endr;
    table << "To" << address.to_string() << fort::endr;
    table << "Coin" << coin << fort::endr;
    table << "Amount" << utils::to_string(amount) << fort::endr;
    std::cout << table.to_string() << std::endl;

    if (!confirm("Is it OK?", true)) {
        std::cout << "Operation canceled" << "\n";
        return;
    }

    bool error = false;

    progress_indeterminate p;
    p.start();

    dev::bigint nonce;
    dev::bigint gas = dev::bigint("1");

    repository data_repo;
    auto nonce_task =
        data_repo.get_tx_count(secret_data.get_address())
                 ->success([&nonce](base_result<tx_count_value> result) {
                   nonce = result.data.count + dev::bigint("1");
                 })
                 ->error([&error](httb::response resp, base_result<tx_count_value> result) {
                   error = true;
                   std::cerr << resp.statusMessage << std::endl;
                 });
    auto gas_task =
        data_repo.get_min_gas()
                 ->success([&gas](base_result<gas_value> result) {
                   gas = result.data.gas;
                 })
                 ->error([&error](httb::response resp, base_result<gas_value> result) {
                   error = true;
                   std::cerr << resp.statusMessage << std::endl;
                 });

    wallet::net::pair_task<
        base_result<tx_count_value>,
        base_result<gas_value>
    > executor(nonce_task, gas_task);

    executor.execute();

    if (error) {
        return;
    }

    auto tx_builder = new_tx();
    tx_builder->set_nonce(nonce);
    tx_builder->set_gas_price(gas);
    tx_builder->set_gas_coin(std::string(MINTER_COIN));
    tx_builder->set_chain_id(mintex::chain_id_str_map.at(std::string(MINTER_CHAIN)));
    auto data = tx_builder->tx_send_coin();
    data->set_to(address);
    data->set_value(amount);
    data->set_coin(coin);

    auto tx = data->build();
    auto signedTx = tx->sign_single(secret_data.priv_key);
//narrow pool ozone start collect ritual because stairs crop group tackle ketchup
//coconut just coast nephew grit inquiry explain rhythm robust fork plate notable
    data_repo.send_tx(signedTx)
             ->success([&p, &gas, &nonce](base_result<tx_send_result> result) {
               p.stop();
               std::cout << "Tx sent!" << std::endl;
               std::cout << result.data.hash << std::endl;
               std::cout << "Commission was: " << utils::to_string(mintex::tx_send_coin_type::get_fee(gas)) << std::endl;
               std::cout << "Nonce: " << utils::to_string(nonce) << std::endl;
             })
             ->error([&p](httb::response resp, base_result<tx_send_result> result) {
               p.stop();
               std::cerr << resp.statusMessage << std::endl;
               std::cerr << resp.getBody() << std::endl;
             })
             ->execute()
             ->handle().wait();

}
void wallet::cmd::tx_controller::action_buy_options(boost::program_options::options_description &options) {
//    tx buy 1 - buy 1 MYCOIN for some amount of MNT
//        options:
//            -t [--target] MNT - coin to buy
//            -s [--source] MYCOIN - coin to sell
//
    options.add_options()
               ("target,t", po::value<std::string>()->required(), "Target coin")
               ("source,s", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Source coin")
               ("amount,v", po::value<std::string>()->required(), "Amount to buy")
               ("gas-coin", po::value<std::string>()->default_value(std::string(MINTER_COIN)))
               ("index,i", po::value<size_t>()->default_value(0), "Account index");
}
void wallet::cmd::tx_controller::action_buy(const boost::program_options::variables_map &options) {
    using namespace mintex;
    using namespace toolboxpp::console;
    using namespace wallet::term;
    using namespace wallet::gate;

    repository data_repo;

    dev::bigdec18 amount = dev::bigdec18(options.at("amount").as<std::string>());
    std::string from_coin = options.at("source").as<std::string>();
    std::string to_coin = options.at("target").as<std::string>();
    std::string gas_coin = options.at("gas-coin").as<std::string>();
    size_t index = options.at("index").as<size_t>();

    progress_indeterminate p;
    p.start();
    bool error = false;
    exchange_calc_result calc_result;

    auto on_calc_error = [&error](httb::response resp) {
        error = true;
        std::cerr << resp.statusMessage << std::endl;
        std::cerr << resp.getBody() << std::endl;
    };
    auto on_calc_success = [&calc_result](exchange_calc_result result) {
        calc_result = result;
    };

    exchange_calculator calc(from_coin, to_coin, amount);

    calc.calculate(exchange_calculator::buy, on_calc_success, on_calc_error);

    p.stop();
    auto secret_data = m_storage->get_secret(index);

    std::cout << std::setprecision(std::numeric_limits<boost::multiprecision::cpp_dec_float<18>>::max_digits10);
    std::cout << "You are converting:\n";
    fort::table table;

    table << "Account" << secret_data.get_address().to_string() << fort::endr;
    table << "From Coin " << from_coin << fort::endr;
    table << "To   Coin" << to_coin << fort::endr;
    table << "Amount" << utils::to_string(amount) << fort::endr;
    table << "You will pay (approx.)" << utils::to_string(calc_result.amount) << fort::endr;
    table << "Commission" << utils::to_string(calc_result.commission) << fort::endr;
    table << "Gas Coin" << gas_coin << fort::endr;
    std::cout << table.to_string() << std::endl;

    if (!confirm("Is it OK?", true)) {
        std::cout << "Operation canceled" << "\n";
        return;
    }

    p.start();

    tx_init_data init_data = data_repo.get_tx_init_data(secret_data.get_address(), error);

    if (error) {
        p.stop();
        return;
    }

    auto tx_builder = new_tx();
    tx_builder->set_nonce(init_data.nonce);
    tx_builder->set_gas_price(init_data.gas);
    tx_builder->set_gas_coin(gas_coin);
    tx_builder->set_chain_id(mintex::chain_id_str_map.at(std::string(MINTER_CHAIN)));
    auto data = tx_builder->tx_buy_coin();
    data->set_coin_to_buy(to_coin.c_str());
    data->set_coin_to_sell(from_coin.c_str());
    data->set_value_to_buy(amount);
    data->set_max_value_to_sell(calc_result.amount);

    auto tx = data->build();
    auto signedTx = tx->sign_single(secret_data.priv_key);
//narrow pool ozone start collect ritual because stairs crop group tackle ketchup
//coconut just coast nephew grit inquiry explain rhythm robust fork plate notable
    data_repo.send_tx(signedTx)
             ->success([&p, &init_data](base_result<tx_send_result> result) {
               p.stop();
               std::cout << "Tx sent!" << std::endl;
               std::cout << result.data.hash << std::endl;
             })
             ->error([&p](httb::response resp, base_result<tx_send_result> result) {
               p.stop();

               if(!result.error.log.empty()) {
                    print_error_message("Error: " + result.error.log);
               } else {
                   std::cerr << resp.statusMessage << std::endl;
                   std::cerr << resp.getBody() << std::endl;
               }

             })
             ->execute()
             ->handle().wait();
}

void wallet::cmd::tx_controller::action_sell_options(boost::program_options::options_description &options) {
    options.add_options()
               ("target,t", po::value<std::string>()->required(), "Target coin")
               ("source,s", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Source coin")
               ("amount,v", po::value<std::string>()->required(), "Amount to sell. You can set this options as \"all\" to spend all coin balance")
               ("gas-coin", po::value<std::string>()->default_value(std::string(MINTER_COIN)))
               ("index,i", po::value<size_t>()->default_value(0), "Account index");
}

void wallet::cmd::tx_controller::action_sell(const boost::program_options::variables_map &options) {
    using namespace mintex;
    using namespace toolboxpp::console;
    using namespace wallet::term;
    using namespace wallet::gate;


    repository data_repo;


    std::string from_coin = options.at("source").as<std::string>();
    std::string to_coin = options.at("target").as<std::string>();
    std::string gas_coin = options.at("gas-coin").as<std::string>();
    size_t index = options.at("index").as<size_t>();
    dev::bigdec18 amount;
    std::string amount_raw = options.at("amount").as<std::string>();

    progress_indeterminate p;
    auto secret_data = m_storage->get_secret(index);

    bool error = false;
    bool sell_all = false;

    if(toolboxpp::strings::equalsIgnoreCase(amount_raw, "all")) {
        p.start();
        wallet::explorer::repository exp_repo;
        auto balance_task = exp_repo.get_balance(secret_data.get_address())
            ->error([&error](httb::response resp, wallet::explorer::base_result<wallet::explorer::balance_items> result) {
              if(!result.error.message.empty()) {
                  print_error_message("Error: " + result.error.message);
              } else {
                  std::cerr << resp.statusMessage << std::endl;
                  print_error_message(resp.getBody());
              }
            })
            ->success([&from_coin, &amount, &sell_all](wallet::explorer::base_result<wallet::explorer::balance_items> result) {
                for(const auto& item: result.data.balances) {
                    if(toolboxpp::strings::equalsIgnoreCase(from_coin, item.coin)) {
                        amount = item.amount;
                        sell_all = true;
                        break;
                    }
                }
            });

        balance_task->execute()->handle().wait();
        p.stop();

        if(error) {
            return;
        }
    } else {
        amount = dev::bigdec18(amount_raw);
    }


    exchange_calc_result calc_result;

    auto on_calc_error = [&error](httb::response resp) {
      error = true;
      std::cerr << resp.statusMessage << std::endl;
      std::cerr << resp.getBody() << std::endl;
    };
    auto on_calc_success = [&calc_result](exchange_calc_result result) {
      calc_result = result;
    };

    exchange_calculator calc(from_coin, to_coin, amount);

    calc.calculate(exchange_calculator::sell, on_calc_success, on_calc_error);

    p.stop();


    std::cout << std::setprecision(std::numeric_limits<boost::multiprecision::cpp_dec_float<18>>::max_digits10);
    std::cout << "You are converting:\n";
    fort::table table;

    table << "Account" << secret_data.get_address().to_string() << fort::endr;
    table << "From Coin " << from_coin << fort::endr;
    table << "To   Coin" << to_coin << fort::endr;
    table << "Amount" << utils::to_string(amount) << fort::endr;
    table << "You will get (approx.)" << utils::to_string(calc_result.amount) << fort::endr;
    table << "Commission" << utils::to_string(calc_result.commission) << fort::endr;
    table << "Gas Coin" << gas_coin << fort::endr;
    std::cout << table.to_string() << std::endl;

    if (!confirm("Is it OK?", true)) {
        std::cout << "Operation canceled" << "\n";
        return;
    }

    p.start();

    tx_init_data init_data = data_repo.get_tx_init_data(secret_data.get_address(), error);

    if (error) {
        p.stop();
        return;
    }

    auto tx_builder = new_tx();
    tx_builder->set_nonce(init_data.nonce);
    tx_builder->set_gas_price(init_data.gas);
    tx_builder->set_gas_coin(gas_coin);
    tx_builder->set_chain_id(mintex::chain_id_str_map.at(std::string(MINTER_CHAIN)));
    std::shared_ptr<mintex::tx> tx;
    if(sell_all) {
        auto data = tx_builder->tx_sell_all_coins();
        data->set_coin_to_buy(to_coin.c_str());
        data->set_coin_to_sell(from_coin.c_str());
        data->set_min_value_to_buy(calc_result.amount);
        tx = data->build();
    } else {
        auto data = tx_builder->tx_sell_coin();
        data->set_coin_to_buy(to_coin.c_str());
        data->set_coin_to_sell(from_coin.c_str());
        data->set_value_to_sell(amount);
        data->set_min_value_to_buy(calc_result.amount);
        tx = data->build();
    }


    auto signedTx = tx->sign_single(secret_data.priv_key);

    data_repo.send_tx(signedTx)
             ->success([&p, &init_data](base_result<tx_send_result> result) {
               p.stop();
               std::cout << "Tx sent!" << std::endl;
               std::cout << result.data.hash << std::endl;
             })
             ->error([&p](httb::response resp, base_result<tx_send_result> result) {
               p.stop();

               if(!result.error.log.empty()) {
                   print_error_message("Error: " + result.error.log);
               } else {
                   std::cerr << resp.statusMessage << std::endl;
                   std::cerr << resp.getBody() << std::endl;
               }

             })
             ->execute()
             ->handle().wait();
}
void wallet::cmd::tx_controller::action_delegate_options(boost::program_options::options_description &options) {

}
void wallet::cmd::tx_controller::action_delegate(const boost::program_options::variables_map &options) {

}
void wallet::cmd::tx_controller::action_unbond(const boost::program_options::variables_map &options) {

}
void wallet::cmd::tx_controller::action_unbond_options(boost::program_options::options_description &options) {

}

//void wallet::cmd::tx_controller::action_dc_options(boost::program_options::options_description &options) {
//
//}
//void wallet::cmd::tx_controller::action_dc(const boost::program_options::variables_map &options) {
//
//}

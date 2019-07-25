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
#include <minter/tx.hpp>
#include "wallet/app/tx_controller.h"
#include "wallet/explorer/repository.h"
#include "wallet/gate/repository.h"
#include "wallet/gate/exchange_calculator.h"
#include "wallet/term.h"
#include "wallet/data_validator.h"

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
               ("index,i", po::value<size_t>()->default_value(0), "Account index")
               ("gas-coin", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Commission coin")
               ("payload", po::value<std::string>()->default_value(""), "Tx payload")
               ;
}

void wallet::cmd::tx_controller::action_send(const boost::program_options::variables_map &options) {
    using namespace minter;
    using namespace toolboxpp::console;
    using namespace wallet::term;
    using namespace wallet;
    using namespace wallet::gate;

    wallet::options_validator validator(&options);
    validator.add_option("amount", new wallet::amount_validator)
             ->add_option("coin", new wallet::coin_validator)
             ->add_option("address", new wallet::address_validator)
             ->add_option("gas-coin", new wallet::coin_validator)
             ->add_option("payload", new wallet::payload_validator)
        ;

    if(!validator.validate()) {
        return;
    }

    dev::bigdec18 amount = dev::bigdec18(options.at("amount").as<std::string>());
    std::string coin = options.at("coin").as<std::string>();
    address_t address = options.at("address").as<std::string>();
    size_t index = options.at("index").as<size_t>();
    std::string gas_coin = options.at("gas-coin").as<std::string>();
    std::string payload = options.at("payload").as<std::string>();

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

    repository data_repo;
    tx_init_data init_data = data_repo.get_tx_init_data(secret_data.get_address(), error);

    if (error) {
        return;
    }

    auto tx_builder = new_tx();
    tx_builder->set_nonce(init_data.nonce);
    tx_builder->set_gas_price(init_data.gas);
    tx_builder->set_gas_coin(gas_coin);
    tx_builder->set_chain_id(minter::chain_id_str_map.at(std::string(MINTER_CHAIN)));
    tx_builder->set_payload(std::move(payload));
    auto data = tx_builder->tx_send_coin();
    data->set_to(address);
    data->set_value(amount);
    data->set_coin(coin);

    auto tx = data->build();
    auto signedTx = tx->sign_single(secret_data.priv_key);
    data_repo.send_tx(signedTx)
             ->success([&p, &init_data](base_result<tx_send_result> result) {
               p.stop();
               print_success_tx(result.data.hash);
             })
             ->error([&p](httb::response resp, base_result<tx_send_result> result) {
               p.stop();
               print_error_tx(resp, result.error);
             })
             ->execute()
             ->handle().run();

}
void wallet::cmd::tx_controller::action_buy_options(boost::program_options::options_description &options) {
    options.add_options()
               ("target,t", po::value<std::string>()->required(), "Target coin")
               ("source,s", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Source coin")
               ("amount,v", po::value<std::string>()->required(), "Amount to buy")
               ("index,i", po::value<size_t>()->default_value(0), "Account index")
               ("gas-coin", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Commission coin")
               ("payload", po::value<std::string>()->default_value(""), "Tx payload")
               ;
}
void wallet::cmd::tx_controller::action_buy(const boost::program_options::variables_map &options) {
    using namespace minter;
    using namespace toolboxpp::console;
    using namespace wallet::term;
    using namespace wallet::gate;

    wallet::options_validator validator(&options);
    validator.add_option("target", new wallet::coin_validator)
             ->add_option("source", new wallet::coin_validator)
             ->add_option("amount", new wallet::amount_validator)
             ->add_option("gas-coin", new wallet::coin_validator)
             ->add_option("payload", new wallet::payload_validator)
        ;

    if(!validator.validate()) {
        return;
    }

    repository data_repo;

    dev::bigdec18 amount = dev::bigdec18(options.at("amount").as<std::string>());
    std::string from_coin = options.at("source").as<std::string>();
    std::string to_coin = options.at("target").as<std::string>();
    std::string gas_coin = options.at("gas-coin").as<std::string>();
    std::string payload = options.at("payload").as<std::string>();
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
    table << "Payload" << payload << fort::endr;
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
    tx_builder->set_chain_id(minter::chain_id_str_map.at(std::string(MINTER_CHAIN)));
    tx_builder->set_payload(std::move(payload));
    auto data = tx_builder->tx_buy_coin();
    data->set_coin_to_buy(to_coin);
    data->set_coin_to_sell(from_coin);
    data->set_value_to_buy(amount);
    data->set_max_value_to_sell(calc_result.amount);

    auto tx = data->build();
    auto signedTx = tx->sign_single(secret_data.priv_key);

    data_repo.send_tx(signedTx)
             ->success([&p, &init_data](base_result<tx_send_result> result) {
               p.stop();
               print_success_tx(result.data.hash);
             })
             ->error([&p](httb::response resp, base_result<tx_send_result> result) {
               p.stop();
               print_error_tx(resp, result.error);
             })
             ->execute()
             ->handle().run();
}

void wallet::cmd::tx_controller::action_sell_options(boost::program_options::options_description &options) {
    options.add_options()
               ("target,t", po::value<std::string>()->required(), "Target coin")
               ("source,s", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Source coin")
               ("amount,v",
                po::value<std::string>()->required(),
                "Amount to sell. You can set this options as \"all\" to spend all coin balance")
               ("index,i", po::value<size_t>()->default_value(0), "Account index")
               ("gas-coin", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Commission coin")
               ("payload", po::value<std::string>()->default_value(""), "Tx payload")
               ;
}

void wallet::cmd::tx_controller::action_sell(const boost::program_options::variables_map &options) {
    using namespace minter;
    using namespace toolboxpp::console;
    using namespace wallet::term;
    using namespace wallet::gate;

    wallet::options_validator validator(&options);
    validator.add_option("target", new wallet::coin_validator)
             ->add_option("source", new wallet::coin_validator)
             ->add_option("amount", new wallet::amount_validator)
             ->add_option("gas-coin", new wallet::coin_validator)
             ->add_option("payload", new wallet::payload_validator)
             ;

    if(!validator.validate()) {
        return;
    }

    repository data_repo;

    std::string from_coin = options.at("source").as<std::string>();
    std::string to_coin = options.at("target").as<std::string>();
    std::string gas_coin = options.at("gas-coin").as<std::string>();
    size_t index = options.at("index").as<size_t>();
    dev::bigdec18 amount;
    std::string amount_raw = options.at("amount").as<std::string>();
    std::string payload = options.at("payload").as<std::string>();

    progress_indeterminate p;
    auto secret_data = m_storage->get_secret(index);

    bool error = false;
    bool sell_all = false;

    if (toolboxpp::strings::equalsIgnoreCase(amount_raw, "all")) {
        p.start();
        wallet::explorer::repository exp_repo;
        auto balance_task = exp_repo.get_balance(secret_data.get_address())
                                    ->error([&error](httb::response resp,
                                                     wallet::explorer::base_result<wallet::explorer::balance_items> result) {
                                      if (!result.error.message.empty()) {
                                          print_error_message("Error: " + result.error.message);
                                      } else {
                                          std::cerr << resp.statusMessage << std::endl;
                                          print_error_message(resp.getBody());
                                      }
                                    })
                                    ->success([&from_coin, &amount, &sell_all](wallet::explorer::base_result<wallet::explorer::balance_items> result) {
                                      for (const auto &item: result.data.balances) {
                                          if (toolboxpp::strings::equalsIgnoreCase(from_coin, item.coin)) {
                                              amount = item.amount;
                                              sell_all = true;
                                              break;
                                          }
                                      }
                                    });

        balance_task->execute()->handle().run();
        p.stop();

        if (error) {
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
    table << "Payload" << payload << fort::endr;
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
    tx_builder->set_chain_id(minter::chain_id_str_map.at(std::string(MINTER_CHAIN)));
    tx_builder->set_payload(std::move(payload));
    std::shared_ptr<minter::tx> tx;
    if (sell_all) {
        auto data = tx_builder->tx_sell_all_coins();
        data->set_coin_to_buy(to_coin);
        data->set_coin_to_sell(from_coin);
        data->set_min_value_to_buy(calc_result.amount);
        tx = data->build();
    } else {
        auto data = tx_builder->tx_sell_coin();
        data->set_coin_to_buy(to_coin);
        data->set_coin_to_sell(from_coin);
        data->set_value_to_sell(amount);
        data->set_min_value_to_buy(calc_result.amount);
        tx = data->build();
    }

    auto signedTx = tx->sign_single(secret_data.priv_key);

    data_repo.send_tx(signedTx)
             ->success([&p, &init_data](base_result<tx_send_result> result) {
               p.stop();
               print_success_tx(result.data.hash);
             })
             ->error([&p](httb::response resp, base_result<tx_send_result> result) {
               p.stop();
               print_error_tx(resp, result.error);
             })
             ->execute()
             ->handle().run();
}
void wallet::cmd::tx_controller::action_delegate_options(boost::program_options::options_description &options) {
    options.add_options()
               ("pubkey,p", po::value<std::string>()->required(), "Validator's public key (Started with prefix \"Mp\")")
               ("stake,v", po::value<std::string>()->required(), "Stake to delegate")
               ("coin,s", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Stake to delegate")
               ("index,i", po::value<size_t>()->default_value(0), "Account index")
               ("gas-coin", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Commission coin")
               ("payload", po::value<std::string>()->default_value(""), "Tx payload")
               ;
}
void wallet::cmd::tx_controller::action_delegate(const boost::program_options::variables_map &options) {
    using namespace minter;
    using namespace toolboxpp::console;
    using namespace wallet::term;
    using namespace wallet::gate;

    wallet::options_validator validator(&options);
    validator.add_option("pubkey", new wallet::pubkey_validator)
             ->add_option("stake", new wallet::amount_validator)
             ->add_option("payload", new wallet::payload_validator)
             ->add_option("coin", new wallet::coin_validator)
             ->add_option("gas-coin", new wallet::coin_validator);

    if(!validator.validate()) {
        return;
    }

    repository data_repo;

    minter::pubkey_t pubkey = minter::pubkey_t(options.at("pubkey").as<std::string>());
    dev::bigdec18 stake = dev::bigdec18(options.at("stake").as<std::string>());
    std::string payload = options.at("payload").as<std::string>();
    std::string coin = options.at("coin").as<std::string>();
    std::string gas_coin = options.at("gas-coin").as<std::string>();
    size_t index = options.at("index").as<size_t>();

    progress_indeterminate p;

    bool error = false;

    auto secret_data = m_storage->get_secret(index);

    std::cout << std::setprecision(std::numeric_limits<boost::multiprecision::cpp_dec_float<18>>::max_digits10);
    std::cout << "You are delegating:\n";
    fort::table table;

    table << "Account" << secret_data.get_address().to_string() << fort::endr;
    table << "Validator" << pubkey.to_string() << fort::endr;
    table << "Stake" << fmt::format("{0} {1}", to_string(stake), coin) << fort::endr;
    table << "Gas Coin" << gas_coin << fort::endr;
    table << "Payload" << payload << fort::endr;
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
    tx_builder->set_chain_id(minter::chain_id_str_map.at(std::string(MINTER_CHAIN)));
    tx_builder->set_payload(std::move(payload));
    auto data = tx_builder->tx_delegate();
    data->set_pub_key(pubkey);
    data->set_stake(stake);
    data->set_coin(coin);

    auto tx = data->build();
    auto signedTx = secret_data.sign(tx);

    data_repo.send_tx(signedTx)
             ->success([&p, &init_data](base_result<tx_send_result> result) {
               p.stop();
               print_success_tx(result.data.hash);
             })
             ->error([&p](httb::response resp, base_result<tx_send_result> result) {
               p.stop();
               print_error_tx(resp, result.error);
             })
             ->execute()
             ->handle().run();
}
void wallet::cmd::tx_controller::action_unbond_options(boost::program_options::options_description &options) {
    options.add_options()
               ("pubkey,p", po::value<std::string>()->required(), "Validator's public key (Started with prefix \"Mp\")")
               ("stake,v", po::value<std::string>()->required(), "Stake to delegate")
               ("coin,s", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Stake to delegate")
               ("index,i", po::value<size_t>()->default_value(0), "Account index")
               ("gas-coin", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Commission coin")
               ("payload", po::value<std::string>()->default_value(""), "Tx payload")
               ;
}
void wallet::cmd::tx_controller::action_unbond(const boost::program_options::variables_map &options) {
    using namespace minter;
    using namespace toolboxpp::console;
    using namespace wallet::term;
    using namespace wallet::gate;

    wallet::options_validator validator(&options);
    validator.add_option("pubkey", new wallet::pubkey_validator)
             ->add_option("stake", new wallet::amount_validator)
             ->add_option("payload", new wallet::payload_validator)
             ->add_option("coin", new wallet::coin_validator)
             ->add_option("gas-coin", new wallet::coin_validator);

    if(!validator.validate()) {
        return;
    }

    repository data_repo;

    minter::pubkey_t pubkey = minter::pubkey_t(options.at("pubkey").as<std::string>());
    dev::bigdec18 stake = dev::bigdec18(options.at("stake").as<std::string>());
    std::string payload = options.at("payload").as<std::string>();
    std::string coin = options.at("coin").as<std::string>();
    std::string gas_coin = options.at("gas-coin").as<std::string>();
    size_t index = options.at("index").as<size_t>();

    progress_indeterminate p;

    bool error = false;

    auto secret_data = m_storage->get_secret(index);

    std::cout << std::setprecision(std::numeric_limits<boost::multiprecision::cpp_dec_float<18>>::max_digits10);
    std::cout << "You are unbonding:\n";
    fort::table table;

    table << "Account" << secret_data.get_address().to_string() << fort::endr;
    table << "Validator" << pubkey.to_string() << fort::endr;
    table << "Stake" << fmt::format("{0} {1}", to_string(stake), coin) << fort::endr;
    table << "Gas Coin" << gas_coin << fort::endr;
    table << "Payload" << payload << fort::endr;
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
    tx_builder->set_chain_id(minter::chain_id_str_map.at(std::string(MINTER_CHAIN)));
    tx_builder->set_payload(std::move(payload));
    auto data = tx_builder->tx_unbond();
    data->set_pub_key(pubkey);
    data->set_value(stake);
    data->set_coin(coin);

    auto tx = data->build();
    auto signedTx = secret_data.sign(tx);

    data_repo.send_tx(signedTx)
             ->success([&p, &init_data](base_result<tx_send_result> result) {
               p.stop();
               print_success_tx(result.data.hash);
             })
             ->error([&p](httb::response resp, base_result<tx_send_result> result) {
               p.stop();
               print_error_tx(resp, result.error);
             })
             ->execute()
             ->handle().run();

}

#include "wallet/prompt_manager.h"

void wallet::cmd::tx_controller::action_autodelegate_options(boost::program_options::options_description &options) {
    options.add_options()
               ("index,i", po::value<size_t>()->default_value(0), "Account index")
               ("gas-coin", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Commission coin")
               ("payload", po::value<std::string>()->default_value(""), "Tx payload")
               ("buy-coin", po::value<std::string>()->default_value(std::string(MINTER_COIN)), "Buy coin and delegate it, if it's not the BIP");
}

void wallet::cmd::tx_controller::action_autodelegate(const boost::program_options::variables_map &options) {
    using namespace minter;
    using namespace toolboxpp::console;
    using namespace wallet::term;
    using namespace wallet::gate;

    wallet::options_validator validator(&options);
    validator.add_option("payload", new wallet::payload_validator)
             ->add_option("coin", new wallet::coin_validator)
             ->add_option("gas-coin", new wallet::coin_validator);

    if (!validator.validate()) {
        return;
    }

    repository data_repo;
    wallet::explorer::repository explorer_repo;

    wallet::prompt_manager pm;
    pm.add_field("pubkey", new wallet::pubkey_validator);
    pm.add_field("threshold", new wallet::amount_validator);
    pm.add_field("coin", new wallet::coin_validator);
    pm.prompt();

    minter::pubkey_t pubkey = minter::pubkey_t(pm.get("pubkey"));
    dev::bigdec18 threshold = dev::bigdec18(pm.get("threshold"));
    std::string coin = pm.get("coin");
    std::string payload = options.at("payload").as<std::string>();
    std::string gas_coin = options.at("gas-coin").as<std::string>();
    size_t index = options.at("index").as<size_t>();

    bool error = false;

    auto secret_data = m_storage->get_secret(index);

    std::cout << std::setprecision(std::numeric_limits<boost::multiprecision::cpp_dec_float<18>>::max_digits10);
    std::cout << "You are about to autodelegate:\n";
    fort::table table;

    table << "Account" << secret_data.get_address().to_string() << fort::endr;
    table << "Validator" << pubkey.to_string() << fort::endr;
    table << "Threshold" << fmt::format("{0} {1}", to_string(threshold), coin) << fort::endr;
    table << "Gas Coin" << gas_coin << fort::endr;
    table << "Payload" << payload << fort::endr;
    std::cout << table.to_string() << std::endl;

    if (!confirm("Is it OK?", true)) {
        std::cout << "Operation canceled" << "\n";
        return;
    }

    std::cout << "Process started..." << std::endl;

    bool running = true;

    while (running) {
        auto balance_task = explorer_repo.get_balance(secret_data.get_address());
        bool execute = false;
        dev::bigdec18 amount;

        balance_task
            ->error([](httb::response resp, wallet::explorer::base_result<wallet::explorer::balance_items> result) { })
            ->success([&execute, &threshold, &amount, &coin](wallet::explorer::base_result<wallet::explorer::balance_items> result) {
              for (const auto &balance: result.data.balances) {
                  if (toolboxpp::strings::equalsIgnoreCase(coin, balance.coin)) {
                      if ((balance.amount + tx_delegate_type::get_fee()) >= threshold) {
                          execute = true;
                          amount = balance.amount;
                      }
                  }
              }
            })
            ->execute()->handle().run();

        if (!execute) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }
        execute = false;

        tx_init_data init_data = data_repo.get_tx_init_data(secret_data.get_address(), error);

        if (error) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }

        auto tx_builder = new_tx();
        tx_builder->set_nonce(init_data.nonce);
        tx_builder->set_gas_price(init_data.gas);
        tx_builder->set_gas_coin(gas_coin);
        tx_builder->set_chain_id(minter::chain_id_str_map.at(std::string(MINTER_CHAIN)));
        tx_builder->set_payload(std::move(payload));
        auto data = tx_builder->tx_delegate();
        data->set_pub_key(pubkey);

        amount = amount - tx_delegate_type::get_fee(init_data.gas);

        data->set_stake(amount);
        data->set_coin(coin);

        auto tx = data->build();
        auto signedTx = secret_data.sign(tx);

        std::cout << "Delegating " << to_string(amount) << "... ";

        data_repo.send_tx(signedTx)
                 ->success([&init_data](base_result<tx_send_result> result) {
                   std::cout << " OK!" << std::endl;
                 })
                 ->error([](httb::response resp, base_result<tx_send_result> result) {
                   std::cout << " ERROR!\n";
                   print_error_tx(resp, result.error);
                 })
                 ->execute()
                 ->handle().run();

        // wait for next block
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}


void wallet::cmd::tx_controller::print_error_tx(const httb::response &resp, const wallet::gate::error_result &error) {
    if (!error.log.empty()) {
        wallet::term::print_error_message("Error: " + error.log);
    } else {
        std::cerr << resp.statusMessage << std::endl;
        std::cerr << resp.getBody() << std::endl;
    }
}

void wallet::cmd::tx_controller::print_success_tx(const minter::hash_t &hash) {
    std::cout << rang::fgB::green << "Tx successfully sent!" << rang::fg::reset << std::endl;
    std::cout << rang::fgB::green
              << std::string(MINTER_EXPLORER) + "transactions/" + hash.to_string()
              << rang::fg::reset
              << std::endl;
}

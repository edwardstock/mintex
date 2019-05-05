/*!
 * mintex.
 * app_account
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include <rang.hpp>
#include "wallet/term.h"
#include "wallet/app/account_controller.h"
#include "wallet/explorer/repository.h"
#include "mintex-tx/utils.h"

wallet::cmd::account_controller::account_controller(std::shared_ptr<const wallet::app> app)
        :command_controller(app) {

    CMD_BIND(account_controller, balance);
    CMD_BIND(account_controller, lastx);
}

std::string wallet::cmd::account_controller::usage() const {
    return "Minter Text Wallet";
}

template<typename T>
inline std::shared_ptr<T> get_tx_data(const std::shared_ptr<wallet::explorer::tx_result> &data) {
    return std::static_pointer_cast<T>(data);
}

#include <utility>
#include <type_traits>

template<typename T>
inline constexpr void print_field(const std::string &name, T data, uint32_t width) {
    std::cout << "\n";
    std::cout << std::setw(width) << name << ": " << data;
}

template<typename T>
inline constexpr void print_tab_field(const std::string &name, T data, uint32_t width) {
    std::cout << "\n\t";
    std::cout << std::setw(width) << name << ": " << data;
}

void wallet::cmd::account_controller::cmd_balance(const po::variables_map &) {
    using namespace wallet::explorer;
    using namespace mintex;
    using namespace mintex::utils;

    wallet::term::progress_indeterminate p;
    repository repo;

    balance_items balance_data;
    delegations_result_t delegations_data;

    const address_t address("Mxf07dc703fae3608abf29719d052a1b527b1dd9f8");

    p.start();

    auto balance_task = repo.get_balance(address.to_string())
            ->success([&p, &balance_data](wallet::explorer::base_result<wallet::explorer::balance_items> result){
                balance_data = result.data;
            })
            ->error([&p](httb::response response, wallet::explorer::base_result<wallet::explorer::balance_items> result) {
                std::cout << "error " << response.statusMessage << "\n";
            });

    auto dtask = repo.get_delegated(address.to_string())
            ->success([&delegations_data](base_result<delegations_result_t> res) {
                delegations_data = res.data;
            })
            ->error([&delegations_data](httb::response response, base_result<delegations_result_t> res){
                std::cout << "error " << response.statusMessage << "\n";
            });

    wallet::net::pair_task<
            base_result<balance_items>,
            base_result<delegations_result_t>
            > executor(balance_task, dtask);

    executor.execute();
    p.stop();

    std::stringstream coins_balance;
    {
        for(auto &item: balance_data.balances) {
            item.amount.precision(18);
            coins_balance << "\n\t" << std::setw(10) << item.coin << ": " << to_string(item.amount);
        }
    }

    std::stringstream delegated_balance;
    {
        std::unordered_map<std::string, dev::bigdec18> delegated_value_map;
        for(auto &item: delegations_data) {
            if(!delegated_value_map.count(item.coin)) {
                delegated_value_map.emplace(item.coin, item.value);
            } else {
                delegated_value_map[item.coin] += item.value;
            }
        }

        for(const auto& item: delegated_value_map) {
            delegated_balance << "\n\t" << std::setw(10) << item.first << ": " << to_string(item.second);
        }
    }

    std::stringstream validators;
    {
        std::unordered_map<std::string, delegated_item> tmp;
        for(const auto& item: delegations_data) {
            if(!tmp.count(item.pub_key.to_string())) {
                tmp.emplace(item.pub_key.to_string(), item);
            } else {
                tmp.at(item.pub_key.to_string()).value += item.value;
            }
        }

        for(const auto& item: tmp) {
            validators << "\n\t" << std::setw(66) << item.first << ": " << to_string(item.second.value) << " " << item.second.coin;
        }
    }



    std::cout << fmt::format(
R"(Address: {0}
Balance: {1}

Delegated sum: {2}

Validators: {3}
)",
            (std::string)balance_data.address,
            coins_balance.str(),
            delegated_balance.str(),
            validators.str()
);

}
void wallet::cmd::account_controller::cmd_lastx(const po::variables_map &opts) {
    using namespace wallet::explorer;
    using namespace mintex;
    using namespace mintex::utils;

    wallet::term::progress_indeterminate p;
    p.start();

    repository repo;

    uint32_t page = 1;
    uint32_t limit = 10;

    if(opts.count("page")) {
        page = opts.at("page").as<uint32_t>();
    }
    if(opts.count("limit")) {
        limit = opts.at("limit").as<uint32_t>();
    }

    const address_t address("Mxf07dc703fae3608abf29719d052a1b527b1dd9f8");

    auto tx_list_task = repo.get_transactions(address, limit, page);
    tx_list_task
        ->error([](httb::response response, base_result<tx_list_t> result) {
            std::cout << "error " << response.statusMessage << "\n";
        })
        ->complete([&p]{
            p.stop();
        });


    try {
        auto result = tx_list_task->execute()->handle().get();

        if(opts.count("short")) {
            for(const transaction_item& item: result.data) {
                std::cout << rang::fgB::yellow << item.hash;
                std::cout << " ";
                std::cout << rang::fgB::green << item.type;
                std::cout << " ";
                std::cout << rang::fgB::yellow << item.timestamp;
                std::cout << "\n" << rang::style::reset;
            }

        } else {
            for(const transaction_item& item: result.data) {
                std::cout << "\n==================================";
                print_field("#", item.txn, 13);
                print_field("sender", item.from, 13);
                print_field("hash", item.hash, 13);
                print_field("nonce", item.nonce, 13);
                print_field("block", item.block, 13);
                print_field("timestamp", item.timestamp, 13);
                print_field("fee", item.fee, 13);
                print_field("type", mintex::tx_type_names.at(item.type), 13);
                print_field("payload", item.payload, 13);
                print_field("data", "", 13);

                switch (item.type) {
                    case tx_type_val::send_coin: {
                        auto data = get_tx_data<tx_send_result>(item.data);
                        print_tab_field("Coin", data->coin, 11);
                        print_tab_field("Value", to_string(data->value), 11);
                        print_tab_field("Recipient", data->to, 11);
                        break;
                    }
                    case tx_type_val::sell_coin:
                    case tx_type_val::sell_all_coins:
                    case tx_type_val::buy_coin: {
                        auto data = get_tx_data<tx_convert_result>(item.data);
                        print_tab_field("Sell coin", data->coin_to_sell, 19);
                        print_tab_field("Buy coin", data->coin_to_buy, 19);
                        print_tab_field("Value buy", to_string(data->value_to_buy), 19);
                        print_tab_field("Value sell", to_string(data->value_to_sell), 19);
                        print_tab_field("Max value to sell", to_string(data->maximum_value_to_sell), 19);
                        print_tab_field("Min value to buy", to_string(data->minimum_value_to_buy), 19);
                        break;
                    }
                    case tx_type_val::create_coin: {
                        auto data = get_tx_data<tx_create_coin_result>(item.data);\
                        print_tab_field("Coin name", data->name, 17);
                        print_tab_field("Ticker", data->symbol, 17);
                        print_tab_field("Initial amount", to_string(data->initial_amount), 17);
                        print_tab_field("Initial reserve", to_string(data->initial_reserve), 17);
                        print_tab_field("CRR", data->symbol, 17);
                        break;
                    }
                    case tx_type_val::declare_candidacy: {
                        auto data = get_tx_data<tx_declare_candidacy_result>(item.data);
                        print_tab_field("Address", data->address, 12);
                        print_tab_field("Pub Key", data->pub_key, 12);
                        print_tab_field("Commission", data->commission, 12);
                        print_tab_field("Coin", data->coin, 12);
                        print_tab_field("Stake", to_string(data->stake), 12);
                        break;
                    }
                    case tx_type_val::delegate:
                    case tx_type_val::unbond: {
                        auto data = get_tx_data<tx_delegate_unbond_result>(item.data);
                        print_tab_field("Pub Key", data->pub_key, 9);
                        print_tab_field("Coin", data->coin, 9);
                        print_tab_field("Stake", to_string(data->stake), 9);
                        print_tab_field("Value", to_string(data->value), 9);
                        break;
                    }
                    case tx_type_val::redeem_check: {
                        auto data = get_tx_data<tx_redeem_check_result>(item.data);
                        break;
                    }
                    case tx_type_val::set_candidate_on:
                    case tx_type_val::set_candidate_off: {
                        auto data = get_tx_data<tx_set_candidate_on_off_result>(item.data);
                        break;
                    }
                    case tx_type_val::create_multisig: {
                        auto data = get_tx_data<tx_create_multisig_result>(item.data);
                        break;
                    }
                    case tx_type_val::multisend: {
                        auto data = get_tx_data<tx_multisend_result>(item.data);
                        break;
                    }
                    case tx_type_val::edit_candidate: {
                        auto data = get_tx_data<tx_edit_candidate_result>(item.data);
                        break;
                    }
                }

                std::cout << "\n";



            }
        }



        std::cout << "Printed count: " << result.data.size() << "\n";
        std::cout << "Page: " << result.meta.current_page << "\n";
        std::cout << "Total pages:" << result.meta.last_page << "\n";


    } catch ( nlohmann::detail::exception &e) {
        std::cerr << e.what() << std::endl;
    }


}

void wallet::cmd::account_controller::cmd_balance_options(boost::program_options::options_description &options) {
    options.add_options()
            ("index,i", po::value<uint32_t>()->default_value(0), "index of secret");
}
void
wallet::cmd::account_controller::cmd_lastx_options(boost::program_options::options_description &options) {
    options.add_options()
            ("number,n", po::value<uint32_t>()->default_value(10), "list size")
            ("short", "show in oneline format")
            ("page,p", po::value<uint32_t>()->default_value(1), "page number");
}
std::string wallet::cmd::account_controller::get_command_name() const {
    return "account";
}



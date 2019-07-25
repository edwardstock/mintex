/*!
 * mintex.
 * app_account
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include <boost/locale.hpp>
#include <rang.hpp>
#include <fort.hpp>
#include "wallet/term.h"
#include "wallet/app/account_controller.h"
#include "wallet/gate/repository.h"
#include "wallet/explorer/repository.h"
#include "wallet/liquidity/repository.h"
#include <minter/tx/utils.h>
#include <wallet/liquidity/base.h>

wallet::cmd::account_controller::account_controller(std::shared_ptr<wallet::app> app)
    : command_controller(app) {

    ACTION_BIND(account_controller, balance);
    ACTION_BIND(account_controller, lastx);
}

bool wallet::cmd::account_controller::before_action(const std::string &action, int argc, char **argv) {
    // todo: unify
    if (wallet::command_controller::before_action(action, argc, argv)) {
        m_storage = wallet::secret_storage::create(false, get_app()->get_settings());
        return true;
    }

    return false;
}

std::string wallet::cmd::account_controller::usage() const {
    return "Minter Text Wallet";
}

void wallet::cmd::account_controller::action_balance_options(boost::program_options::options_description &options) {
    options.add_options()
               ("index,i", po::value<size_t>(), "Account index")
               ("all", "Count balance for all existed accounts");
}

void wallet::cmd::account_controller::action_balance(const po::variables_map &opts) {
    using namespace minter;
    using namespace minter::utils;
    using namespace wallet::explorer;
    using namespace wallet::term;

    progress_indeterminate p;
    repository repo;
    wallet::gate::repository gate_repo;
    wallet::liquidity::repository lp_repo;


    std::unordered_map<address_t, std::vector<balance_item>> balance_data;
    delegations_result_t delegations_data;

    dev::bigdec18 all_actives_in_bip = dev::bigdec18("0");

    std::vector<address_t> addresses;

    if (opts.count("index")) {
        size_t index = opts.at("index").as<size_t>();
        addresses.push_back(m_storage->get_address(index));
    } else {
        auto all_addresses = m_storage->get_addresses();
        addresses.insert(addresses.begin(), all_addresses.begin(), all_addresses.end());
    }

    p.start();

    dev::bigdec18 coin_price("0.32");
    coin_price.precision(2);

    auto get_coin_price_task = lp_repo.get_price()
        ->success([&coin_price](wallet::liquidity::base_result<wallet::liquidity::bip_price> res) {
            coin_price = dev::bigdec18(res.data.price) / dev::bigdec18("10000");
        })
        ->error([](httb::response resp, wallet::liquidity::base_result<wallet::liquidity::bip_price> res) {
          std::cerr << "error " << resp.statusMessage << "\n";
        });

    get_coin_price_task->execute()->handle().run();


    for (const auto &address: addresses) {
        auto balance_task = repo.get_balance(address.to_string())
                                ->success([&p, &balance_data](wallet::explorer::base_result<wallet::explorer::balance_items> result) {
                                  balance_data.emplace(result.data.address, result.data.balances);
                                })
                                ->error([&p](httb::response response,
                                             wallet::explorer::base_result<wallet::explorer::balance_items> result) {
                                  std::cout << "error " << response.statusMessage << "\n";
                                });

        auto delegated_task = repo.get_delegated(address.to_string())
                                  ->success([&delegations_data](base_result<delegations_result_t> res) {
                                    delegations_data.insert(delegations_data.begin(), res.data.begin(), res.data.end());
                                  })
                                  ->error([&delegations_data](httb::response response,
                                                              base_result<delegations_result_t> res) {
                                    std::cout << "error " << response.statusMessage << "\n";
                                  });

        wallet::net::pair_task<
            base_result<balance_items>,
            base_result<delegations_result_t>
        > executor(balance_task, delegated_task);

        executor.execute();
    }

    // Balances
    fort::table balance_table;
    balance_table << fort::header;
    balance_table << "Coin" << "Balance" << fort::endr;
    {
        for (auto &balances: balance_data) {
            balance_table << balances.first.to_string() << "" << fort::endr;

            for (const auto &item: balances.second) {
                balance_table << item.coin;

                if (!toolboxpp::strings::equalsIgnoreCase(std::string(MINTER_COIN), item.coin)) {
                    gate_repo.get_exchange_sell_currency(item.coin, item.amount, std::string(MINTER_COIN))
                             ->success([&item, &balance_table, &all_actives_in_bip](wallet::gate::base_result<wallet::gate::exchange_sell_value> result) {
                               all_actives_in_bip += minter::utils::humanize_value(result.data.will_get);
                               balance_table
                                   << fmt::format("{0} (~{1} {2})",
                                                  to_string(item.amount),
                                                  minter::utils::to_string_lp(minter::utils::humanize_value(result.data
                                                                                                                  .will_get)),
                                                  std::string(MINTER_COIN)
                                   );
                             })
                             ->error([&balance_table, &item](httb::response resp,
                                                             wallet::gate::base_result<wallet::gate::exchange_sell_value> result) {
                               std::cerr << resp.getBody() << std::endl;
                               balance_table << to_string(item.amount);
                             })
                             ->execute()->handle().run();
                } else {
                    all_actives_in_bip += item.amount;
                    balance_table << to_string(item.amount);
                }

                balance_table << fort::endr;
            }
        }
    }

    // Delegated
    std::unordered_map<std::string, delegated_item> delegated_stakes;
    fort::table validators_table;
    validators_table << fort::header;
    validators_table << "Pub Key" << "Coin" << "Delegated Stake" << fort::endr;
    {
        for (const auto &item: delegations_data) {
            if (!delegated_stakes.count(item.pub_key.to_string())) {
                delegated_stakes.emplace(item.pub_key.to_string(), item);
            } else {
                delegated_stakes.at(item.pub_key.to_string()).value += item.value;
            }
        }
        for (const auto &item: delegated_stakes) {
            validators_table << item.first << item.second.coin << to_string(item.second.value) << fort::endr;
        }
    }

    // Summary delegated
    std::unordered_map<std::string, dev::bigdec18> summary_stake;
    fort::table validators_sum_table;
    validators_sum_table << fort::header;
    validators_sum_table << "Coin" << "Delegated Stake" << fort::endr;
    {
        for (const auto &item: delegations_data) {
            if (!summary_stake.count(item.coin)) {
                summary_stake.emplace(item.coin, item.value);
            } else {
                summary_stake.at(item.coin) += item.value;
            }
        }

        for (const auto &item: summary_stake) {

            validators_sum_table << item.first;
            if (!toolboxpp::strings::equalsIgnoreCase(std::string(MINTER_COIN), item.first)) {
                gate_repo.get_exchange_sell_currency(item.first, item.second, std::string(MINTER_COIN))
                         ->success([&item, &validators_sum_table, &all_actives_in_bip](wallet::gate::base_result<wallet::gate::exchange_sell_value> result) {
                           all_actives_in_bip += minter::utils::humanize_value(result.data.will_get);
                           validators_sum_table
                               << fmt::format("{0} (~{1} {2})",
                                              to_string(item.second),
                                              minter::utils::to_string_lp(minter::utils::humanize_value(result.data
                                                                                                              .will_get)),
                                              std::string(MINTER_COIN)
                               );
                         })
                         ->error([&validators_sum_table, &item](httb::response resp,
                                                                wallet::gate::base_result<wallet::gate::exchange_sell_value> result) {
                           std::cerr << resp.getBody() << std::endl;
                           validators_sum_table << to_string(item.second);
                         })
                         ->execute()->handle().run();
            } else {
                validators_sum_table << to_string(item.second);
                all_actives_in_bip += item.second;
            }

            validators_sum_table << fort::endr;
        }
    }

    p.stop();

    // Common
    wallet::term::print_success_message("Balance");
    std::cout << balance_table.to_string();

    // Delegated
    wallet::term::print_success_message("Validators");
    if (delegated_stakes.size()) {
        std::cout << validators_table.to_string();
    } else {
        validators_table << "Nothing to show" << fort::endr;
        validators_table.cell(1, 0).set_cell_span(3);
        validators_table.cell(1, 0).set_cell_text_align(fort::text_align::center);
        std::cout << validators_table.to_string();
    }

    // Summary delegated
    wallet::term::print_success_message("Summary Delegated");
    if (summary_stake.size()) {
        std::cout << validators_sum_table.to_string();
    } else {
        validators_sum_table << "Nothing to show" << fort::endr;
        validators_sum_table.cell(1, 0).set_cell_span(3);
        validators_sum_table.cell(1, 0).set_cell_text_align(fort::text_align::center);
        std::cout << validators_sum_table.to_string();
    }

    // All actives
    fort::table all_actives_table;
    all_actives_table << fort::header << "Coin" << "Volume" << fort::endr;
    all_actives_table << MINTER_COIN << to_string_lp(all_actives_in_bip) << fort::endr;
    all_actives_table << "USD" << to_string_lp(all_actives_in_bip * dev::bigdec18(coin_price)) << fort::endr;
    all_actives_table << "RUB" << to_string_lp(((all_actives_in_bip * dev::bigdec18(coin_price)) * dev::bigdec18("64"))) << fort::endr;

    all_actives_table.cell(0, 1).set_cell_text_align(fort::text_align::right);
    all_actives_table.cell(1, 1).set_cell_text_align(fort::text_align::right);
    all_actives_table.cell(2, 1).set_cell_text_align(fort::text_align::right);

    wallet::term::print_success_message("All volumes (including frozen) in " + std::string(MINTER_COIN) + "s");
    std::cout << all_actives_table.to_string();

}

void wallet::cmd::account_controller::action_lastx_options(boost::program_options::options_description &options) {
    options.add_options()
               ("index,i", po::value<size_t>()->default_value(0), "Account index")
               ("limit,l", po::value<uint32_t>()->default_value(10), "List size")
               ("page,p", po::value<uint32_t>()->default_value(1), "Page number")
               ("short", "show in oneline format");
}

void wallet::cmd::account_controller::action_lastx(const po::variables_map &opts) {
    using namespace wallet::explorer;
    using namespace minter;
    using namespace minter::utils;
    using namespace wallet::term;

    progress_indeterminate p;
    p.start();

    repository repo;

    uint32_t page = 1;
    uint32_t limit = 10;

    if (opts.count("page")) {
        page = opts.at("page").as<uint32_t>();
    }
    if (opts.count("limit")) {
        limit = opts.at("limit").as<uint32_t>();
    }

    size_t index = opts.at("index").as<size_t>();
    const address_t address = m_storage->get_address(index);

    auto tx_list_task = repo.get_transactions(address, page, limit);
    tx_list_task
        ->error([](httb::response response, base_result<tx_list_t> result) {
          std::cout << "error " << response.statusMessage << "\n";
        })
        ->complete([&p] {
          p.stop();
        });

    try {
        wallet::explorer::base_result<wallet::explorer::tx_list_t> result;
        tx_list_task->success([&result](wallet::explorer::base_result<wallet::explorer::tx_list_t> res){
            result = res;
        })->execute()->handle().run();

        if (opts.count("short")) {
            for (const transaction_item &item: result.data) {
                std::cout << rang::fgB::yellow << item.hash;
                std::cout << " ";
                std::cout << rang::fgB::green << item.type;
                std::cout << " ";
                std::cout << rang::fgB::yellow << item.timestamp;
                std::cout << "\n" << rang::style::reset;
            }

        } else {
            for (const transaction_item &item: result.data) {
                std::cout << "\n==================================";
                print_field("#", item.txn, 13);
                print_field("sender", item.from, 13);
                print_field("hash", item.hash, 13);
                print_field("nonce", item.nonce, 13);
                print_field("block", item.block, 13);
                print_field("timestamp", item.timestamp, 13);
                print_field("fee", item.fee, 13);
                print_field("type", minter::tx_type_names.at(item.type), 13);
                print_field("payload", item.payload, 13);
                print_field("data", "", 13);

                switch (item.type) {
                    case tx_type_val::send_coin: {
                        auto data = std::static_pointer_cast<tx_send_result>(item.data);
                        print_tab_field("Coin", data->coin, 11);
                        print_tab_field("Value", to_string(data->value), 11);
                        print_tab_field("Recipient", data->to, 11);
                        break;
                    }
                    case tx_type_val::sell_coin:
                    case tx_type_val::sell_all_coins:
                    case tx_type_val::buy_coin: {
                        auto data = std::static_pointer_cast<tx_convert_result>(item.data);
                        print_tab_field("Sell coin", data->coin_to_sell, 19);
                        print_tab_field("Buy coin", data->coin_to_buy, 19);
                        print_tab_field("Value buy", to_string(data->value_to_buy), 19);
                        print_tab_field("Value sell", to_string(data->value_to_sell), 19);
                        print_tab_field("Max value to sell", to_string(data->maximum_value_to_sell), 19);
                        print_tab_field("Min value to buy", to_string(data->minimum_value_to_buy), 19);
                        break;
                    }
                    case tx_type_val::create_coin: {
                        auto data = std::static_pointer_cast<tx_create_coin_result>(item.data);\
                        print_tab_field("Coin name", data->name, 17);
                        print_tab_field("Ticker", data->symbol, 17);
                        print_tab_field("Initial amount", to_string(data->initial_amount), 17);
                        print_tab_field("Initial reserve", to_string(data->initial_reserve), 17);
                        print_tab_field("CRR", data->symbol, 17);
                        break;
                    }
                    case tx_type_val::declare_candidacy: {
                        auto data = std::static_pointer_cast<tx_declare_candidacy_result>(item.data);
                        print_tab_field("Address", data->address, 12);
                        print_tab_field("Pub Key", data->pub_key, 12);
                        print_tab_field("Commission", data->commission, 12);
                        print_tab_field("Coin", data->coin, 12);
                        print_tab_field("Stake", to_string(data->stake), 12);
                        break;
                    }
                    case tx_type_val::delegate:
                    case tx_type_val::unbond: {
                        auto data = std::static_pointer_cast<tx_delegate_unbond_result>(item.data);
                        print_tab_field("Pub Key", data->pub_key, 9);
                        print_tab_field("Coin", data->coin, 9);
                        print_tab_field("Stake", to_string(data->stake), 9);
                        print_tab_field("Value", to_string(data->value), 9);
                        break;
                    }
                    case tx_type_val::redeem_check: {
                        auto data = std::static_pointer_cast<tx_redeem_check_result>(item.data);
                        print_tab_field("Raw Check", (data->raw_check), 11);
                        print_tab_field("Proof", (data->raw_check), 11);
                        print_tab_field("Stake", to_string(data->stake), 11);
                        print_tab_field("Value", to_string(data->value), 11);

                        print_tab_field("Check Data:", "", 11);

                        print_tab_field("Nonce", data->check.nonce, 7, 2);
                        print_tab_field("Nonce", data->check.due_block, 7, 2);
                        print_tab_field("Coin", data->check.coin, 7, 2);
                        print_tab_field("Value", to_string(data->check.value), 7, 2);
                        print_tab_field("Sender", data->check.sender, 7, 2);
                        break;
                    }
                    case tx_type_val::set_candidate_on:
                    case tx_type_val::set_candidate_off: {
                        auto data = std::static_pointer_cast<tx_set_candidate_on_off_result>(item.data);
                        print_tab_field("Pub Key", data->pub_key, 11);
                        break;
                    }
                    case tx_type_val::create_multisig: {
//                        auto pre = std::static_pointer_cast<tx_create_multisig_result>(item.data);
                        break;
                    }
                    case tx_type_val::multisend: {
                        auto base = std::static_pointer_cast<tx_multisend_result>(item.data);
                        print_tab_field("Recipients", base->items.size(), 12);
                        size_t i = 0;
                        for (const auto &data: base->items) {
                            print_tab_field("#", i + 1, 11, 2);
                            print_tab_field("Coin", data.coin, 11, 2);
                            print_tab_field("Value", to_string(data.value), 11, 2);
                            print_tab_field("Recipient", data.to, 11, 2);

                            i++;
                        }
                        break;
                    }
                    case tx_type_val::edit_candidate: {
                        auto data = std::static_pointer_cast<tx_edit_candidate_result>(item.data);
                        print_tab_field("Pub Key", data->pub_key, 16);
                        print_tab_field("Reward Address", data->reward_address, 16);
                        print_tab_field("Owner Address", data->owner_address, 16);
                        break;
                    }
                }

                std::cout << "\n";
            }
        }

        std::cout << "Printed count: " << result.data.size() << "\n";
        std::cout << "Page: " << result.meta.current_page << "\n";
        std::cout << "Total pages:" << result.meta.last_page << "\n";

    } catch (nlohmann::detail::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

std::string wallet::cmd::account_controller::get_command_name() const {
    return "account";
}
std::unordered_map<std::string, std::string> wallet::cmd::account_controller::get_actions_descriptions() const {
    return std::unordered_map<std::string, std::string>{
        {"balance", "Print account balance"},
        {"lastx",   "Print last account transactions"}
    };
}



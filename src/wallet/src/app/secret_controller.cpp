/*!
 * mintex.
 * secret_controller
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include <string>
#include <rang.hpp>
#include <toolboxpp.hpp>
#include <fort.hpp>

#include "wallet/app/secret_controller.h"
#include "wallet/kv_storage.h"
#include "wallet/crypto.h"
#include "wallet/term.h"

wallet::cmd::secret_controller::secret_controller(const std::shared_ptr<wallet::app> &app) : command_controller(app) {
    ACTION_BIND(secret_controller, add);
    ACTION_BIND(secret_controller, generate);
    ACTION_BIND(secret_controller, list);
    ACTION_BIND(secret_controller, remove);
    ACTION_BIND(secret_controller, purge);
    ACTION_BIND(secret_controller, export);
    ACTION_BIND(secret_controller, import);
}

bool wallet::cmd::secret_controller::before_action(const std::string &action, int argc, char **argv) {
    if (wallet::command_controller::before_action(action, argc, argv)) {
        if (!toolboxpp::strings::equalsIgnoreCase(action, "list")) {
            m_storage = secret_storage::create(true, get_app()->get_settings());
        }

        return true;
    }

    return false;
}

void wallet::cmd::secret_controller::action_add(const boost::program_options::variables_map &options) {
    if (!options.count("mnemonic")) {
        std::cerr << "You must choose one of variants how to add your secret data:\n";
        std::cerr << "Use --mnemonic to add mnemonic phrase or private key directly\n";
        return;
    }

    uint32_t derive_index = options.at("derive-index").as<uint32_t>();
    std::string mnemonic = options.at("mnemonic").as<std::string>();

    secret_data sd = secret_data::from_mnemonic(mnemonic, derive_index);
    m_storage->add(sd);

    std::cout << "Successfully added secret with address: " << rang::fgB::green << sd.get_address() << "\n";
}
void wallet::cmd::secret_controller::action_add_options(boost::program_options::options_description &options) {
    options.add_options()
               ("mnemonic,m", po::value<std::string>()->required(),"Add new secret from mnemonic phrase (12 words)")
               ("derive-index", po::value<uint32_t>()->default_value(0), "Pass derive index for private key");
}
void wallet::cmd::secret_controller::action_generate_options(boost::program_options::options_description &options) {
    options.add_options()
               ("derive-index", po::value<uint32_t>()->default_value(0), "Pass derive index for private key");
}
void wallet::cmd::secret_controller::action_generate(const boost::program_options::variables_map &options) {
    uint32_t derive_index = options.at("derive-index").as<uint32_t>();
    secret_data generated;

    wallet::term::print_message_block(
        "Attention, Please",
        "Before creating a secret, make sure nobody looks at your monitor.",
        rang::fg::black, rang::bg::yellow
    );

    if (!toolboxpp::console::confirm("I Understand", true)) {
        std::cout << "Operation interrupted\n";
        return;
    }
    std::cout << std::endl;

    auto str_repeat = [](std::string &&s, size_t n) {
      std::stringstream ss;
      for (size_t i = 0; i < n; i++) {
          ss << s;
      }

      return ss.str();
    };

    auto show_mnem = [&str_repeat](secret_data &sd) {
      wallet::term::print_colored_message(sd.mnemonic, rang::fg::black, rang::bg::green);
      std::cout << std::endl;
    };

    generated = m_storage->generate(derive_index);
    show_mnem(generated);

    while (!toolboxpp::console::confirm("Do you like this mnemonic? Answer \"yes\" if you do, or \"no\" to generate new one",
                                        false)) {
        generated = m_storage->generate(derive_index);
        show_mnem(generated);
    }

    m_storage->add(generated);
    std::cout << "Successfully added secret with address: " << rang::fgB::green << generated.get_address() << "\n";
}
void wallet::cmd::secret_controller::action_list_options(boost::program_options::options_description &options) {
    options.add_options()
               ("index,i", "Account index. Use this option to show only specified account")
               ("password", "Type password to see full secret data, not only address");
}

void wallet::cmd::secret_controller::action_list(const boost::program_options::variables_map &options) {
    bool show_secret = options.count("password");
    bool show_index = false;
    size_t index = 0;
    if(options.count("index")) {
        show_index = true;
        index = options.at("index").as<size_t>();
    }



    if (show_secret) {
        m_storage = wallet::secret_storage::create(get_app()->get_settings()->pwd_secured, get_app()->get_settings());

        auto items = m_storage->get_secrets();
        if (!items.size()) {
            std::cout << rang::fgB::yellow << "No one secret added\n";
            return;
        }

        size_t i = 0;
        for (const auto &item: items) {
            if(show_index && index != i) {
                continue;
            }
            fort::table table;
            table << "#" << i << fort::endr;
            table << "Mnemonic" << item.mnemonic << fort::endr;
            table << "Address" << item.get_address().to_string() << fort::endr;
            table << "Priv Key" << item.priv_key.toHex() << fort::endr;
            table << "Pub Key" << item.get_pub_key().to_string_no_prefix() << fort::endr;
            table << "Derive Idx" << item.derive_index << fort::endr;

            i++;
            std::cout << table.to_string() << std::endl;
        }

    } else {
        // no password
        m_storage = wallet::secret_storage::create(false, get_app()->get_settings());

        auto items = m_storage->get_addresses();
        if (!items.size()) {
            std::cout << rang::fgB::yellow << "No one secret added\n";
            return;
        }

        size_t i = 0;
        for (const auto &item: items) {
            if(show_index && index != i) {
                continue;
            }
            fort::table table;
            table << "#" << i << fort::endr;
            table << "Address" << item.to_string() << fort::endr;
            std::cout << table.to_string() << std::endl;
        }
    }
}

void wallet::cmd::secret_controller::action_remove_options(boost::program_options::options_description &options) {
    options.add_options()
               ("index,i", po::value<size_t>()->required(), "Account index");
}
void wallet::cmd::secret_controller::action_remove(const boost::program_options::variables_map &options) {
    using namespace toolboxpp::console;
    size_t index = options.at("index").as<size_t>();
    auto address = m_storage->get_address(index);

    if(!confirm(fmt::format("Do you really want to remove account {0} {1}", index, address.to_string()))) {
        std::cout << "Operation canceled" << "\n";
        return;
    }

    m_storage->remove(index);
    std::cout << "Account was deleted!" << "\n";

}
void wallet::cmd::secret_controller::action_purge(const boost::program_options::variables_map &options) {
    using namespace toolboxpp::console;

    if(!confirm("Do you really want to remove all accounts from wallet?")) {
        std::cout << "Operation canceled" << "\n";
        return;
    }

    m_storage->purge();
    std::cout << "Operation completed!" << "\n";

}
void wallet::cmd::secret_controller::action_purge_options(boost::program_options::options_description &options) {
}
void wallet::cmd::secret_controller::action_export_options(boost::program_options::options_description &options) {
    options.add_options()
               ("encrypt", "Export encrypted data");
}
void wallet::cmd::secret_controller::action_export(const boost::program_options::variables_map &options) {

}

void wallet::cmd::secret_controller::action_import(const boost::program_options::variables_map &options) {

}
void wallet::cmd::secret_controller::action_import_options(boost::program_options::options_description &options) {

}

std::string wallet::cmd::secret_controller::usage() const {
    return std::string();
}
std::string wallet::cmd::secret_controller::get_command_name() const {
    return "secret";
}
std::unordered_map<std::string, std::string> wallet::cmd::secret_controller::get_actions_descriptions() const {
    return std::unordered_map<std::string, std::string>{
        {"add",      "Add new private key or mnemonic to wallet"},
        {"generate", "Create new account"},
        {"list",     "Print existed accounts"},
        {"view",     "Show specified account"},
        {"remove",   "Remove specified account from wallet"},
        {"purge",    "Delete ALL existing accounts from wallet"},
        {"export",   "Export accounts"},
        {"import",   "Import accounts"},
    };
}


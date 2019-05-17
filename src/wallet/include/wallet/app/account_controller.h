/*!
 * mintex.
 * app_account
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_ACCOUNT_CONTROLLER_H
#define MINTEX_ACCOUNT_CONTROLLER_H

#include <unordered_map>
#include <memory>
#include "app.h"

#include "wallet/data/secret_storage.h"
namespace wallet {
namespace cmd {

class account_controller: public virtual wallet::command_controller {
public:
    enum cmd {
        balance,
        last_transactions
    };
    account_controller(std::shared_ptr<wallet::app> app);
    std::string get_command_name() const override;
    std::unordered_map<std::string, std::string> get_actions_descriptions() const override;

    std::string usage() const override;

    bool before_action(const std::string &action, int argc, char **argv) override;
    ACTION_DECLARE(balance);
    ACTION_DECLARE(lastx);
private:
     std::unique_ptr<wallet::secret_storage> m_storage;
};

}
}

#endif //MINTEX_ACCOUNT_CONTROLLER_H

/*!
 * mintex.
 * app_tx
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TX_CONTROLLER_H
#define MINTEX_TX_CONTROLLER_H

#include <utility>
#include "wallet/gate/repository.h"
#include "wallet/app/app.h"

namespace wallet {
namespace cmd {

class tx_controller : public wallet::command_controller {
 public:

    tx_controller(const std::shared_ptr<app> &app);
    std::string usage() const override;
    std::string get_command_name() const override;
    std::unordered_map<std::string, std::string> get_actions_descriptions() const override;

    bool before_action(const std::string &action, int argc, char **argv) override;

    ACTION_DECLARE(send);
    ACTION_DECLARE(buy);
    ACTION_DECLARE(sell);
    ACTION_DECLARE(delegate);
    ACTION_DECLARE(unbond);
    ACTION_DECLARE(autodelegate);
//    ACTION_DECLARE(dc);
//    ACTION_DECLARE(candidate);

 private:
    std::unique_ptr<wallet::secret_storage> m_storage;

    static void print_error_tx(const httb::response &resp, const wallet::gate::error_result &error);
    static void print_success_tx(const minter::hash_t &hash);


};

} // cmd
} // wallet

#endif //MINTEX_TX_CONTROLLER_H

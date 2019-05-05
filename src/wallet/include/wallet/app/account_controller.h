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

#include "app.h"

#include <unordered_map>
namespace wallet {
namespace cmd {

class account_controller: public virtual wallet::command_controller {
public:
    enum cmd {
        balance,
        last_transactions
    };
    account_controller(std::shared_ptr<const wallet::app> app);
    std::string get_command_name() const override;

    std::string usage() const override;

    CMD_DECLARE(balance);
    CMD_DECLARE(lastx);
private:

};

}
}

#endif //MINTEX_ACCOUNT_CONTROLLER_H

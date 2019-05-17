/*!
 * mintex.
 * app_secret
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_SECRET_CONTROLLER_H
#define MINTEX_SECRET_CONTROLLER_H

#include "wallet/app/app.h"
#include "wallet/kv_storage.h"

namespace wallet {
namespace cmd {

class secret_controller: public command_controller {
 public:
    secret_controller(const std::shared_ptr<app> &app);
    std::string usage() const override;
    std::string get_command_name() const override;
    std::unordered_map<std::string, std::string> get_actions_descriptions() const override;

    bool before_action(const std::string &action, int argc, char **argv) override;

    //secret generate - generate new secret key
    ////    secret add "your mnemonic phrase" - mnemonic phrase contains 16 words
    ////    secret add _hex_private_key_string_ - 32 bytes hex private key
    ////    secret list - list secrets
    ////    secret remove N - remove secret by index from list
    ////    secret purge - remove all secrets
    ////    secret export - export secret data to file
    ////    secret import - import secret data to file

    ACTION_DECLARE(add);
    ACTION_DECLARE(generate);
    ACTION_DECLARE(list);
    ACTION_DECLARE(remove);
    ACTION_DECLARE(purge);
    ACTION_DECLARE(export);
    ACTION_DECLARE(import);

 private:
    std::unique_ptr<wallet::secret_storage> m_storage;
};


} //cmd
} //wallet

#endif //MINTEX_SECRET_CONTROLLER_H

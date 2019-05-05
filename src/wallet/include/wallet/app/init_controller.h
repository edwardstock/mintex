/*!
 * mintex.
 * app_init
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_INIT_CONTROLLER_H
#define MINTEX_INIT_CONTROLLER_H

#include "app.h"

namespace wallet {
namespace cmd {

class init_controller: public wallet::command_controller {
public:
    init_controller(std::shared_ptr<const wallet::app> app);
    std::string usage() const override;
    std::string get_command_name() const override;
};

} // app
} // wallet

#endif //MINTEX_INIT_CONTROLLER_H

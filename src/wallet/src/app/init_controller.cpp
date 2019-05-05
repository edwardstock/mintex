#include <boost/beast/core/string.hpp>
/*!
 * mintex.
 * app_init
 *
 * \date 02.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include "wallet/app/init_controller.h"
wallet::cmd::init_controller::init_controller(std::shared_ptr<const wallet::app> app)
        :command_controller(app) {

}
std::string wallet::cmd::init_controller::usage() const {
    return std::__cxx11::string();
}
std::string wallet::cmd::init_controller::get_command_name() const {
    return "init";
}

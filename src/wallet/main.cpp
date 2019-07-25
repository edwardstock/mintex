#include <iostream>

#include <minter/address.h>
#include "wallet/app/app.h"
#include "wallet/base_repository.h"
#include "wallet/explorer/repository.h"

#include <boost/asio/io_context.hpp>

#include <functional>
#include <httb/httb.h>
#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
    auto app = wallet::app::create();
    app->run(argc, argv);

//    wallet::explorer::repository repo;
//
//    minter::address_t address("Mxa386ba04b5cdc6cfb77917dd046492e34233afd3");
//    auto task = repo.get_balance(address);
//    task->success([](wallet::explorer::base_result<wallet::explorer::balance_items> res) {
//      std::cerr << "success" << std::endl;
//    });
//    task->error([](httb::response resp, wallet::explorer::base_result<wallet::explorer::balance_items> res) {
//       std::cerr << resp.statusMessage << std::endl;
//    });
//
//    task->execute();
    return 0;
}
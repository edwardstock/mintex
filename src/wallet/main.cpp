#include <iostream>
#include <cstdlib>
#include <boost/program_options.hpp>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

#include <boost/filesystem.hpp>
#include "wallet/app/app.h"
#include "wallet/resp_gen.h"
#include "wallet/explorer/repository.h"
#include "wallet/explorer/results.hpp"
#include "mintex-tx/utils.h"
#include <rang.hpp>
namespace po = boost::program_options;
namespace fs = boost::filesystem;
using namespace nlohmann;

using namespace boost::multiprecision;
int main(int argc, char** argv) {
    auto app = wallet::app::create();
    app->run(argc, argv);
//

//    mintex::privkey_t pk("40e6e1c7cc2bce05876b9bf4f03a88ecb2d7398a3ac94472e721d60c42c251b0");
//
//    auto pub_key = pk.get_public_key();
//    address_t add = address_t(pub_key);
//
//    std::cout << add << std::endl;
//    fs::path homedir(getenv("HOME"));
//
//    for(fs::directory_entry& entry: fs::directory_iterator(homedir)) {
//        std::cout << entry.path() << "\n";
//    }



//    wallet::explorer::explorer_repository repo;
//    auto res = repo.get_balance("Mxf07dc703fae3608abf29719d052a1b527b1dd9f8");
//    res.progress([](){
//        std::cout << "Loading...\n";
//    }).success([](wallet::explorer::base_result<wallet::explorer::balance_items> result){
//        std::cout << result.data.address << std::endl;
//    })
//    .error([](wallet::explorer::base_result<wallet::explorer::balance_items> result) {
//
//    })
//    .complete([]{
//        std::cout << "Loaded\n";
//    });
//
//    res.execute();


//    repo.getBalance("Mxf07dc703fae3608abf29719d052a1b527b1dd9f8",
//            [&exit](wallet::explorer::base_result<wallet::explorer::balance_items> balance) {
//
//                std::cout << "Loaded\n";
//                std::cout << balance.data.address << std::endl;
//
//                exit = true;
//            });
//
//    while (!exit) {
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }

    return 0;
}
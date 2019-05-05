/*!
 * mintex.
 * transaction_item_deser_test.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <wallet/explorer/transaction.h>

TEST(ExplorerTransaciton, DeserializeHistoryTransaction) {
    nlohmann::json j = R"({
  "data": [
    {
      "txn": 17653,
      "hash": "Mtc923c18c1d842dcba262c63df22003d5ee4e683dc2927acf9580f083ad9e193e",
      "nonce": 113,
      "block": 119,
      "timestamp": "2019-02-19T00:00:00+03:00",
      "fee": "10.000000000000000000",
      "type": 1,
      "payload": "Super tx from developers",
      "from": "Mx184ac726059e43643e67290666f7b3195093f870",
      "data": {
        "coin": "MNT",
        "to": "Mxee81347211c72524338f9680072af90744333146",
        "value": "12.357"
      }
    }
  ],
  "links": {
    "first": "https://testnet.explorer.minter.network/blocks?page=1",
    "last": "https://testnet.explorer.minter.network/blocks?page=10",
    "prev": "https://testnet.explorer.minter.network/blocks?page=1",
    "next": "https://testnet.explorer.minter.network/blocks?page=2"
  },
  "meta": {
    "current_page": 2,
    "last_page": 10,
    "path": "https://testnet.explorer.minter.network",
    "per_page": 50,
    "total": 16434
  }
})"_json;

    using namespace wallet::explorer;
//    transaction_item<> send_result = j;
}
 
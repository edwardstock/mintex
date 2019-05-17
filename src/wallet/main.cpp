#include <iostream>

#include "wallet/app/app.h"
#include "wallet/explorer/repository.h"

#include <unordered_map>
#include <string>
#include <sodium.h>
#include <algorithm>
#include <fort.hpp>

void some();

int main(int argc, char **argv) {
    auto app = wallet::app::create();
    app->run(argc, argv);

//some();
    return 0;
}

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";


static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

struct vote {
  dev::bigint txn;
  std::string address;
  dev::bigdec18 amount;
  dev::bigdec18 balance;

  bool operator<(const vote& other) noexcept {
      return amount < other.amount;
  }

  bool operator>(const vote& other) noexcept {
      return amount > other.amount;
  }
};

#include <queue>

void some() {


    using namespace wallet::explorer;
    mintex::address_t addr("Mx00000000000000000000000000000000000000a1");


    wallet::explorer::repository repo;
    size_t page = 1;
    size_t total_pages = -1;
    std::unordered_map<std::string, vote> stakes;

    auto h = [&total_pages, &stakes](base_result<tx_list_t> result){
      total_pages = result.meta.last_page;
      for(const auto& item: result.data) {
          if(item.type == tx_type_val::send_coin) {
              if(item.payload != "") {
                  try {
                      std::string tmp = base64_decode(item.payload);
                      auto num = dev::bigdec18(tmp);
                      if(num > dev::bigdec18("1.00") || num < dev::bigdec18("0.01")) {
                          continue;
                      }
                      if(!stakes.count(item.from.to_string())) {
                          stakes.emplace(item.from.to_string(), vote{item.txn, item.from.to_string(), num});
                      } else {
                          if(stakes[item.from.to_string()].txn < item.txn) {
                              stakes[item.from.to_string()] = vote{item.txn, item.from.to_string(), num};
                          }

                      }
                  } catch(const std::exception &e) {

                  }
              }
          }
      }
    };

    auto task = repo.get_transactions(addr, page);

    while(page <= total_pages || total_pages < 0) {
        page++;
        std::cout << "Getting page " << page << "\n";

        auto tmp_task = repo.get_transactions(addr, page);
        tmp_task->success(h)->execute()->handle().wait();
    }


    size_t i = stakes.size();
    size_t threads = 8;
    size_t working = 0;
    std::queue<std::shared_ptr<response_task<base_result<balance_items>>>> tasks;

    for(auto &stake: stakes) {
        std::cout << "Get balance for " << stake.first << "; left: " << i << "\n";
        auto balance_task = repo.get_balance(stake.first)->success([&working, &stake](base_result<balance_items> result) {
          for(const auto& item: result.data.balances) {
              if(item.coin == "MNT") {
                  stake.second.balance = item.amount;
              }
          }

          working--;
        });
        balance_task->execute();
        tasks.push(std::move(balance_task));
        working++;


        if(working == threads) {
            tasks.back()->handle().wait();
        }

        i--;
    }

    std::vector<vote> votes;

    std::cout << "\n\n";

    for(auto &item: stakes) {
        votes.push_back(item.second);
    }

    std::sort(votes.begin(), votes.end(), [](vote l, vote r) {
      return l.balance > r.balance;
    });

    for(const auto& item: votes) {
        std::cout << item.address << ": " << mintex::utils::to_string(item.balance) << ": " << mintex::utils::to_string(item.amount) << "\n";
    }
}
/*!
 * mintex.
 * address_book_test.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <gtest/gtest.h>
#include <string>
#include <boost/filesystem.hpp>
#include "wallet/data/address_book.h"

TEST(AddressBook, TestAll) {
    namespace fs = boost::filesystem;
    std::system("rm -rf /tmp/test.db");

    using namespace wallet;
    leveldb::Options opts;
    auto storage = wallet::address_book::create(db::kv_storage("/tmp/test.db", opts));

    book_record rec1{validator, "monster", "Mp46d3d6afe0084fcf530b03d1f4427e516a1cb4ec542640bcbc84c2c4b4f53c13"};
    storage->set(rec1);


    book_record res1 = storage->get("monster");
    ASSERT_EQ(wallet::record_type::validator, res1.type);
    ASSERT_STREQ("monster", res1.name.c_str());
    ASSERT_STREQ("Mp46d3d6afe0084fcf530b03d1f4427e516a1cb4ec542640bcbc84c2c4b4f53c13", res1.value.c_str());


    book_record rec2{record_type::wallet, "main", "Mxa0e1c038dea3dc5970a3dcf30be0b2d85e4d1759"};
    storage->set(rec2);

    book_record res2 = storage->get("main");
    ASSERT_EQ(wallet::record_type::wallet, res2.type);
    ASSERT_STREQ("main", res2.name.c_str());
    ASSERT_STREQ("Mxa0e1c038dea3dc5970a3dcf30be0b2d85e4d1759", res2.value.c_str());

}
 
 
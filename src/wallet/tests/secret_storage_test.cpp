/*!
 * mintex.
 * secret_storage_test.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <cstdlib>
#include <gtest/gtest.h>
#include <leveldb/db.h>
#include <boost/filesystem.hpp>
#include <wallet/kv_storage.h>
#include <wallet/data/secret_storage.h>

TEST(SecretStorage, TestSecretsWriteReadList) {

    namespace fs = boost::filesystem;
    std::system("rm -rf /tmp/test.db");

    using namespace wallet;
    leveldb::Options opts;
    secret_storage storage(db::kv_storage("/tmp/test.db", opts));
    storage.init("qwe123");

    // Load list, check it's empty
    auto items = storage.get_secrets();
    auto addresses = storage.get_addresses();
    ASSERT_EQ(0, items.size());
    ASSERT_EQ(0, addresses.size());


    // Add 1 secret
    auto secret0 = storage.generate();
    storage.add(secret0);

    items.clear();
    items = storage.get_secrets();
    addresses = storage.get_addresses();

    ASSERT_EQ(1, items.size());
    ASSERT_EQ(secret0.priv_key, items[0].priv_key);
    ASSERT_STREQ(secret0.mnemonic.c_str(), items[0].mnemonic.c_str());
    ASSERT_EQ(secret0.derive_index, items[0].derive_index);

    ASSERT_EQ(1, addresses.size());
    ASSERT_EQ(secret0.get_address(), addresses[0]);

    // Add 2 secret
    auto secret1 = storage.generate();
    storage.add(secret1);

    // reload data, to test everything ok
    items.clear();
    items = storage.get_secrets();
    addresses = storage.get_addresses();

    ASSERT_EQ(2, items.size());

    ASSERT_EQ(secret1.priv_key, items[1].priv_key);
    ASSERT_STREQ(secret1.mnemonic.c_str(), items[1].mnemonic.c_str());
    ASSERT_EQ(secret1.derive_index, items[1].derive_index);

    ASSERT_EQ(2, addresses.size());
    ASSERT_EQ(secret1.get_address(), addresses[1]);


    // Remove 0 secret by value
    storage.remove(secret0);
    // reload data, to test everything ok
    items.clear();
    items = storage.get_secrets();
    addresses = storage.get_addresses();

    // test 0 secret is second secret
    ASSERT_EQ(1, items.size());
    ASSERT_EQ(1, addresses.size());

    ASSERT_EQ(secret1.priv_key, items[0].priv_key);
    ASSERT_STREQ(secret1.mnemonic.c_str(), items[0].mnemonic.c_str());
    ASSERT_EQ(secret1.derive_index, items[0].derive_index);

    ASSERT_EQ(secret1.get_address(), addresses[0]);


    // Add again, and test order the same as we were added to vector
    storage.add(secret0);
    // reload data, to test everything ok
    items.clear();
    items = storage.get_secrets();
    addresses = storage.get_addresses();

    ASSERT_EQ(2, items.size());
    ASSERT_TRUE(secret0.priv_key == items[1].priv_key);
    ASSERT_STREQ(secret0.mnemonic.c_str(), items[1].mnemonic.c_str());
    ASSERT_EQ(secret0.derive_index, items[1].derive_index);

    ASSERT_EQ(2, addresses.size());
    ASSERT_EQ(secret0.get_address(), addresses[1]);

    // Remove 0 item by index
    storage.remove(0);
    // reload data, to test everything ok
    items.clear();
    items = storage.get_secrets();
    addresses = storage.get_addresses();

    ASSERT_EQ(1, items.size());
    ASSERT_TRUE(secret0.priv_key == items[0].priv_key);
    ASSERT_STREQ(secret0.mnemonic.c_str(), items[0].mnemonic.c_str());
    ASSERT_EQ(secret0.derive_index, items[0].derive_index);

    ASSERT_EQ(1, addresses.size());
    ASSERT_EQ(secret0.get_address(), addresses[0]);
}
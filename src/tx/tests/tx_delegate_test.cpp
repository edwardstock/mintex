/*!
 * mintex.
 * tx_delegate_test.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <mintex-tx/tx_builder.h>
#include <gtest/gtest.h>

TEST(TxDelegate, TestEncode) {
    const char* expected = "f8900102018a4d4e540000000000000007b6f5a00eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a438a4d4e5400000000000000888ac7230489e80000808001b845f8431ba01c2c8f702d80cf64da1e9bf1f07a52e2fee8721aebe419aa9f62260a98983f89a07ed297d71d9dc37a57ffe9bb16915dccc703d8c09f30da8aadb9d5dbab8c7da9";
    mintex::minter_private_key pk = "07bc17abdcee8b971bb8723e36fe9d2523306d5ab2d683631693238e0f9df142";


    auto tx = mintex::new_tx()
        ->set_gas_coin("MNT")
        .set_nonce("1")
        .set_gas_price("1")
        .set_chain_id(mintex::testnet)
        .tx_delegate()
        ->set_pub_key()
        .build();


    auto signature = tx->sign_single(pk);

    ASSERT_STREQ(expected, signature.toHex().c_str());
}

TEST(TxDelegate, TestDecode) {
    const char* encoded = "f8830102018a4d4e540000000000000002a9e88a4d4e5400000000000000880de0b6b3a76400008a54455354000000000000880de0b6b3a7640000808001b845f8431ba0e34be907a18acb5a1aed263ef419f32f5adc6e772b92f949906b497bba557df3a0291d7704980994f7a6f5950ca84720746b5928f21c3cfc5a5fbca2a9f4d35db0";
    auto decoded = mintex::tx::decode(encoded);
    auto data = decoded->get_data<mintex::tx_sell_coin>();

    ASSERT_STREQ("TEST", data->get_coin_to_buy().c_str());
    ASSERT_STREQ("MNT", data->get_coin_to_sell().c_str());
    ASSERT_EQ(dev::bigdec18("1"), data->get_value_to_sell());
    ASSERT_EQ(dev::bigdec18("1"), data->get_min_value_to_buy());
}

 
 
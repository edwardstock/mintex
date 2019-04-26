//
// Created by edwardstock on 26.04.19.
//
#include <gtest/gtest.h>
#include <mintex-tx/tx_builder.h>

TEST(TxBuy, TestEncode) {

    const char* expected = "f8830102018a4d4e540000000000000004a9e88a54455354000000000000880de0b6b3a76400008a4d4e5400000000000000880de0b6b3a7640000808001b845f8431ca04ee095a20ca58062a5758e2a6d3941857daa8943b5873c57f111190ca88dbc56a01148bf2fcc721ca353105e4f4a3419bec471d7ae08173f443a28c3ae6d27018a";

    mintex::minter_private_key pk = "07bc17abdcee8b971bb8723e36fe9d2523306d5ab2d683631693238e0f9df142";

    auto tx = mintex::new_tx()
            ->set_nonce("1")
            .set_gas_price("1")
            .set_gas_coin("MNT")
            .set_chain_id(mintex::testnet)
            .tx_buy_coin()
            ->set_coin_to_buy("TEST")
            .set_coin_to_sell("MNT")
            .set_value_to_buy("1")
            .set_max_value_to_sell("1")
            .build();

    auto signature = tx->sign_single(pk);

    ASSERT_STREQ(expected, signature.toHex().c_str());

}

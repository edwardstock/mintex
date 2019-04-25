/*!
 * mintex.
 * tx_send_test.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */


#include <gtest/gtest.h>

#include "mintex-tx/tx.h"
#include "mintex-tx/tx_builder.h"
#include "mintex-tx/minter_private_key.h"
#include "mintex-tx/tx_send_coin.h"

TEST(TxSend, TestSendEncode) {
    const char* expectedTx = "f8840102018a4d4e540000000000000001aae98a4d4e5400000000000000940000000000000000000000000000000000000000888ac7230489e80000808001b845f8431ba07a27eed01eae753c028d74e189f23b371fdd936dc0274d36ab27d8f0f4b2e9e1a03cd85dcf65e4204b393de0cd4db03d47479ba4321f074bedff2049b6e048fb6f";

    auto tx_builder = mintex::new_tx();
    tx_builder->set_nonce("1");
    tx_builder->set_gas_price("1");
    tx_builder->set_gas_coin("MNT");
    tx_builder->set_chain_id(mintex::testnet);
    auto data = tx_builder->tx_send_coin();
    data->set_to("Mx0000000000000000000000000000000000000000");
    data->set_value("10");
    data->set_coin("MNT");
    auto tx = data->build();

    mintex::minter_private_key pk("df1f236d0396cc43147e44206c341a65573326e907d033690e31a21323c03a9f");
    auto signedTx = tx->sign_single(pk);

    std::cout << signedTx << std::endl;

    ASSERT_STREQ(expectedTx, signedTx.toHex().c_str());
}

TEST(TxSend, TestSendDecode) {
    const char* encoded = "f8840102018a4d4e540000000000000001aae98a4d4e5400000000000000940000000000000000000000000000000000000000888ac7230489e80000808001b845f8431ba07a27eed01eae753c028d74e189f23b371fdd936dc0274d36ab27d8f0f4b2e9e1a03cd85dcf65e4204b393de0cd4db03d47479ba4321f074bedff2049b6e048fb6f";

    std::shared_ptr<mintex::tx> decoded = mintex::tx::decode(encoded);
    mintex::data::minter_address exAddress = "Mx0000000000000000000000000000000000000000";

    ASSERT_EQ(dev::bigint("1"), decoded->get_nonce());
    ASSERT_STREQ("MNT", decoded->get_gas_coin().c_str());
    ASSERT_EQ(dev::bigint("1"), decoded->get_gas_price());
    ASSERT_NE(dev::bigint("2"), decoded->get_gas_price());
    ASSERT_EQ(mintex::testnet, decoded->get_chain_id());
    std::shared_ptr<mintex::tx_send_coin> data = decoded->get_data<mintex::tx_send_coin>();
    ASSERT_EQ(exAddress, data->get_to());
    ASSERT_EQ(dev::bigdec18("10"), data->get_value());
    ASSERT_STREQ("MNT", data->get_coin().c_str());

}
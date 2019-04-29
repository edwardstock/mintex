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
#include <mintex-tx/minter_public_key.h>
#include <mintex-tx/tx_type.h>
#include <mintex-tx/signature_data.h>
#include <mintex-tx/utils.h>

TEST(TxDelegate, TestEncode) {
    const char* expected = "f8900102018a4d4e540000000000000007b6f5a00eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a438a4d4e5400000000000000888ac7230489e80000808001b845f8431ba01c2c8f702d80cf64da1e9bf1f07a52e2fee8721aebe419aa9f62260a98983f89a07ed297d71d9dc37a57ffe9bb16915dccc703d8c09f30da8aadb9d5dbab8c7da9";
    mintex::privkey_t pk = "6e1df6ec69638d152f563c5eca6c13cdb5db4055861efc11ec1cdd578afd96bf";
    mintex::pubkey_t pub = "Mp0eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a43";

    auto tx = mintex::new_tx()
        ->set_gas_coin("MNT")
        .set_nonce("1")
        .set_gas_price("1")
        .set_chain_id(mintex::testnet)
        .tx_delegate()
        ->set_pub_key(pub)
        .set_coin("MNT")
        .set_stake("10")
        .build();

    auto signature = tx->sign_single(pk);

    ASSERT_STREQ(expected, signature.toHex().c_str());
}

TEST(TxDelegate, TestDecode) {
    const char* encoded = "f8900102018a4d4e540000000000000007b6f5a00eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a438a4d4e5400000000000000888ac7230489e80000808001b845f8431ba01c2c8f702d80cf64da1e9bf1f07a52e2fee8721aebe419aa9f62260a98983f89a07ed297d71d9dc37a57ffe9bb16915dccc703d8c09f30da8aadb9d5dbab8c7da9";
    mintex::pubkey_t pub = "Mp0eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a43";
    auto decoded = mintex::tx::decode(encoded);
    auto data = decoded->get_data<mintex::tx_delegate>();

    ASSERT_STREQ("MNT", data->get_coin().c_str());
    ASSERT_EQ(dev::bigdec18("10"), data->get_stake());
    ASSERT_EQ(pub, data->get_pub_key());
}

TEST(TxUnbond, TestEncode) {
    const char *expected = "f8900102018a4d4e540000000000000008b6f5a00eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a438a4d4e5400000000000000888ac7230489e80000808001b845f8431ca0ff5766c85847b37a276f3f9d027fb7c99745920fa395c7bd399cedd8265c5e1da000791bcdfe4d1bc1e73ada7bf833103c828f22d83189dad2b22ad28a54aacf2a";
    mintex::pubkey_t pub = "Mp0eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a43";
    mintex::privkey_t pk = "6e1df6ec69638d152f563c5eca6c13cdb5db4055861efc11ec1cdd578afd96bf";

    ASSERT_EQ(0x0e, pub[0]);
    ASSERT_EQ(0x43, pub[31]);

    ASSERT_EQ(0x6e, pk[0]);
    ASSERT_EQ(0xbf, pk[31]);

    auto tx = mintex::new_tx()
            ->set_nonce("1")
            .set_gas_price("1")
            .set_gas_coin("MNT")
            .set_chain_id(mintex::testnet)
            .tx_unbound()
            ->set_pub_key(pub)
            .set_coin("MNT")
            .set_value("10")
            .build();

    auto signature = tx->sign_single(pk);

    {
        auto decoded = mintex::tx::decode(signature.cget());
        auto data = decoded->get_data<mintex::tx_unbond>();

        const char* R = "ff5766c85847b37a276f3f9d027fb7c99745920fa395c7bd399cedd8265c5e1d";
        const char* S = "00791bcdfe4d1bc1e73ada7bf833103c828f22d83189dad2b22ad28a54aacf2a";
        const char* V = "1c";

        auto sign_data = decoded->get_signature_data<mintex::signature_single_data>();
        minter::Data r = sign_data->get_r();
        minter::Data s = sign_data->get_s();
        minter::Data v = sign_data->get_v();
        ASSERT_STREQ(R, r.toHex().c_str());
        ASSERT_STREQ(S, s.toHex().c_str());
        ASSERT_STREQ(V, v.toHex().c_str());
    }

    ASSERT_STREQ(expected, signature.toHex().c_str());
}

TEST(TxUnbond, TestDecode) {
    const char* encoded = "f8900102018a4d4e540000000000000008b6f5a00eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a438a4d4e5400000000000000888ac7230489e80000808001b845f8431ca0ff5766c85847b37a276f3f9d027fb7c99745920fa395c7bd399cedd8265c5e1da000791bcdfe4d1bc1e73ada7bf833103c828f22d83189dad2b22ad28a54aacf2a";
    mintex::pubkey_t pub = "Mp0eb98ea04ae466d8d38f490db3c99b3996a90e24243952ce9822c6dc1e2c1a43";
    auto decoded = mintex::tx::decode(encoded);
    auto data = decoded->get_data<mintex::tx_unbond>();

    ASSERT_STREQ("MNT", decoded->get_gas_coin().c_str());
    ASSERT_EQ(dev::bigint("1"), decoded->get_gas_price());
    ASSERT_EQ(mintex::testnet, decoded->get_chain_id());
    ASSERT_EQ(dev::bigint("1"), decoded->get_nonce());
    ASSERT_EQ((uint8_t)0x08, decoded->get_type());
    ASSERT_EQ(dev::bytes(0), decoded->get_service_data());
    ASSERT_EQ(dev::bytes(0), decoded->get_payload());
    ASSERT_EQ(mintex::signature_type::single, decoded->get_signature_type());

    ASSERT_STREQ("MNT", data->get_coin().c_str());
    ASSERT_EQ(dev::bigdec18("10"), data->get_value());
    ASSERT_EQ(pub, data->get_pub_key());

    const char* R = "ff5766c85847b37a276f3f9d027fb7c99745920fa395c7bd399cedd8265c5e1d";
    const char* S = "00791bcdfe4d1bc1e73ada7bf833103c828f22d83189dad2b22ad28a54aacf2a";
    const char* V = "1c";

    auto sign_data = decoded->get_signature_data<mintex::signature_single_data>();
    minter::Data r = sign_data->get_r();
    minter::Data s = sign_data->get_s();
    minter::Data v = sign_data->get_v();
    ASSERT_STREQ(R, r.toHex().c_str());
    ASSERT_STREQ(S, s.toHex().c_str());
    ASSERT_STREQ(V, v.toHex().c_str());
}
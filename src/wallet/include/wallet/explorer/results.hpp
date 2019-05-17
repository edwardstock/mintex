#ifndef GEN_EXPLORER_RESULTS
#define GEN_EXPLORER_RESULTS

// AUTOGENERATED header. DO NOT modify manually.
//
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <eth/Common.h>
#include "wallet/explorer/base.h"
#include "mintex-tx/minter_hash.h"
#include "mintex-tx/minter_address.h"
#include "mintex-tx/minter_public_key.h"
#include "mintex-tx/tx_type.h"



namespace wallet {
namespace explorer {

struct transaction_item {
	dev::bigint txn;
	mintex::hash_t hash;
	dev::bigint nonce;
	dev::bigint block;
	std::string timestamp;
	dev::bigdec18 fee;
	mintex::tx_type_val type;
	std::string payload;
	mintex::address_t from;
	std::shared_ptr<wallet::explorer::tx_result> data;
	static ::nlohmann::json _data_encode(nlohmann::json &j, const ::wallet::explorer::transaction_item& resp);
	static void _data_decode(const nlohmann::json &j, ::wallet::explorer::transaction_item& resp);
};
struct tx_send_result : public wallet::explorer::tx_result  {
	mintex::address_t to;
	std::string coin;
	dev::bigdec18 value;
};
struct tx_create_coin_result : public wallet::explorer::tx_result  {
	std::string name;
	std::string symbol;
	dev::bigdec18 initial_amount;
	dev::bigdec18 initial_reserve;
	uint32_t crr;
};
struct tx_convert_result : public wallet::explorer::tx_result  {
	std::string coin_to_sell;
	std::string coin_to_buy;
	dev::bigdec18 value_to_buy;
	dev::bigdec18 value_to_sell;
	dev::bigdec18 maximum_value_to_sell;
	dev::bigdec18 minimum_value_to_buy;
};
struct tx_declare_candidacy_result : public wallet::explorer::tx_result  {
	mintex::address_t address;
	mintex::pubkey_t pub_key;
	uint32_t commission;
	std::string coin;
	dev::bigdec18 stake;
};
struct tx_set_candidate_on_off_result : public wallet::explorer::tx_result  {
	mintex::pubkey_t pub_key;
};
struct tx_delegate_unbond_result : public wallet::explorer::tx_result  {
	mintex::pubkey_t pub_key;
	std::string coin;
	dev::bigdec18 stake;
	dev::bigdec18 value;
};
struct tx_check_data_result {
	std::string coin;
	dev::bigint nonce;
	dev::bigdec18 value;
	mintex::address_t sender;
	dev::bigint due_block;
};
struct tx_redeem_check_result : public wallet::explorer::tx_result  {
	std::string raw_check;
	std::string proof;
	dev::bigdec18 stake;
	dev::bigdec18 value;
	tx_check_data_result check;
};
struct tx_create_multisig_result : public wallet::explorer::tx_result  {
	dev::bigint threshold;
	std::vector<dev::bigint> weights;
	std::vector<mintex::address_t> addresses;
};
struct tx_multisend_result : public wallet::explorer::tx_result  {
	std::vector<tx_send_result> items;
};
struct tx_edit_candidate_result : public wallet::explorer::tx_result  {
	mintex::address_t reward_address;
	mintex::address_t owner_address;
	mintex::pubkey_t pub_key;
};
struct balance_item {
	std::string coin;
	dev::bigdec18 amount;
};
struct balance_items {
	mintex::address_t address;
	std::vector<balance_item> balances;
};
struct delegated_item : public wallet::explorer::tx_result  {
	mintex::pubkey_t pub_key;
	std::string coin;
	dev::bigdec18 value;
};
struct reward_item {
	uint64_t block;
	std::string role;
	dev::bigdec18 amount;
	mintex::address_t address;
	mintex::pubkey_t validator;
	std::string timestamp;
};
inline void from_json(const nlohmann::json& j, ::wallet::explorer::transaction_item& resp) {
	if(j.find("txn") != j.end() && !j.at("txn").is_null()) j.at("txn").get_to<dev::bigint>(resp.txn);
	if(j.find("hash") != j.end() && !j.at("hash").is_null()) j.at("hash").get_to<mintex::hash_t>(resp.hash);
	if(j.find("nonce") != j.end() && !j.at("nonce").is_null()) j.at("nonce").get_to<dev::bigint>(resp.nonce);
	if(j.find("block") != j.end() && !j.at("block").is_null()) j.at("block").get_to<dev::bigint>(resp.block);
	if(j.find("timestamp") != j.end() && !j.at("timestamp").is_null()) j.at("timestamp").get_to<std::string>(resp.timestamp);
	if(j.find("fee") != j.end() && !j.at("fee").is_null()) j.at("fee").get_to<dev::bigdec18>(resp.fee);
	if(j.find("type") != j.end() && !j.at("type").is_null()) j.at("type").get_to<mintex::tx_type_val>(resp.type);
	if(j.find("payload") != j.end() && !j.at("payload").is_null()) j.at("payload").get_to<std::string>(resp.payload);
	if(j.find("from") != j.end() && !j.at("from").is_null()) j.at("from").get_to<mintex::address_t>(resp.from);
	if(j.find("data") != j.end() && !j.at("data").is_null()) ::wallet::explorer::transaction_item::_data_decode(j, resp);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::transaction_item& resp) {
	j = nlohmann::json{
		{"txn", resp.txn},
		{"hash", resp.hash},
		{"nonce", resp.nonce},
		{"block", resp.block},
		{"timestamp", resp.timestamp},
		{"fee", resp.fee},
		{"type", resp.type},
		{"payload", resp.payload},
		{"from", resp.from},
		{"data", ::wallet::explorer::transaction_item::_data_encode(j, resp)},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::tx_send_result& resp) {
	if(j.find("to") != j.end() && !j.at("to").is_null()) j.at("to").get_to<mintex::address_t>(resp.to);
	if(j.find("coin") != j.end() && !j.at("coin").is_null()) j.at("coin").get_to<std::string>(resp.coin);
	if(j.find("value") != j.end() && !j.at("value").is_null()) j.at("value").get_to<dev::bigdec18>(resp.value);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::tx_send_result& resp) {
	j = nlohmann::json{
		{"to", resp.to},
		{"coin", resp.coin},
		{"value", resp.value},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::tx_create_coin_result& resp) {
	if(j.find("name") != j.end() && !j.at("name").is_null()) j.at("name").get_to<std::string>(resp.name);
	if(j.find("symbol") != j.end() && !j.at("symbol").is_null()) j.at("symbol").get_to<std::string>(resp.symbol);
	if(j.find("initial_amount") != j.end() && !j.at("initial_amount").is_null()) j.at("initial_amount").get_to<dev::bigdec18>(resp.initial_amount);
	if(j.find("initial_reserve") != j.end() && !j.at("initial_reserve").is_null()) j.at("initial_reserve").get_to<dev::bigdec18>(resp.initial_reserve);
	if(j.find("constant_reserve_ratio") != j.end() && !j.at("constant_reserve_ratio").is_null()) j.at("constant_reserve_ratio").get_to<uint32_t>(resp.crr);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::tx_create_coin_result& resp) {
	j = nlohmann::json{
		{"name", resp.name},
		{"symbol", resp.symbol},
		{"initial_amount", resp.initial_amount},
		{"initial_reserve", resp.initial_reserve},
		{"constant_reserve_ratio", resp.crr},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::tx_convert_result& resp) {
	if(j.find("coin_to_sell") != j.end() && !j.at("coin_to_sell").is_null()) j.at("coin_to_sell").get_to<std::string>(resp.coin_to_sell);
	if(j.find("coin_to_buy") != j.end() && !j.at("coin_to_buy").is_null()) j.at("coin_to_buy").get_to<std::string>(resp.coin_to_buy);
	if(j.find("value_to_buy") != j.end() && !j.at("value_to_buy").is_null()) j.at("value_to_buy").get_to<dev::bigdec18>(resp.value_to_buy);
	if(j.find("value_to_sell") != j.end() && !j.at("value_to_sell").is_null()) j.at("value_to_sell").get_to<dev::bigdec18>(resp.value_to_sell);
	if(j.find("maximum_value_to_sell") != j.end() && !j.at("maximum_value_to_sell").is_null()) j.at("maximum_value_to_sell").get_to<dev::bigdec18>(resp.maximum_value_to_sell);
	if(j.find("minimum_value_to_buy") != j.end() && !j.at("minimum_value_to_buy").is_null()) j.at("minimum_value_to_buy").get_to<dev::bigdec18>(resp.minimum_value_to_buy);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::tx_convert_result& resp) {
	j = nlohmann::json{
		{"coin_to_sell", resp.coin_to_sell},
		{"coin_to_buy", resp.coin_to_buy},
		{"value_to_buy", resp.value_to_buy},
		{"value_to_sell", resp.value_to_sell},
		{"maximum_value_to_sell", resp.maximum_value_to_sell},
		{"minimum_value_to_buy", resp.minimum_value_to_buy},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::tx_declare_candidacy_result& resp) {
	if(j.find("address") != j.end() && !j.at("address").is_null()) j.at("address").get_to<mintex::address_t>(resp.address);
	if(j.find("pub_key") != j.end() && !j.at("pub_key").is_null()) j.at("pub_key").get_to<mintex::pubkey_t>(resp.pub_key);
	if(j.find("commission") != j.end() && !j.at("commission").is_null()) j.at("commission").get_to<uint32_t>(resp.commission);
	if(j.find("coin") != j.end() && !j.at("coin").is_null()) j.at("coin").get_to<std::string>(resp.coin);
	if(j.find("stake") != j.end() && !j.at("stake").is_null()) j.at("stake").get_to<dev::bigdec18>(resp.stake);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::tx_declare_candidacy_result& resp) {
	j = nlohmann::json{
		{"address", resp.address},
		{"pub_key", resp.pub_key},
		{"commission", resp.commission},
		{"coin", resp.coin},
		{"stake", resp.stake},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::tx_set_candidate_on_off_result& resp) {
	if(j.find("pub_key") != j.end() && !j.at("pub_key").is_null()) j.at("pub_key").get_to<mintex::pubkey_t>(resp.pub_key);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::tx_set_candidate_on_off_result& resp) {
	j = nlohmann::json{
		{"pub_key", resp.pub_key},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::tx_delegate_unbond_result& resp) {
	if(j.find("pub_key") != j.end() && !j.at("pub_key").is_null()) j.at("pub_key").get_to<mintex::pubkey_t>(resp.pub_key);
	if(j.find("coin") != j.end() && !j.at("coin").is_null()) j.at("coin").get_to<std::string>(resp.coin);
	if(j.find("stake") != j.end() && !j.at("stake").is_null()) j.at("stake").get_to<dev::bigdec18>(resp.stake);
	if(j.find("value") != j.end() && !j.at("value").is_null()) j.at("value").get_to<dev::bigdec18>(resp.value);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::tx_delegate_unbond_result& resp) {
	j = nlohmann::json{
		{"pub_key", resp.pub_key},
		{"coin", resp.coin},
		{"stake", resp.stake},
		{"value", resp.value},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::tx_check_data_result& resp) {
	if(j.find("coin") != j.end() && !j.at("coin").is_null()) j.at("coin").get_to<std::string>(resp.coin);
	if(j.find("nonce") != j.end() && !j.at("nonce").is_null()) j.at("nonce").get_to<dev::bigint>(resp.nonce);
	if(j.find("value") != j.end() && !j.at("value").is_null()) j.at("value").get_to<dev::bigdec18>(resp.value);
	if(j.find("sender") != j.end() && !j.at("sender").is_null()) j.at("sender").get_to<mintex::address_t>(resp.sender);
	if(j.find("due_block") != j.end() && !j.at("due_block").is_null()) j.at("due_block").get_to<dev::bigint>(resp.due_block);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::tx_check_data_result& resp) {
	j = nlohmann::json{
		{"coin", resp.coin},
		{"nonce", resp.nonce},
		{"value", resp.value},
		{"sender", resp.sender},
		{"due_block", resp.due_block},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::tx_redeem_check_result& resp) {
	if(j.find("raw_check") != j.end() && !j.at("raw_check").is_null()) j.at("raw_check").get_to<std::string>(resp.raw_check);
	if(j.find("proof") != j.end() && !j.at("proof").is_null()) j.at("proof").get_to<std::string>(resp.proof);
	if(j.find("stake") != j.end() && !j.at("stake").is_null()) j.at("stake").get_to<dev::bigdec18>(resp.stake);
	if(j.find("value") != j.end() && !j.at("value").is_null()) j.at("value").get_to<dev::bigdec18>(resp.value);
	if(j.find("check") != j.end() && !j.at("check").is_null()) j.at("check").get_to<tx_check_data_result>(resp.check);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::tx_redeem_check_result& resp) {
	j = nlohmann::json{
		{"raw_check", resp.raw_check},
		{"proof", resp.proof},
		{"stake", resp.stake},
		{"value", resp.value},
		{"check", resp.check},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::tx_create_multisig_result& resp) {
	if(j.find("threshold") != j.end() && !j.at("threshold").is_null()) j.at("threshold").get_to<dev::bigint>(resp.threshold);
	if(j.find("weights") != j.end() && !j.at("weights").is_null()) j.at("weights").get_to<std::vector<dev::bigint>>(resp.weights);
	if(j.find("addresses") != j.end() && !j.at("addresses").is_null()) j.at("addresses").get_to<std::vector<mintex::address_t>>(resp.addresses);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::tx_create_multisig_result& resp) {
	j = nlohmann::json{
		{"threshold", resp.threshold},
		{"weights", resp.weights},
		{"addresses", resp.addresses},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::tx_multisend_result& resp) {
	if(j.find("list") != j.end() && !j.at("list").is_null()) j.at("list").get_to<std::vector<tx_send_result>>(resp.items);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::tx_multisend_result& resp) {
	j = nlohmann::json{
		{"list", resp.items},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::tx_edit_candidate_result& resp) {
	if(j.find("reward_address") != j.end() && !j.at("reward_address").is_null()) j.at("reward_address").get_to<mintex::address_t>(resp.reward_address);
	if(j.find("owner_address") != j.end() && !j.at("owner_address").is_null()) j.at("owner_address").get_to<mintex::address_t>(resp.owner_address);
	if(j.find("pub_key") != j.end() && !j.at("pub_key").is_null()) j.at("pub_key").get_to<mintex::pubkey_t>(resp.pub_key);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::tx_edit_candidate_result& resp) {
	j = nlohmann::json{
		{"reward_address", resp.reward_address},
		{"owner_address", resp.owner_address},
		{"pub_key", resp.pub_key},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::balance_item& resp) {
	if(j.find("coin") != j.end() && !j.at("coin").is_null()) j.at("coin").get_to<std::string>(resp.coin);
	if(j.find("amount") != j.end() && !j.at("amount").is_null()) j.at("amount").get_to<dev::bigdec18>(resp.amount);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::balance_item& resp) {
	j = nlohmann::json{
		{"coin", resp.coin},
		{"amount", resp.amount},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::balance_items& resp) {
	if(j.find("address") != j.end() && !j.at("address").is_null()) j.at("address").get_to<mintex::address_t>(resp.address);
	if(j.find("balances") != j.end() && !j.at("balances").is_null()) j.at("balances").get_to<std::vector<balance_item>>(resp.balances);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::balance_items& resp) {
	j = nlohmann::json{
		{"address", resp.address},
		{"balances", resp.balances},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::delegated_item& resp) {
	if(j.find("pub_key") != j.end() && !j.at("pub_key").is_null()) j.at("pub_key").get_to<mintex::pubkey_t>(resp.pub_key);
	if(j.find("coin") != j.end() && !j.at("coin").is_null()) j.at("coin").get_to<std::string>(resp.coin);
	if(j.find("value") != j.end() && !j.at("value").is_null()) j.at("value").get_to<dev::bigdec18>(resp.value);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::delegated_item& resp) {
	j = nlohmann::json{
		{"pub_key", resp.pub_key},
		{"coin", resp.coin},
		{"value", resp.value},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::explorer::reward_item& resp) {
	if(j.find("block") != j.end() && !j.at("block").is_null()) j.at("block").get_to<uint64_t>(resp.block);
	if(j.find("role") != j.end() && !j.at("role").is_null()) j.at("role").get_to<std::string>(resp.role);
	if(j.find("amount") != j.end() && !j.at("amount").is_null()) j.at("amount").get_to<dev::bigdec18>(resp.amount);
	if(j.find("address") != j.end() && !j.at("address").is_null()) j.at("address").get_to<mintex::address_t>(resp.address);
	if(j.find("validator") != j.end() && !j.at("validator").is_null()) j.at("validator").get_to<mintex::pubkey_t>(resp.validator);
	if(j.find("timestamp") != j.end() && !j.at("timestamp").is_null()) j.at("timestamp").get_to<std::string>(resp.timestamp);
}
inline void to_json(nlohmann::json& j, const ::wallet::explorer::reward_item& resp) {
	j = nlohmann::json{
		{"block", resp.block},
		{"role", resp.role},
		{"amount", resp.amount},
		{"address", resp.address},
		{"validator", resp.validator},
		{"timestamp", resp.timestamp},
	};
}


}
}

#endif

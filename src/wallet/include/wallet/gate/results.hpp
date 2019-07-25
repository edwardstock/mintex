#ifndef GEN_GATE_RESULTS
#define GEN_GATE_RESULTS

// AUTOGENERATED header. DO NOT modify manually.
//
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <minter/eth/Common.h>
#include <minter/hash.h>
#include <minter/address.h>
#include <minter/public_key.h>
#include <minter/tx/tx_type.h>
#include "wallet/gate/base.h"



namespace wallet {
namespace gate {

struct gas_value {
	dev::bigint gas;
};
struct commission_value {
	dev::bigint value;
};
struct exchange_sell_value {
	dev::bigint will_get;
	dev::bigint commission;
};
struct exchange_buy_value {
	dev::bigint will_pay;
	dev::bigint commission;
};
struct tx_count_value {
	dev::bigint count;
};
struct tx_send_result {
	minter::hash_t hash;
};
inline void from_json(const nlohmann::json& j, ::wallet::gate::gas_value& resp) {
	if(j.find("gas") != j.end() && !j.at("gas").is_null()) j.at("gas").get_to<dev::bigint>(resp.gas);
}
inline void to_json(nlohmann::json& j, const ::wallet::gate::gas_value& resp) {
	j = nlohmann::json{
		{"gas", resp.gas},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::gate::commission_value& resp) {
	if(j.find("commission") != j.end() && !j.at("commission").is_null()) j.at("commission").get_to<dev::bigint>(resp.value);
}
inline void to_json(nlohmann::json& j, const ::wallet::gate::commission_value& resp) {
	j = nlohmann::json{
		{"commission", resp.value},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::gate::exchange_sell_value& resp) {
	if(j.find("will_get") != j.end() && !j.at("will_get").is_null()) j.at("will_get").get_to<dev::bigint>(resp.will_get);
	if(j.find("commission") != j.end() && !j.at("commission").is_null()) j.at("commission").get_to<dev::bigint>(resp.commission);
}
inline void to_json(nlohmann::json& j, const ::wallet::gate::exchange_sell_value& resp) {
	j = nlohmann::json{
		{"will_get", resp.will_get},
		{"commission", resp.commission},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::gate::exchange_buy_value& resp) {
	if(j.find("will_pay") != j.end() && !j.at("will_pay").is_null()) j.at("will_pay").get_to<dev::bigint>(resp.will_pay);
	if(j.find("commission") != j.end() && !j.at("commission").is_null()) j.at("commission").get_to<dev::bigint>(resp.commission);
}
inline void to_json(nlohmann::json& j, const ::wallet::gate::exchange_buy_value& resp) {
	j = nlohmann::json{
		{"will_pay", resp.will_pay},
		{"commission", resp.commission},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::gate::tx_count_value& resp) {
	if(j.find("nonce") != j.end() && !j.at("nonce").is_null()) j.at("nonce").get_to<dev::bigint>(resp.count);
}
inline void to_json(nlohmann::json& j, const ::wallet::gate::tx_count_value& resp) {
	j = nlohmann::json{
		{"nonce", resp.count},
	};
}
inline void from_json(const nlohmann::json& j, ::wallet::gate::tx_send_result& resp) {
	if(j.find("hash") != j.end() && !j.at("hash").is_null()) j.at("hash").get_to<minter::hash_t>(resp.hash);
}
inline void to_json(nlohmann::json& j, const ::wallet::gate::tx_send_result& resp) {
	j = nlohmann::json{
		{"hash", resp.hash},
	};
}


}
}

#endif

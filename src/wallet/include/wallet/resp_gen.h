/*!
 * mintex.
 * resp_gen.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <minter/eth/Common.h>
#include <nlohmann/json.hpp>
#include <minter/tx/utils.h>
#include <minter/address.h>
#include <minter/public_key.h>
#include <minter/hash.h>

#ifndef MINTEX_RESP_GEN_H
#define MINTEX_RESP_GEN_H

namespace nlohmann {

template <>
struct adl_serializer<dev::bigdec18> {
  static inline void to_json(json& j, const dev::bigdec18& p) {
      j = minter::utils::to_string(p);
  }

  static inline void from_json(const json& j, dev::bigdec18& p) {
      if(j.is_null()) {
          p = dev::bigdec18("0");
      } else if(j.is_number()) {
          p = dev::bigdec18(j.get<double>());
      } else {
          p = dev::bigdec18(j.get<std::string>());
      }
  }
};

template <>
struct adl_serializer<dev::bigint> {
  static inline void to_json(json& j, const dev::bigint& p) {
      j = minter::utils::to_string(p);
  }

  static inline void from_json(const json& j, dev::bigint& p) {
      if(j.is_null()) {
          p = dev::bigint("0");
      } else if(j.is_number()) {
          double d = j.get<double>();
          p = dev::bigint(dev::bigdec18(d));
      } else {
          p = dev::bigint(j.get<std::string>());
      }
  }
};

template <>
struct adl_serializer<minter::address_t> {
  static inline void to_json(json& j, const minter::address_t& val) {
      j = val.to_string();
  }

  static inline void from_json(const json& j, minter::address_t& val) {
      val = minter::address_t(j.get<std::string>());
  }
};

template <>
struct adl_serializer<minter::pubkey_t > {
  static inline void to_json(json& j, const minter::pubkey_t& val) {
      j = val.to_string();
  }

  static inline void from_json(const json& j, minter::pubkey_t& val) {
      val = minter::pubkey_t(j.get<std::string>());
  }
};

template <>
struct adl_serializer<minter::privkey_t > {
  static inline void to_json(json& j, const minter::privkey_t& val) {
      j = val.toHex();
  }

  static inline void from_json(const json& j, minter::privkey_t& val) {
      val = minter::privkey_t(j.get<std::string>().c_str());
  }
};

template <>
struct adl_serializer<minter::hash_t > {
  static inline void to_json(json& j, const minter::hash_t& val) {
      j = val.to_string();
  }

  static inline void from_json(const json& j, minter::hash_t& val) {
      val = minter::hash_t(j.get<std::string>());
  }
};

} //nlohmann



#define CREATE_RESP1(name, t1, f1) struct name { \
t1 f1; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{{ \
        #f1, r.f1 \
    }}; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    if(j.find(#f1) != j.end() && !j.at(#f1).is_null()) j.at(#f1).get_to<t1>(r.f1); \
} \

#define CREATE_RESP_PARENT1(name, parent, t1, f1) struct name: parent { \
t1 f1; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{{ \
        #f1, r.f1 \
    }}; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    j.at(#f1).get_to(r.f1); \
} \

#define CREATE_RESP2(name, t1, f1, t2, f2) struct name { \
t1 f1; \
t2 f2; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{ \
        {#f1, r.f1}, \
        {#f2, r.f2} \
    }; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    if(j.find(#f1) != j.end() && !j.at(#f1).is_null()) j.at(#f1).get_to<t1>(r.f1); \
    if(j.find(#f2) != j.end() && !j.at(#f2).is_null()) j.at(#f2).get_to<t2>(r.f2); \
} \

#define CREATE_RESP_PARENT2(name, parent, t1, f1, t2, f2) struct name: parent { \
t1 f1; \
t2 f2; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{ \
        {#f1, r.f1}, \
        {#f2, r.f2} \
    }; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    if(j.find(#f1) != j.end() && !j.at(#f1).is_null()) j.at(#f1).get_to<t1>(r.f1); \
    if(j.find(#f2) != j.end() && !j.at(#f2).is_null()) j.at(#f2).get_to<t2>(r.f2); \
} \

#define CREATE_RESP3(name, t1, f1, t2, f2, t3, f3) struct name { \
t1 f1; \
t2 f2; \
t3 f3; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{{ \
    #f1, r.f1, \
    #f2, r.f2, \
    #f3, r.f3 \
    }}; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    if(j.find(#f1) != j.end() && !j.at(#f1).is_null()) j.at(#f1).get_to<t1>(r.f1); \
    if(j.find(#f2) != j.end() && !j.at(#f2).is_null()) j.at(#f2).get_to<t2>(r.f2); \
    if(j.find(#f3) != j.end() && !j.at(#f3).is_null()) j.at(#f3).get_to<t3>(r.f3); \
} \

#define CREATE_RESP_PARENT3(name, parent, t1, f1, t2, f2, t3, f3) struct name: parent { \
t1 f1; \
t2 f2; \
t3 f3; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{{ \
    #f1, r.f1, \
    #f2, r.f2, \
    #f3, r.f3 \
    }}; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    if(j.find(#f1) != j.end() && !j.at(#f1).is_null()) j.at(#f1).get_to<t1>(r.f1); \
    if(j.find(#f2) != j.end() && !j.at(#f2).is_null()) j.at(#f2).get_to<t2>(r.f2); \
    if(j.find(#f3) != j.end() && !j.at(#f3).is_null()) j.at(#f3).get_to<t3>(r.f3); \
} \

#define CREATE_RESP4(name, t1, f1, t2, f2, t3, f3, t4, f4) struct name { \
t1 f1; \
t2 f2; \
t3 f3; \
t4 f4; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{{ \
    #f1, r.f1, \
    #f2, r.f2, \
    #f3, r.f3, \
    #f4, r.f4 \
    }}; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    if(j.find(#f1) != j.end() && !j.at(#f1).is_null()) j.at(#f1).get_to<t1>(r.f1); \
    if(j.find(#f2) != j.end() && !j.at(#f2).is_null()) j.at(#f2).get_to<t2>(r.f2); \
    if(j.find(#f3) != j.end() && !j.at(#f3).is_null()) j.at(#f3).get_to<t3>(r.f3); \
    if(j.find(#f4) != j.end() && !j.at(#f4).is_null()) j.at(#f4).get_to<t4>(r.f4); \
} \

#define CREATE_RESP_PARENT4(name, parent, t1, f1, t2, f2, t3, f3, t4, f4) struct name: parent { \
t1 f1; \
t2 f2; \
t3 f3; \
t4 f4; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{{ \
    #f1, r.f1, \
    #f2, r.f2, \
    #f3, r.f3, \
    #f4, r.f4 \
    }}; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    if(j.find(#f1) != j.end() && !j.at(#f1).is_null()) j.at(#f1).get_to<t1>(r.f1); \
    if(j.find(#f2) != j.end() && !j.at(#f2).is_null()) j.at(#f2).get_to<t2>(r.f2); \
    if(j.find(#f3) != j.end() && !j.at(#f3).is_null()) j.at(#f3).get_to<t3>(r.f3); \
    if(j.find(#f4) != j.end() && !j.at(#f4).is_null()) j.at(#f4).get_to<t4>(r.f4); \
} \

#define CREATE_RESP5(name, t1, f1, t2, f2, t3, f3, t4, f4, t5, f5) struct name { \
t1 f1; \
t2 f2; \
t3 f3; \
t4 f4; \
t5 f5; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{{ \
    #f1, r.f1, \
    #f2, r.f2, \
    #f3, r.f3, \
    #f4, r.f4, \
    #f5, r.f5 \
    }}; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    if(j.find(#f1) != j.end() && !j.at(#f1).is_null()) j.at(#f1).get_to<t1>(r.f1); \
    if(j.find(#f2) != j.end() && !j.at(#f2).is_null()) j.at(#f2).get_to<t2>(r.f2); \
    if(j.find(#f3) != j.end() && !j.at(#f3).is_null()) j.at(#f3).get_to<t3>(r.f3); \
    if(j.find(#f4) != j.end() && !j.at(#f4).is_null()) j.at(#f4).get_to<t4>(r.f4); \
    if(j.find(#f5) != j.end() && !j.at(#f5).is_null()) j.at(#f5).get_to<t5>(r.f5); \
} \

#define CREATE_RESP6(name, t1, f1, t2, f2, t3, f3, t4, f4, t5, f5, t6, f6) struct name { \
t1 f1; \
t2 f2; \
t3 f3; \
t4 f4; \
t5 f5; \
t6 f6; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{{ \
    #f1, r.f1, \
    #f2, r.f2, \
    #f3, r.f3, \
    #f4, r.f4, \
    #f5, r.f5, \
    #f6, r.f6 \
    }}; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    if(j.find(#f1) != j.end() && !j.at(#f1).is_null()) j.at(#f1).get_to<t1>(r.f1); \
    if(j.find(#f2) != j.end() && !j.at(#f2).is_null()) j.at(#f2).get_to<t2>(r.f2); \
    if(j.find(#f3) != j.end() && !j.at(#f3).is_null()) j.at(#f3).get_to<t3>(r.f3); \
    if(j.find(#f4) != j.end() && !j.at(#f4).is_null()) j.at(#f4).get_to<t4>(r.f4); \
    if(j.find(#f5) != j.end() && !j.at(#f5).is_null()) j.at(#f5).get_to<t5>(r.f5); \
    if(j.find(#f6) != j.end() && !j.at(#f6).is_null()) j.at(#f6).get_to<t6>(r.f6); \
} \



#define CREATE_RESP7(name, t1, f1, t2, f2, t3, f3, t4, f4, t5, f5, t6, f6, t7, f7) struct name { \
t1 f1; \
t2 f2; \
t3 f3; \
t4 f4; \
t5 f5; \
t6 f6; \
t7 f7; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{{ \
    #f1, r.f1, \
    #f2, r.f2, \
    #f3, r.f3, \
    #f4, r.f4, \
    #f5, r.f5, \
    #f6, r.f6, \
    #f7, r.f7 \
    }}; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    if(j.find(#f1) != j.end() && !j.at(#f1).is_null()) j.at(#f1).get_to<t1>(r.f1); \
    if(j.find(#f2) != j.end() && !j.at(#f2).is_null()) j.at(#f2).get_to<t2>(r.f2); \
    if(j.find(#f3) != j.end() && !j.at(#f3).is_null()) j.at(#f3).get_to<t3>(r.f3); \
    if(j.find(#f4) != j.end() && !j.at(#f4).is_null()) j.at(#f4).get_to<t4>(r.f4); \
    if(j.find(#f5) != j.end() && !j.at(#f5).is_null()) j.at(#f5).get_to<t5>(r.f5); \
    if(j.find(#f6) != j.end() && !j.at(#f6).is_null()) j.at(#f6).get_to<t6>(r.f6); \
    if(j.find(#f7) != j.end() && !j.at(#f7).is_null()) j.at(#f7).get_to<t7>(r.f7); \
} \


#define CREATE_RESP8(name, t1, f1, t2, f2, t3, f3, t4, f4, t5, f5, t6, f6, t7, f7, t8, f8) struct name { \
t1 f1; \
t2 f2; \
t3 f3; \
t4 f4; \
t5 f5; \
t6 f6; \
t7 f7; \
t8 f8; \
}; \
inline void to_json(nlohmann::json& j, const name &r) { \
    j = nlohmann::json{{ \
    #f1, r.f1, \
    #f2, r.f2, \
    #f3, r.f3, \
    #f4, r.f4, \
    #f5, r.f5, \
    #f6, r.f6, \
    #f7, r.f7, \
    #f8, r.f8 \
    }}; \
} \
inline void from_json(const nlohmann::json &j, name &r) { \
    if(j.find(#f1) != j.end() && !j.at(#f1).is_null()) j.at(#f1).get_to<t1>(r.f1); \
    if(j.find(#f2) != j.end() && !j.at(#f2).is_null()) j.at(#f2).get_to<t2>(r.f2); \
    if(j.find(#f3) != j.end() && !j.at(#f3).is_null()) j.at(#f3).get_to<t3>(r.f3); \
    if(j.find(#f4) != j.end() && !j.at(#f4).is_null()) j.at(#f4).get_to<t4>(r.f4); \
    if(j.find(#f5) != j.end() && !j.at(#f5).is_null()) j.at(#f5).get_to<t5>(r.f5); \
    if(j.find(#f6) != j.end() && !j.at(#f6).is_null()) j.at(#f6).get_to<t6>(r.f6); \
    if(j.find(#f7) != j.end() && !j.at(#f7).is_null()) j.at(#f7).get_to<t7>(r.f7); \
    if(j.find(#f8) != j.end() && !j.at(#f8).is_null()) j.at(#f8).get_to<t8>(r.f8); \
} \

#endif //MINTEX_RESP_GEN_H

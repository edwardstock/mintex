/*!
 * mintex.
 * signature_data.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_SIGNATURE_DATA_H
#define MINTEX_SIGNATURE_DATA_H

#include <vector>
#include <eth/RLP.h>
#include "mintex-tx/minter_address.h"
#include "mintex-tx/signature.h"

namespace mintex {

class signature_data {
public:
    virtual dev::bytes encode() = 0;
    virtual void decode(const dev::RLP &data) = 0;
    virtual ~signature_data() = default;
};

class signature_single_data: public virtual mintex::signature_data {
public:
    void set_signature(const mintex::signature &sig);
    void set_signature(mintex::signature &&sig);
    void set_signature(const uint8_t *data);
    void set_signature(dev::bytes &&v, dev::bytes &&r, dev::bytes &&s);
    void set_signature(dev::bytes &&data);
    void set_signature(const dev::bytes &data);
    const dev::bytes & get_v() const;
    const dev::bytes & get_r() const;
    const dev::bytes & get_s() const;

    dev::bytes encode() override;
    void decode(const dev::RLP &data) override;
private:
    dev::bytes m_v, m_r, m_s;
};

class signature_multi_data: public virtual mintex::signature_data {
public:
    signature_multi_data &set_signatures(const mintex::data::minter_address &address, std::vector<mintex::signature_single_data> &&signs);
    dev::bytes encode() override;
    void decode(const dev::RLP &data) override;

private:
    mintex::data::minter_address m_address;
    std::vector<mintex::signature_single_data> m_signs;
};

}

#endif //MINTEX_SIGNATURE_DATA_H

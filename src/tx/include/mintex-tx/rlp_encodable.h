/*!
 * mintex.
 * rlp_encodable.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_RLP_ENCODABLE_H
#define MINTEX_RLP_ENCODABLE_H

#include <eth/RLP.h>

namespace mintex {

class rlp_encodable {
public:
    virtual dev::bytes encode() = 0;
};

}

#endif //MINTEX_RLP_ENCODABLE_H

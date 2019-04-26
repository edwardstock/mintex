/*!
 * mintex.
 * signature.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_SIGNATURE_H
#define MINTEX_SIGNATURE_H

#include <eth/Common.h>

namespace mintex {

typedef enum {
  single = (uint8_t) 0x01,
  multi = (uint8_t) 0x02
} signature_type;

typedef struct {
  dev::bytes r, s, v;
  bool success = false;
} signature;

}

#endif //MINTEX_SIGNATURE_H

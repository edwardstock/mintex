/*!
 * mintex.
 * except
 *
 * \date 08.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_EXCEPT_H
#define MINTEX_EXCEPT_H

#include <exception>
#include <stdexcept>

namespace wallet {

 struct out_of_bound_error: public std::runtime_error {
   out_of_bound_error(const std::string &arg) : runtime_error(arg) { }
   out_of_bound_error(const char *string) : runtime_error(string) { }
   out_of_bound_error(const runtime_error &error) : runtime_error(error) { }
 };

}

#endif //MINTEX_EXCEPT_H

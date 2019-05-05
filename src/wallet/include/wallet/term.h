/*!
 * mintex.
 * term
 *
 * \date 04.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_TERM_H
#define MINTEX_TERM_H

#include <atomic>
#include <thread>

namespace wallet {
namespace term {

class progress_indeterminate {
public:
    progress_indeterminate();
    ~progress_indeterminate();
    void start();
    void stop();
private:
    std::thread *m_handler;
    std::atomic_bool m_started;

};

} // term
} // wallet

#endif //MINTEX_TERM_H

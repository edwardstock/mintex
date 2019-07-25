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
#include <iomanip>
#include <termios.h>
#include <errno.h>   /* for errno */
#include <unistd.h>  /* for EINTR */
#include <sstream>
#include <rang.hpp>
#include <toolboxpp.hpp>
#include <minter/address.h>
#include <minter/hash.h>
#include <minter/public_key.h>

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

template<typename T>
inline constexpr void print_field_data(const std::string &name, const T &data, size_t width = 4, size_t tab_cnt = 0) {
    std::cout << "\n";
    for (size_t i = 0; i < tab_cnt; i++) {
        std::cout << "\t";
    }
    std::cout << std::setw(width) << name << ": " << data;
}

template<typename T>
inline constexpr void print_field(const std::string &name, const T &data, size_t width) {
    print_field_data(name, data, width, 0);
}

template<typename T>
inline constexpr void print_tab_field(const std::string &name, const T &data, size_t width, size_t tab_cnt = 1) {
    print_field_data(name, data, width, tab_cnt);
}

void print_colored_message(const std::string &message,
                                  rang::fg fg = rang::fg::reset,
                                  rang::bg bg = rang::bg::reset);

void print_error_message(const std::string &message);
void print_success_message(const std::string &message);
void print_warning_message(const std::string &message);
void print_message_block(const std::string &title,
                                const std::string &message,
                                rang::fg fg = rang::fg::reset,
                                rang::bg bg = rang::bg::reset,
                                size_t target_len = 76);

} // term
} // wallet

#endif //MINTEX_TERM_H

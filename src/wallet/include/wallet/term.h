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
#include "mintex-tx/minter_address.h"
#include "mintex-tx/minter_hash.h"
#include "mintex-tx/minter_public_key.h"

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

inline std::string str_repeat(const std::string &in, size_t n) {
    std::stringstream ss;
    for (size_t i = 0; i < n; i++) {
        ss << in;
    }

    return ss.str();
}

inline std::vector<std::string> split_by_len(const std::string &src, size_t max) {
    std::vector<std::string> out;
    if (src.length() > max) {
        size_t got = 0;
        for (size_t i = 0; i < src.length(); i += max) {
            size_t n;
            if (got + max > src.length()) {
                n = src.length() - got;
            } else {
                n = got + max;
                got = n;
            }
            auto part = src.substr(i, n);
            if (part[0] == ' ') {
                part = part.substr(1, part.size());
            }

            out.push_back(part);
        }
    } else {
        out.push_back(src);
    }

    return out;
}

inline void print_colored_message(const std::string &message,
                                  rang::fg fg = rang::fg::reset,
                                  rang::bg bg = rang::bg::reset) {
    size_t target_len = 76;

    std::vector<std::string> message_parts = split_by_len(message, target_len);

    std::cout << bg << fg << str_repeat(" ", target_len + 4) << rang::fg::reset << rang::bg::reset << std::endl;
    for (const auto &msg: message_parts) {
        std::cout << bg << fg << "  " << msg << "  ";
        if (msg.length() + 2 < target_len) {
            std::cout << str_repeat(" ", target_len - msg.length());
        }

        std::cout << rang::fg::reset << rang::bg::reset << std::endl;
    }

    std::cout << bg << fg << str_repeat(" ", target_len + 4) << rang::fg::reset << rang::bg::reset << std::endl;
}

inline void print_error_message(const std::string &message) {
    print_colored_message(message, rang::fg::reset, rang::bg::red);
}

inline void print_success_message(const std::string &message) {
    print_colored_message(message, rang::fg::reset, rang::bg::green);
}

inline void print_warning_message(const std::string &message) {
    print_colored_message(message, rang::fg::black, rang::bg::yellow);
}

inline void print_message_block(const std::string &title,
                                const std::string &message,
                                rang::fg fg = rang::fg::reset,
                                rang::bg bg = rang::bg::reset) {
    size_t target_len = 76;

    std::vector<std::string> title_parts = split_by_len(title, target_len);
    std::vector<std::string> message_parts = split_by_len(message, target_len);

    size_t title_offset;
    if (title_parts.size() == 1) {
        title_offset = target_len - title.length();
        title_offset /= 2;
    } else {
        title_offset = 0;
    }

    std::cout << bg << fg << str_repeat(" ", target_len + 4) << rang::fg::reset << rang::bg::reset << std::endl;
    if (!title_offset) {
        for (const auto &msg: title_parts) {
            std::cout << bg << fg << "  " << msg << "  " << rang::fg::reset << rang::bg::reset << std::endl;
        }
    } else {
        std::cout << bg << fg << "  ";
        std::cout << str_repeat(" ", title_offset);
        std::cout << rang::style::underline << title_parts[0] << rang::style::reset << bg << fg;
        std::cout << str_repeat(" ", title_offset+1);
        std::cout << "  " << rang::fg::reset << rang::bg::reset << std::endl;
    }

    std::cout << bg << fg << str_repeat(" ", target_len + 4) << rang::fg::reset << rang::bg::reset << std::endl;
    for (const auto &msg: message_parts) {
        std::cout << bg << fg << "  " << msg << "  ";
        if (msg.length() + 2 < target_len) {
            std::cout << str_repeat(" ", target_len - msg.length());
        }

        std::cout << rang::fg::reset << rang::bg::reset << std::endl;
    }

    std::cout << bg << fg << str_repeat(" ", target_len + 4) << rang::fg::reset << rang::bg::reset << std::endl;
}

std::string prompt_password(const std::string &message, size_t pass_size = 32);

/* read a string from fp into pw masking keypress with mask char.
getpasswd will read upto sz - 1 chars into pw, null-terminating
the resulting string. On success, the number of characters in
pw are returned, -1 otherwise.
*/
//ssize_t getpasswd(char **pw, size_t sz, int mask = 0, FILE *fp = stdin);
ssize_t password(const std::string &message, char **pw, size_t sz, int mask = 0, FILE *fp = stdin);

} // term
} // wallet

#endif //MINTEX_TERM_H

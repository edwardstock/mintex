/*!
 * mintex.
 * term
 *
 * \date 04.05.19
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include <vector>
#include <string>
#include <iostream>
#include <rang.hpp>
#include <thread>
#include <chrono>

#include "wallet/term.h"

using namespace std::literals::chrono_literals;

wallet::term::progress_indeterminate::progress_indeterminate(): m_started(false) {

}

wallet::term::progress_indeterminate::~progress_indeterminate() {
    stop();
}
void wallet::term::progress_indeterminate::start() {
    if(m_started) return;

    m_handler = new std::thread([this]{
        m_started = true;
        std::vector<std::string> syms = {
                "[M     ]",
                "[ I    ]",
                "[  N   ]",
                "[   T  ]",
                "[    E ]",
                "[     R]",
                "[    E ]",
                "[   T  ]",
                "[  N   ]",
                "[ I    ]",
                "[⎺     ]",
                "[ ⎺    ]",
                "[  ⎺   ]",
                "[   ⎺  ]",
                "[    ⎺ ]",
                "[     ⎺]",
                "[     ⎯]",
                "[    ⎯ ]",
                "[   ⎯  ]",
                "[  ⎯   ]",
                "[ ⎯    ]",
                "[⎯     ]",
        };

        uint32_t i = 0;
        while (m_started) {
            std::cout << "\r" << rang::fgB::yellow << syms[i%syms.size()] << rang::style::reset << std::flush;
            std::this_thread::sleep_for(500ms);
            i++;
        }
        std::cout << rang::bg::reset << std::endl;
    });
}
void wallet::term::progress_indeterminate::stop() {
    if(m_handler == nullptr || !m_started) {
        return;
    }
    std::cout << "\r" << "        " << std::flush;
    m_started = false;
    m_handler->join();
    m_handler = nullptr;
}
void wallet::term::print_colored_message(const std::string &message, rang::fg fg, rang::bg bg) {
    using namespace toolboxpp::strings;
    size_t target_len = 76;

    std::vector<std::string> message_parts = toolboxpp::strings::splitByLength(message, target_len);

    std::cout << bg << fg << repeat(" ", target_len + 4) << rang::fg::reset << rang::bg::reset << std::endl;
    for (const auto &msg: message_parts) {
        std::cout << bg << fg << "  " << msg << "  ";
        if (msg.length() + 2 < target_len) {
            std::cout << repeat(" ", target_len - msg.length());
        }

        std::cout << rang::fg::reset << rang::bg::reset << std::endl;
    }

    std::cout << bg << fg << repeat(" ", target_len + 4) << rang::fg::reset << rang::bg::reset << std::endl;
}
void wallet::term::print_error_message(const std::string &message) {
    print_colored_message(message, rang::fg::black, rang::bg::red);
}
void wallet::term::print_success_message(const std::string &message) {
    print_colored_message(message, rang::fg::black, rang::bg::green);
}
void wallet::term::print_warning_message(const std::string &message) {
    print_colored_message(message, rang::fg::black, rang::bg::yellow);
}
void wallet::term::print_message_block(const std::string &title, const std::string &message, rang::fg fg, rang::bg bg, size_t target_len) {
    using namespace toolboxpp::strings;
    std::vector<std::string> title_parts = toolboxpp::strings::splitByLength(title, target_len);
    std::vector<std::string> message_parts = toolboxpp::strings::splitByLength(message, target_len);

    size_t title_offset;
    if (title_parts.size() == 1) {
        title_offset = target_len - title.length();
        title_offset /= 2;
    } else {
        title_offset = 0;
    }

    std::cout << bg << fg << repeat(" ", target_len + 4) << rang::fg::reset << rang::bg::reset << std::endl;
    if (!title_offset) {
        for (const auto &msg: title_parts) {
            std::cout << bg << fg << "  " << msg << "  " << rang::fg::reset << rang::bg::reset << std::endl;
        }
    } else {
        std::cout << bg << fg << "  ";
        std::cout << repeat(" ", title_offset);
        std::cout << rang::style::underline << title_parts[0] << rang::style::reset << bg << fg;
        std::cout << repeat(" ", title_offset+1);
        std::cout << "  " << rang::fg::reset << rang::bg::reset << std::endl;
    }

    std::cout << bg << fg << repeat(" ", target_len + 4) << rang::fg::reset << rang::bg::reset << std::endl;
    for (const auto &msg: message_parts) {
        std::cout << bg << fg << "  " << msg << "  ";
        if (msg.length() + 2 < target_len) {
            std::cout << repeat(" ", target_len - msg.length());
        }

        std::cout << rang::fg::reset << rang::bg::reset << std::endl;
    }

    std::cout << bg << fg << repeat(" ", target_len + 4) << rang::fg::reset << rang::bg::reset << std::endl;
}

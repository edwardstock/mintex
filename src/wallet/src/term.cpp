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
            std::cout << "\r" << rang::fgB::yellow << syms[i%syms.size()] << std::flush;
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
    std::cout << rang::style::reset;
    m_started = false;
    m_handler->join();
    m_handler = nullptr;
}

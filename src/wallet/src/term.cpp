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

ssize_t wallet::term::password(const std::string &message, char **pw, size_t sz, int mask, FILE *fp) {
    if (!pw || !sz || !fp) return -1;       /* validate input   */
#ifdef MAXPW
    if (sz > MAXPW) sz = MAXPW;
#endif

    std::cout << message << ": ";

    if (*pw == nullptr) {              /* reallocate if no address */
        void *tmp = realloc(*pw, sz * sizeof **pw);
        if (!tmp)
            return -1;
        memset(tmp, 0, sz);    /* initialize memory to 0   */
        *pw = static_cast<char *>(tmp);
    }

    size_t idx = 0;         /* index, number of chars in read   */
    int c = 0;

    termios old_kbd_mode;    /* orig keyboard settings   */
    termios new_kbd_mode;

    if (tcgetattr(0, &old_kbd_mode)) { /* save orig settings   */
        fprintf(stderr, "%s() error: tcgetattr failed.\n", __func__);
        return -1;
    }   /* copy old to new */
    memcpy(&new_kbd_mode, &old_kbd_mode, sizeof(struct termios));

    new_kbd_mode.c_lflag &= ~(ICANON | ECHO);  /* new kbd flags */
    new_kbd_mode.c_cc[VTIME] = 0;
    new_kbd_mode.c_cc[VMIN] = 1;
    if (tcsetattr(0, TCSANOW, &new_kbd_mode)) {
        fprintf(stderr, "%s() error: tcsetattr failed.\n", __func__);
        return -1;
    }

    /* read chars from fp, mask if valid char specified */
    while (((c = fgetc(fp)) != '\n' && c != EOF && idx < sz - 1) ||
        (idx == sz - 1 && c == 127)) {
        if (c != 127) {
            if (31 < mask && mask < 127)    /* valid ascii char */
                fputc(mask, stdout);
            (*pw)[idx++] = c;
        } else if (idx > 0) {         /* handle backspace (del)   */
            if (31 < mask && mask < 127) {
                fputc(0x8, stdout);
                fputc(' ', stdout);
                fputc(0x8, stdout);
            }
            (*pw)[--idx] = 0;
        }
    }
    (*pw)[idx] = 0; /* null-terminate   */

    /* reset original keyboard  */
    if (tcsetattr(0, TCSANOW, &old_kbd_mode)) {
        fprintf(stderr, "%s() error: tcsetattr failed.\n", __func__);
        return -1;
    }

    if (idx == sz - 1 && c != '\n') /* warn if pw truncated */
        fprintf(stderr, " (%s() warning: truncated at %zu chars.)\n",
                __func__, sz - 1);

    return idx; /* number of chars in passwd    */
}
std::string wallet::term::prompt_password(const std::string &message, size_t pass_size) {
    char tmp[pass_size];

    return std::string();
}

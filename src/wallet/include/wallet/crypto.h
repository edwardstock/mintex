/*!
 * mintex.
 * crypto.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_CRYPTO_H
#define MINTEX_CRYPTO_H
#include <memory>
#include <sodium.h>
#include <bip39/utils.h>
#include <aes256.hpp>
#include "mintex-tx/utils.h"

template<typename _CharT>
class sodium_s_alloc {
 public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _CharT *pointer;
    typedef const _CharT *const_pointer;
    typedef _CharT &reference;
    typedef const _CharT &const_reference;
    typedef _CharT value_type;

    template<class _Up>
    struct rebind { typedef sodium_s_alloc<_Up> other; };

    // return address of values
    pointer address(reference value) const {
        return &value;
    }
    const_pointer address(const_reference value) const {
        return &value;
    }

    sodium_s_alloc() throw() { }
    sodium_s_alloc(const sodium_s_alloc &) throw() { }

    template<class _Up>
    sodium_s_alloc(const sodium_s_alloc<_Up> &up) throw() { }

    ~sodium_s_alloc() throw() { }

    // return maximum number of elements that can be allocated
    size_type max_size() const throw() {
        return std::numeric_limits<std::size_t>::max() / sizeof(_CharT);
    }

    // initialize elements of allocated storage p with value value
    void construct(pointer p, const _CharT &value) {
        // initialize memory with placement new
        new((void *) p)_CharT(value);
    }

    pointer allocate(size_type __n, std::allocator<void>::const_pointer = 0) override {
        return static_cast<pointer>(sodium_allocarray(__n * sizeof(_CharT), __alignof(_CharT)));
    }

    // destroy elements of initialized storage p
    void destroy(pointer p) {
        // destroy objects by calling their destructor
        p->~_CharT();
    }

    // deallocate storage p of deleted elements
    void deallocate(pointer p, size_type num) {
        ::operator delete((void *) p);
    }
};

// return that all specializations of this allocator are interchangeable
template<class T1, class T2>
bool operator==(const sodium_s_alloc<T1> &,
                const sodium_s_alloc<T2> &) throw() {
    return true;
}
template<class T1, class T2>
bool operator!=(const sodium_s_alloc<T1> &,
                const sodium_s_alloc<T2> &) throw() {
    return false;
}

namespace wallet {
namespace crypto {

class mlock_guard {
 public:
    mlock_guard(): m_init(false) {}

    mlock_guard(void * ptr, size_t len) :
        m_ptr(ptr),
        m_len(len),
        m_init(true) {
        sodium_mlock(m_ptr, len);
    }
    mlock_guard(minter::Data &d) : mlock_guard(d.data(), d.size()) {
    }

//    mlock_guard(mlock_guard&& other):
//        m_ptr(other.m_ptr),
//        m_len(other.m_len),
//        m_init(other.m_init) {
//    }
//
//

    ~mlock_guard() {
        if (m_init) {
            sodium_munlock(m_ptr, m_len);
        }

    }
 private:
    bool m_init = false;
    void * m_ptr;
    size_t m_len;
};

std::string hash_argon2i(const std::string &data);
bool verify_argon2i(const std::string &data, const std::string &hash);

std::string encrypt_data(const minter::Data &key, const std::string &data);
std::string decrypt_data(const minter::Data &key, const std::string &data);
std::string encrypt_data_string(const std::string &key, const std::string &data);
std::string decrypt_data_string(const std::string &key, const std::string &data);

}
}

#endif //MINTEX_CRYPTO_H

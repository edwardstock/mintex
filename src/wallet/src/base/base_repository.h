/*!
 * mintex.
 * base_repository.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef MINTEX_BASE_REPOSITORY_H
#define MINTEX_BASE_REPOSITORY_H

#include <httb/httb.h>
#include <memory>

namespace mintex {
namespace net {

class base_repository {
public:

    using response_cb = std::function<void(httb::response)>;

    base_repository();
    virtual ~base_repository();

    virtual std::string getBaseUrl();

protected:
    httb::request newRequest() {
        return httb::request(getBaseUrl());
    }

    const std::unique_ptr<httb::client>& getClient() {
        return m_client;
    }
private:
    std::unique_ptr<httb::client> m_client;
};


}
}

#endif //MINTEX_BASE_REPOSITORY_H

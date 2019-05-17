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
#include <chrono>
#include <string>
#include <utility>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <thread>
#include <future>
#include <toolboxpp.hpp>
#include <mutex>

namespace wallet {
namespace net {

class base_repository {
 public:

    using response_cb = std::function<void(httb::response)>;

    base_repository();
    virtual ~base_repository();

    virtual std::string getBaseUrl() const;

    httb::request newRequest() const {
        return httb::request(getBaseUrl());
    }

    const std::shared_ptr<httb::client> &getClient() const {
        return m_client;
    }
 private:
    std::shared_ptr<httb::client> m_client;
};

using on_progress = std::function<void()>;
template<typename T>
using on_success = std::function<void(T)>;
template<typename T>
using on_error = std::function<void(httb::response, T)>;
using on_complete = std::function<void()>;

template<typename T>
class response_task;

template<typename T>
using task_ptr = std::shared_ptr<response_task<T>>;

template<typename T, typename... _Args>
inline task_ptr<T> make_task_ptr(_Args&&... __args) {
    return std::make_shared<response_task<T>>(std::forward<_Args>(__args)...);
}

template<typename T>
class response_task : public std::enable_shared_from_this<response_task<T>> {
 public:
    response_task(httb::request &&req) : m_request(std::move(req)) {
    }

    std::shared_ptr<response_task<T>> execute() {

        auto fut = std::async(std::launch::async, [this] {
//          auto weak_it = std::weak_ptr<response_task<T>>(this->shared_from_this());
//          std::shared_ptr<response_task<T>> ptr = weak_it.lock();

          httb::client client;
          auto resp = client.execute(m_request);
          nlohmann::json val = nlohmann::json::parse(resp.data);
          T res = val.get<T>();

          if (resp.isSuccess()) {
              if (m_on_success) {
                  m_on_success(res);
              }
          } else {
              if (m_on_error) {
                  m_on_error(resp, res);
              }
          }

          if (m_on_complete) {
              m_on_complete();
          }

          return res;
        });

        m_handle = std::move(fut);

        if (m_on_progress) {
            m_on_progress();
        }

        return this->shared_from_this();
    }

    std::shared_ptr<response_task<T>> progress(on_progress cb) {
        m_on_progress = cb;
        return this->shared_from_this();
    }

    std::shared_ptr<response_task<T>> success(on_success<T> cb) {
        m_on_success = cb;
        return this->shared_from_this();
    }

    std::shared_ptr<response_task<T>> error(on_error<T> cb) {
        m_on_error = cb;
        return this->shared_from_this();
    }

    std::shared_ptr<response_task<T>> complete(on_complete cb) {
        m_on_complete = cb;
        return this->shared_from_this();
    }

    httb::request get_request() const {
        return m_request;
    }

    std::shared_future<T> handle() {
        return m_handle.share();
    }
 private:
    httb::request m_request;
    on_progress m_on_progress;
    on_success<T> m_on_success;
    on_error<T> m_on_error;
    on_complete m_on_complete;
    std::future<T> m_handle;
};

template<typename A, typename B>
class pair_task {
 public:
    pair_task(task_ptr<A> task_a, task_ptr<B> task_b) :
        resp_a(std::move(task_a)),
        resp_b(std::move(task_b)) {
    }

    void execute() {
        resp_a->execute();
        resp_b->execute();

        resp_a->handle().wait();
        resp_b->handle().wait();
    }
 private:
    task_ptr<A> resp_a;
    task_ptr<B> resp_b;
};




}
}

#endif //MINTEX_BASE_REPOSITORY_H

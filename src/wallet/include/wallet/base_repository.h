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
#include <boost/asio/io_context.hpp>

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
 private:
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
    response_task(httb::request &&req) :
        m_ctx(),
        m_request(std::move(req))
        {
    }

    std::shared_ptr<response_task<T>> execute() {
        client.executeInContext(m_ctx, m_request, [this](httb::response resp) {
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
        });

        if (m_on_progress) {
            m_on_progress();
        }

        return this->shared_from_this();
    }

    std::shared_ptr<response_task<T>> execute(boost::asio::io_context &io_ctx) {
        client.executeInContext(io_ctx, m_request, [this](httb::response resp) {
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
        });

        if (m_on_progress) {
            m_on_progress();
        }

        return this->shared_from_this();
    }

    std::shared_ptr<response_task<T>> progress(const on_progress &cb) {
        m_on_progress = cb;
        return this->shared_from_this();
    }

    std::shared_ptr<response_task<T>> success(const on_success<T> &cb) {
        m_on_success = cb;
        return this->shared_from_this();
    }

    std::shared_ptr<response_task<T>> error(const on_error<T> &cb) {
        m_on_error = cb;
        return this->shared_from_this();
    }

    std::shared_ptr<response_task<T>> complete(const on_complete &cb) {
        m_on_complete = cb;
        return this->shared_from_this();
    }

    httb::request get_request() const {
        return m_request;
    }

    boost::asio::io_context& handle() {
        return m_ctx;
    }
 private:
    httb::client client;
    boost::asio::io_context m_ctx;
    httb::request m_request;
    on_progress m_on_progress;
    on_success<T> m_on_success;
    on_error<T> m_on_error;
    on_complete m_on_complete;
};

template<typename A, typename B>
class pair_task {
 public:
    pair_task(task_ptr<A> task_a, task_ptr<B> task_b) :
        resp_a(std::move(task_a)),
        resp_b(std::move(task_b)) {
    }

    void execute() {
        boost::asio::io_context ctx(2);
        resp_a->execute(ctx);
        resp_b->execute(ctx);

        ctx.run();
    }
 private:
    task_ptr<A> resp_a;
    task_ptr<B> resp_b;
};




}
}

#endif //MINTEX_BASE_REPOSITORY_H

/*!
 * mintex.
 * base_repository.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "wallet/base_repository.h"

wallet::net::base_repository::base_repository():
    m_client(std::make_shared<httb::client>())
    {

}
wallet::net::base_repository::~base_repository() {

}
std::string wallet::net::base_repository::getBaseUrl() const {
    return std::string();
}

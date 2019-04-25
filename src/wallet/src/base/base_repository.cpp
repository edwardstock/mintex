/*!
 * mintex.
 * base_repository.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "base_repository.h"

mintex::net::base_repository::base_repository():
    m_client(std::make_unique<httb::client>())
    {

}
mintex::net::base_repository::~base_repository() {

}
std::string mintex::net::base_repository::getBaseUrl() {
    return std::string();
}

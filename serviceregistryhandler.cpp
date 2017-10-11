#include "serviceregistryhandler.h"
#include <proxygen/httpserver/ResponseBuilder.h>
#include <folly/FBString.h>
#include <folly/dynamic.h>
#include <sstream>

ServiceRegistryHandler::ServiceRegistryHandler(ServiceRegistry *registry)
    : _registry(registry) {}

void ServiceRegistryHandler::onRequest(
    std::unique_ptr<proxygen::HTTPMessage> message) noexcept {
  _method = message->getMethodString();
  if (_method == "POST") {
  } else if (_method == "GET") {
      std::string path = message->getPath();
      u_long p = path.find_last_of("/");
      std::string name = path.substr(p+1 );
      LOG(INFO) << "name is " << name;
      auto nodes = _registry->getServiceNodes(name);
      folly::dynamic res = folly::dynamic::array();
      for (auto &n: nodes) {
          res.push_back(n->to_dynamic());
      }

      std::stringstream ss;
      ss << res;
      std::string res_str = ss.str();
      std::unique_ptr<folly::IOBuf> buffer = folly::IOBuf::copyBuffer(res_str);
      proxygen::ResponseBuilder(downstream_)
              .status(200, "OK")
              .body(std::move(buffer))
              .sendWithEOM();
  }
}

void ServiceRegistryHandler::onBody(
    std::unique_ptr<folly::IOBuf> body) noexcept {
  if (_body) {
    _body->prependChain(std::move(body));
  } else {
    _body = std::move(body);
  }
}

void ServiceRegistryHandler::onEOM() noexcept {
  if (_body && _method == "POST") {

    _body->coalesce();
    const char *data = reinterpret_cast<const char *>(_body->data());
    std::string body_str(data, _body->length());
    _body->clear();
    auto node = ServiceNode::parseJson(body_str);
    _registry->registerServiceNode(node->getServiceName(), node->getUrl());
    proxygen::ResponseBuilder(downstream_).status(200, "OK").sendWithEOM();
  }
}

void ServiceRegistryHandler::onUpgrade(
    proxygen::UpgradeProtocol proto) noexcept {}

void ServiceRegistryHandler::requestComplete() noexcept {}

void ServiceRegistryHandler::onError(proxygen::ProxygenError err) noexcept {}

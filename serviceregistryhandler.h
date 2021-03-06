#ifndef SERVICEREGISTRYHANDLER_H
#define SERVICEREGISTRYHANDLER_H

#include <proxygen/httpserver/RequestHandler.h>
#include "serviceregistry.h"
#include <memory>

using std::shared_ptr;
using proxygen::RequestHandler;

class ServiceRegistryHandler : public RequestHandler {
public:
  ServiceRegistryHandler();

  void
  onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

  void onEOM() noexcept override;

  void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

  void requestComplete() noexcept override;

  void onError(proxygen::ProxygenError err) noexcept override;
private:
  shared_ptr<ServiceRegistry> _registry;
  std::unique_ptr<folly::IOBuf> _body;
  std::string _method;
};

#endif // SERVICEREGISTRYHANDLER_H

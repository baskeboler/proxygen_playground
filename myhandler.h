#ifndef MYHANDLER_H
#define MYHANDLER_H


#include <folly/Memory.h>
#include <proxygen/httpserver/RequestHandler.h>

namespace proxygen {
class ResponseHandler;
}

namespace EchoService {

//class EchoStats;

class EchoHandler : public proxygen::RequestHandler {
 public:
  explicit EchoHandler();

  void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers)
      noexcept override;

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

  void onEOM() noexcept override;

  void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

  void requestComplete() noexcept override;

  void onError(proxygen::ProxygenError err) noexcept override;

 private:
//  EchoStats* const stats_{nullptr};

  std::unique_ptr<folly::IOBuf> body_;
};

}
#endif // MYHANDLER_H

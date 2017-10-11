#ifndef TIMEHANDLER_H
#define TIMEHANDLER_H

#include <proxygen/httpserver/RequestHandler.h>
using proxygen::RequestHandler;

class TimeHandler: public RequestHandler
{
public:
    TimeHandler();

    void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers)
        noexcept override;

    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

    void onEOM() noexcept override;

    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

    void requestComplete() noexcept override;

    void onError(proxygen::ProxygenError err) noexcept override;

};

#endif // TIMEHANDLER_H

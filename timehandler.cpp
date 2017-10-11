#include "timehandler.h"
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <folly/io/IOBuf.h>
#include <proxygen/httpserver/ResponseBuilder.h>

TimeHandler::TimeHandler() {}

void TimeHandler::onRequest(
    std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {}

void TimeHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {}

void TimeHandler::onEOM() noexcept {
  boost::gregorian::date d{boost::gregorian::day_clock::local_day()};
  std::stringstream ss;
  ss << d;
  // downstream_->
  proxygen::ResponseBuilder(downstream_)
      .status(200, "OK")
      .body(folly::IOBuf::copyBuffer(ss.str()))
      .sendWithEOM();
}

void TimeHandler::onUpgrade(proxygen::UpgradeProtocol proto) noexcept {}

void TimeHandler::requestComplete() noexcept { delete this; }

void TimeHandler::onError(proxygen::ProxygenError err) noexcept { delete this; }

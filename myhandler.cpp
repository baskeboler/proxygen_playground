/*
 *  Copyright (c) 2017, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include "myhandler.h"

#include <folly/FBString.h>
#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>
//#include "EchoStats.h"
#include <glog/logging.h>

using namespace proxygen;
using namespace folly;
using std::string;

namespace EchoService {

EchoHandler::EchoHandler() {}

void EchoHandler::onRequest(std::unique_ptr<HTTPMessage> headers) noexcept {
  //  stats_->recordRequest();
  LOG(INFO) << headers->getPath() << std::endl;
  headers->getHeaders().forEach(
      [this](const string &header, const string &value) {
        string s = header + ": " + value + "\n";
        auto a = IOBuf::copyBuffer(s);
        if (body_) {
          body_->appendChain(std::move(a));
        } else {
          body_ = std::move(a);
        }

      });
}

void EchoHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
  if (body_) {
    body_->prependChain(std::move(body));
  } else {
    body_ = std::move(body);
  }
}

void EchoHandler::onEOM() noexcept {
  ResponseBuilder(downstream_)
      .status(200, "OK")
      .header("Request-Number", folly::to<std::string>(666))
      .body(std::move(body_))
      .sendWithEOM();
}

void EchoHandler::onUpgrade(UpgradeProtocol /*protocol*/) noexcept {
  // handler doesn't support upgrades
}

void EchoHandler::requestComplete() noexcept { delete this; }

void EchoHandler::onError(ProxygenError /*err*/) noexcept { delete this; }
}

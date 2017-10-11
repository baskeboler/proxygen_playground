#include "myhandler.h"
#include "timehandler.h"
#include <folly/FBString.h>
#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <unistd.h>
//#include "EchoStats.h"
#include "serviceregistry.h"
#include "serviceregistryhandler.h"
#include <string>

using namespace EchoService;
using namespace proxygen;

using folly::EventBase;
using folly::EventBaseManager;
using folly::SocketAddress;

using Protocol = HTTPServer::Protocol;

DEFINE_int32(http_port, 11000, "Port to listen on with HTTP protocol");
DEFINE_int32(spdy_port, 11001, "Port to listen on with SPDY protocol");
DEFINE_int32(h2_port, 11002, "Port to listen on with HTTP/2 protocol");
DEFINE_string(ip, "localhost", "IP/Hostname to bind to");
DEFINE_int32(threads, 1, "Number of threads to listen on. Numbers <= 0 "
                         "will use the number of cores on this machine.");

class EchoHandlerFactory : public RequestHandlerFactory {
public:
  void onServerStart(folly::EventBase * /*evb*/) noexcept override {
    //    stats_.reset(new EchoStats);
    _registry.reset(new ServiceRegistry);
  }

  void onServerStop() noexcept override {
    //    stats_.reset();
  }

  RequestHandler *onRequest(RequestHandler *,
                            HTTPMessage *m) noexcept override {
    std::cout << "got request" << m << std::endl;
    std::string path = m->getPath();
    std::cout << "path is " << path << std::endl;
    if (path == "/time") {
      std::cout << "Time request" << std::endl;
      return new TimeHandler();
    } else if (path.find("/registry") == 0) {
      return new ServiceRegistryHandler(_registry.get());
    } else {

      return new EchoHandler();
    }
  }

private:
  // folly::ThreadLocalPtr<EchoStats> stats_;
  folly::ThreadLocalPtr<ServiceRegistry> _registry;
};

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  LOG(INFO) << "Initializing server";
  std::vector<HTTPServer::IPConfig> IPs = {
      {SocketAddress(FLAGS_ip, FLAGS_http_port, true), Protocol::HTTP},
      {SocketAddress(FLAGS_ip, FLAGS_spdy_port, true), Protocol::SPDY},
      {SocketAddress(FLAGS_ip, FLAGS_h2_port, true), Protocol::HTTP2},
  };

  if (FLAGS_threads <= 0) {
    FLAGS_threads = sysconf(_SC_NPROCESSORS_ONLN);
    CHECK(FLAGS_threads > 0);
  }

  HTTPServerOptions options;
  options.threads = static_cast<size_t>(FLAGS_threads);
  options.idleTimeout = std::chrono::milliseconds(60000);
  options.shutdownOn = {SIGINT, SIGTERM};
  options.enableContentCompression = false;
  options.handlerFactories =
      RequestHandlerChain().addThen<EchoHandlerFactory>().build();
  options.h2cEnabled = true;

  HTTPServer server(std::move(options));
  server.bind(IPs);

  // Start HTTPServer mainloop in a separate thread
  std::thread t([&]() { server.start(); });

  t.join();
  return 0;
}

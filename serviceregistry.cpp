#include "serviceregistry.h"
#include <algorithm>
#include <glog/logging.h>
#include <folly/String.h>
#include <folly/Singleton.h>

namespace {
    struct ServiceRegistryTag {};
}
static folly::Singleton<ServiceRegistry, ServiceRegistryTag> the_instance;
ServiceRegistry::ServiceRegistry() {
  LOG(INFO) << "Initializing Service Registry";
}

vector<ServiceNodePtr>
ServiceRegistry::getServiceNodes(const std::string &serviceName) {
  auto locked = _serviceMap.rlock();
  auto search = locked->find(serviceName);
  if (search != locked->end()) {
    auto v = search->second;
    return v;
  } else
    return {};
}

void ServiceRegistry::registerServiceNode(const std::string &serviceName,
                                          const std::string &url) {
  _serviceMap.withWLock([&](auto &map) {
    auto search = map.find(serviceName);
    ServiceNodePtr node = make_shared<ServiceNode>(serviceName, url);
    if (search != map.end()) {
      if (std::find_if(search->second.begin(), search->second.end(),
                       [node](auto ptr) { return *ptr == *node; }) ==
          search->second.end()) {

        search->second.insert(search->second.end(), node);
      }
    } else {
      vector<ServiceNodePtr> v = {node};
      map[serviceName] = v;
    }
  });
}

shared_ptr<ServiceRegistry> ServiceRegistry::getInstance()
{
    return the_instance.try_get();
}

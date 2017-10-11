#ifndef SERVICEREGISTRY_H
#define SERVICEREGISTRY_H

#include "servicenode.h"
#include <folly/Synchronized.h>
#include <map>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::map;
using folly::Synchronized;

typedef shared_ptr<ServiceNode> ServiceNodePtr;

class ServiceRegistry {
public:
  ServiceRegistry();
  vector<ServiceNodePtr> getServiceNodes(const string &serviceName);
  void registerServiceNode(const string &serviceName, const string &url);

private:
  Synchronized<map<string, vector<ServiceNodePtr>>> _serviceMap;
};

#endif // SERVICEREGISTRY_H

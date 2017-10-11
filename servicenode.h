#ifndef SERVICENODE_H
#define SERVICENODE_H
#include <memory>
#include <ostream>
#include <string>
#include <folly/dynamic.h>

using std::shared_ptr;
using std::string;

class ServiceNode {
public:
  ServiceNode();
  ServiceNode(const string &serviceName, const string &url);
  string getUrl() const;

  string getServiceName() const;

  bool operator==(const ServiceNode &other) const;
  bool operator!=(const ServiceNode &other) const;
  friend std::ostream &operator<<(std::ostream &os, const ServiceNode &c);

  static shared_ptr<ServiceNode> parseJson(const std::string &str);
  folly::dynamic to_dynamic() const;
private:
  string _serviceName;
  string _url;
};

#endif // SERVICENODE_H

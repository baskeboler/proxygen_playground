#include "servicenode.h"
#include <folly/dynamic.h>
#include <folly/json.h>

using std::make_shared;
ServiceNode::ServiceNode()
{

}

ServiceNode::ServiceNode(const std::string &serviceName, const std::string &url): _serviceName(serviceName), _url(url) {}

string ServiceNode::getUrl() const
{
    return _url;
}

string ServiceNode::getServiceName() const
{
    return _serviceName;
}

bool ServiceNode::operator==(const ServiceNode &other) const {
    return _serviceName == other.getServiceName() && _url == other.getUrl();
}

bool ServiceNode::operator!=(const ServiceNode &other) const {
    return (*this == other) == false;
}

shared_ptr<ServiceNode> ServiceNode::parseJson(const std::string& str) {
    folly::dynamic json_obj = folly::parseJson(str);
    if (json_obj.count("service-name") && json_obj.count("url")) {
        return make_shared<ServiceNode>(json_obj["service-name"].asString(), json_obj["url"].asString());
    } else return nullptr;
}

folly::dynamic ServiceNode::to_dynamic() const
{
    return folly::dynamic::object("service-name", _serviceName)("url", _url);
}

std::ostream &operator<<(std::ostream &os, const ServiceNode &c)
{
    folly::dynamic d = folly::dynamic::object("service-name", c.getServiceName())
            ("url", c.getUrl());
    os << d;
    return os;
}

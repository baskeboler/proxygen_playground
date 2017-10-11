#include <folly/portability/GTest.h>
#include "servicenode.h"
#include "serviceregistry.h"
TEST(ServiceNode, create) {

    EXPECT_NO_FATAL_FAILURE(ServiceNode node;);
}

TEST(ServiceNode, createWithArgs) {

    ServiceNode n("service name", "http://localhost");
    EXPECT_STREQ(n.getServiceName().c_str(), "service name");
    EXPECT_STREQ(n.getUrl().c_str(), "http://localhost");
}

TEST(ServiceNode, compare) {
    ServiceNode n1("service name", "http://localhost");
    ServiceNode n2("service name", "http://localhost");
    ServiceNode n3("service name 2", "http://localhost");
    ServiceNode n4("service name", "2http://localhost");
    EXPECT_EQ(n1, n2);
    EXPECT_NE(n1, n3);
    EXPECT_NE(n3, n1);
    EXPECT_NE(n1, n4);
}

TEST(ServiceRegistry, create) {
    EXPECT_NO_FATAL_FAILURE(ServiceRegistry r;);

}


TEST(ServiceRegistry, register) {
    ServiceRegistry r;
    r.registerServiceNode("service name", "http://localhost");
    auto v = r.getServiceNodes("service name");
    EXPECT_TRUE(v.size() == 1);
    for (auto n: v) {
        std::cout << *n << std::endl;
    }
}

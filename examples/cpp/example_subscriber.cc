//
// Created by pairs on 7/29/18.
//

#include <test_message.pb.h>
#include <zros/zros.h>
#include <zros/node_handle.h>

int main() {
    // 1. init & run service discovery
    // for service discovery
    const std::string master_address = "localhost:23333";
    const std::string agent_address = "localhost:40004"; // listen address for accept register/unregister request
    zros::init(master_address, agent_address);

    // 2. create node
    const std::string node_address = "localhost:50004";
    const std::string node_name = "example_subscriber";
    auto node_handle = std::make_shared<zros::NodeHandle>(node_address, node_name);

    // 3. create subscriber
    const std::string topic = "test_topic";
    auto sub = node_handle->subscribe<zros_example::TestMessage> (topic, [](const zros_example::TestMessage message) {
        SSPD_LOG_INFO << "receive publish message " << message.detail();
    });

    getchar();
}
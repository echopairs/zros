//
// Created by pairs on 7/26/18.
//

#include <test_message.pb.h>
#include <zros/zros.h>
#include <zros/node_handle.h>

int main() {

    // 1. init & run service discovery
    // for service discovery
    const std::string master_address = "localhost:23333";
    zros::init(master_address);

    // 2. create node
    const std::string node_name = "example_publisher";
    auto node_handle = std::make_shared<zros::NodeHandle>(node_name);

    // 3. create publisher
    const std::string topic = "test_topic";
    auto pub = node_handle->advertise<zros_example::TestMessage> (topic);

    std::thread pub_thread = std::thread([pub](){
        while(true) {
            zros_example::TestMessage message;
            message.set_detail("hello world");
            pub->publish(message);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    pub_thread.join();
}
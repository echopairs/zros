//
// Created by pairs on 7/15/18.
//

#include <zros/node_handle.h>
#include <zros.pb.h>

namespace zros {

    NodeHandle::NodeHandle(const std::string &node_address, const std::string &node_name):
            node_address_(node_address),node_name_(node_name) {
        init();
    }

    NodeHandle::NodeHandle(const std::string &node_name):
            node_name_(node_name), node_address_("[::]:") {
        init();
    }

    void NodeHandle::init() {
        thread_pool_ = std::make_shared<ThreadPool>(8);
        service_server_mgr_ = std::make_shared<ServiceServerManager>(node_address_);
        service_client_mgr_ = std::make_shared<ServiceClientManager>();
        publisher_manager_ = std::make_shared<PublisherManager>();
        subscriber_manager_ = std::make_shared<SubscriberManager>();
        spin();
    }
    void NodeHandle::spin() {
        auto real_address = service_server_mgr_->start();
        set_node_address(real_address);
    }

    const string &NodeHandle::get_node_address() const {
        return node_address_;
    }

    void NodeHandle::set_node_address(const std::string &address) {
        node_address_ = address;
    }

    const string &NodeHandle::get_node_name() const {
        return node_name_;
    }

    std::shared_ptr<zros_rpc::ServiceResponse>
    NodeHandle::call(const std::string &service_name, const std::string &content, const std::string &cli_info,
                     int timeout_mseconds) {
        return service_client_mgr_->call(service_name, content, cli_info, timeout_mseconds);
    }

    void NodeHandle::publish(const std::string &topic, const std::string &content) {
        publisher_manager_->publish(topic, content);
    }

}

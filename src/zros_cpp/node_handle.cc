//
// Created by pairs on 7/15/18.
//

#include <zros/node_handle.h>
#include <zros.pb.h>
#include "zros/service_server_manager.h"

namespace zros {
    NodeHandle::NodeHandle(const std::string &node_address, const std::string &node_name):
            node_address_(node_address),node_name_(node_name) {
        thread_pool_ = std::make_shared<ThreadPool>(8);
        service_server_mgr_ = std::make_shared<ServiceServerManager>(node_address);
        service_client_mgr_ = std::make_shared<ServiceClientManager>();
    }

    void NodeHandle::spin() {
        service_server_mgr_->start();
    }

    const string &NodeHandle::get_node_address() const {
        return node_address_;
    }

    const string &NodeHandle::get_node_name() const {
        return node_name_;
    }

    std::shared_ptr<zros_rpc::ServiceResponse>
    NodeHandle::call(const std::string &service_name, const std::string &content, const std::string &cli_info,
                     int timeout_mseconds) {
        return shared_ptr<zros_rpc::ServiceResponse>();
    }
}

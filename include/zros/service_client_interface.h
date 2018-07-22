//
// Created by pairs on 7/11/18.
//

#pragma once

#include <string>
#include <atomic>
#include <memory>
#include <zros.grpc.pb.h>

namespace zros {

    class NodeHandle;
    class IServiceClient {
    public:
        IServiceClient(const std::string & service_name, std::shared_ptr<NodeHandle> node_handle):
                service_name_(service_name),
                node_handle_(node_handle) {
            is_ready_ = false;
        }

        const std::string &get_service_name() {
            return service_name_;
        }

        const std::shared_ptr<NodeHandle> &get_node_handle() const {
            return node_handle_;
        }

        std::shared_ptr<zros_rpc::ServiceResponse> call_(const std::string &content, const std::string &cli_info, int timeout);
        void set_ready(bool ready) { is_ready_ = ready; }
    protected:
        std::atomic<bool> is_ready_;
        std::shared_ptr<NodeHandle> node_handle_;
        std::string service_name_;
    };
}

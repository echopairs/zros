//
// Created by pairs on 7/11/18.
//


#pragma once

#include "zros.grpc.pb.h"
#include <string>

namespace zros {
    class NodeHandle;
    class IServiceServer {
    public:
        IServiceServer(const std::string & service_name, std::shared_ptr<NodeHandle>node_handle ):
                service_name_(service_name),
                node_handle_(node_handle) {}
        virtual void invoke(const zros_rpc::ServiceRequest * request, zros_rpc::ServiceResponse * response) = 0;
        const std::string &get_service_name() {
            return service_name_;
        };

        const std::shared_ptr<NodeHandle> &get_node_handle() const {
            return node_handle_;
        }
    protected:
        std::shared_ptr<NodeHandle> node_handle_;
        std::string service_name_;
    };
}
//
// Created by pairs on 7/11/18.
//

#pragma once

#include "service_client_interface.h"
#include <zros.grpc.pb.h>
#include <memory>

namespace zros {
    class NodeHandle;
    template <typename TRequest, typename TResponse>
    class ServiceClient : public IServiceClient {
    public:
        ServiceClient(const std::string &service_name, const std::string &client_info, std::shared_ptr<NodeHandle> node_handle):
                IServiceClient(service_name, node_handle), cli_info_(client_info) {
        }

        zros_rpc::Status call (const TRequest * request, TResponse * response, int timeout = -1) {
            if (!is_ready_) {
                zros_rpc::Status status;
                status.set_code(status.UNKNOWN);
                status.set_details("server not ready");
                return status;
            }
            auto request_data = serializeToString(*request);
            auto res = call_(request_data, cli_info_, timeout);
            parseFromString(res->response_data(), response);
            return res->status();
        }

        // todo add asyncCall
    protected:
        std::string cli_info_;

    };
}

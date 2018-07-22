//
// Created by pairs on 7/20/18.
//

#include <zros/service_client_interface.h>
#include <zros/node_handle.h>

namespace zros {
    std::shared_ptr<zros_rpc::ServiceResponse> IServiceClient::call_(const std::string &content, const std::string &cli_info, int timeout) {
        return node_handle_->call(service_name_, content, cli_info, timeout);
    }
}
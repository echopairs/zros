//
// Created by pairs on 7/11/18.
//

#pragma once

#include "service_server_interface.h"
#include "serialization_helper.h"
#include <functional>

namespace zros {
    class NodeHandle;
    template <typename TRequest, typename TResponse>
    class ServiceServer : public IServiceServer {
    public:
        ServiceServer (
                const std::string & service_name,
                std::function<zros_rpc::Status(const TRequest * request, TResponse * response)> serviceCb,
                std::shared_ptr<NodeHandle> node_handle)
                : IServiceServer(service_name, node_handle), service_cb_(serviceCb){
        }

        void invoke(const zros_rpc::ServiceRequest * request, zros_rpc::ServiceResponse *response) override {
            TRequest tRequest;
            TResponse tResponse;
            parseFromString(request->request_data(), &tRequest);
            auto status = service_cb_(&tRequest, &tResponse);
            response->set_response_data(serializeToString(tResponse));
            response->mutable_status()->set_code(status.code());
            response->mutable_status()->set_details(status.details());
        }
    private:

        std::function<zros_rpc::Status(const TRequest * request, TResponse * response)> service_cb_;
    };
}

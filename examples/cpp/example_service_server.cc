//
// Created by pairs on 7/11/18.
//

#include <zros/zros.h>
#include <zros/node_handle.h>
#include <zros/service_server.h>
#include <zros/service_server.h>
#include <sspdlog/sspdlog.h>
#include "test_service.pb.h"

int main() {

    // 1. init & run service discovery
    // for service discovery
    const std::string master_address = "localhost:23333";
    const std::string agent_address = "localhost:40001";    // listen address for accept register/unregister request
    zros::init(master_address, agent_address);

    // 2. create node
    const std::string node_address = "localhost:50001";
    const std::string node_name = "example_service_server";
    auto node_handle = std::make_shared<zros::NodeHandle>(node_address, node_name);

    // 3. create service server
    const std::string service_name = "test_service";
    node_handle->advertiseService<zros_example::TestServiceRequest, zros_example::TestServiceResponse>(
            service_name,
            [](const zros_example::TestServiceRequest * request, zros_example::TestServiceResponse * response) -> zros_rpc::Status {
                response->set_detail("response of " + request->detail());
                SSPD_LOG_INFO << "the request detail is: " << strlen(request->detail().c_str());
                return zros_rpc::Status();
            });
    getchar();
}
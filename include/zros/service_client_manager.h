//
// Created by pairs on 7/17/18.
//

#pragma once
#include "service_client_interface.h"
#include "service_discovery.h"

namespace zros {
    class ServiceClientsImpl;
    class ServiceClientManager {
    public:
        ServiceClientManager();
        bool registerClient(const std::shared_ptr<IServiceClient> client);
        bool unregisterClient(const std::string &service_name);
        std::shared_ptr<zros_rpc::ServiceResponse> call(const std::string &service_name, const std::string & content,
                                                        const std::string &cli_info, int timeout_mseconds);
    protected:
        std::shared_ptr<ServiceDiscovery>  service_discovery_;
        std::shared_ptr<ServiceClientsImpl> impl_;
    };
}
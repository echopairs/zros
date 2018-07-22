//
// Created by pairs on 7/12/18.
//

#pragma once

#include <memory>
#include <zros/service_server.h>
#include <zros/service_client.h>

namespace zros {
    class ServiceDiscoveryImpl;

    class ServiceDiscovery {
    public:
        // init construct
        ServiceDiscovery(const std::string &master_address, const std::string &agent_address);

        // get construct
        ServiceDiscovery();
        void init();
        void spin();

        bool addServiceServer(const std::shared_ptr<IServiceServer> server);
        bool addServiceClient(const std::shared_ptr<IServiceClient> client);
    private:
        std::shared_ptr<ServiceDiscoveryImpl> impl_;
    };
}   // namespace nsky

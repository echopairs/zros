//
// Created by pairs on 7/16/18.
//

#pragma once

#include <string>
#include "service_server_interface.h"
#include "service_discovery.h"

namespace zros {
    class ServiceDiscovery;
    class IServersImpl;
    class ServiceServerManager {
    public:
        ServiceServerManager(const std::string & service_address);
        const std::string& start();
        void stop();
        bool registerServer(const std::shared_ptr<IServiceServer> server);
        bool unregisterServer(const std::string & service_name);
    protected:
        std::shared_ptr<ServiceDiscovery>  service_discovery_;
        std::shared_ptr<IServersImpl> impl_;
    };
}

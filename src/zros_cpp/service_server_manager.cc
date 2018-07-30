//
// Created by pairs on 7/16/18.
//

#include <zros/service_server_manager.h>
#include <zros/singleton.h>
#include <sspdlog/sspdlog.h>
#include "impl/service_servers_impl.h"

namespace zros {

    ServiceServerManager::ServiceServerManager(const std::string &service_address) {
        impl_ = std::make_shared<GrpcServersImpl>(service_address);
        service_discovery_ = SingleTon<ServiceDiscovery>::Instance();
    }

    const std::string& ServiceServerManager::start() {
        impl_->start();
    }

    void ServiceServerManager::stop() {
        impl_->stop();
    }

    bool ServiceServerManager::registerServer(const std::shared_ptr<zros::IServiceServer> server) {
        // 1. register to master first
        bool ok = service_discovery_->addServiceServer(server);
        if (ok) {
            // 2. register to memory
            SSPD_LOG_INFO << "register server " << server->get_service_name() << " to master success";
            return impl_->registerServer(server);
        }
        SSPD_LOG_WARNING << "register server " << server->get_service_name() << " to master failed";
        return false;
    }

    bool ServiceServerManager::unregisterServer(const std::string &service_name) {
        impl_->unregisterServer(service_name);
    }
}
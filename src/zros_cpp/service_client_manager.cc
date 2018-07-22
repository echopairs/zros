//
// Created by pairs on 7/17/18.
//

#include <zros/service_client_manager.h>
#include <zros/singleton.h>
#include <sspdlog/sspdlog.h>
#include "impl/service_clients_impl.h"

namespace zros {

    ServiceClientManager::ServiceClientManager() {
        impl_ = std::make_shared<ServiceClientsImpl>();
        service_discovery_ = SingleTon<ServiceDiscovery>::Instance();
    }

    bool ServiceClientManager::registerClient(const std::shared_ptr<zros::IServiceClient> client) {
        // 1. register to master first
        bool ok = service_discovery_->addServiceClient(client);
        if (ok) {
            // todo
            SSPD_LOG_INFO << "register client " << client->get_service_name() << " to master success";
            return true;
         }
        SSPD_LOG_WARNING << "register client " << client->get_service_name() << " to master failed";
        return false;
    }

    bool ServiceClientManager::unregisterClient(const std::string &service_name) {
        // todo
        return false;
    }

    std::shared_ptr<zros_rpc::ServiceResponse>
    ServiceClientManager::call(const std::string &service_name, const std::string &content, const std::string &cli_info,
                               int timeout_mseconds) {
        return impl_->call(service_name, content, cli_info, timeout_mseconds);
    }
}
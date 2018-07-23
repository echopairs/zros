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
        service_discovery_->set_register_service_server_cb([this](const zros_rpc::ServiceServerInfo *serverInfo, zros_rpc::Status *status){
            impl_->registerServiceClient(serverInfo, status);
        });
        service_discovery_->set_unregister_service_server_cb([this](const zros_rpc::ServiceServerInfo *serverInfo, zros_rpc::Status *status) {
            impl_->unregisterServiceClient(serverInfo, status);
        });


    }

    bool ServiceClientManager::registerClient(const std::shared_ptr<zros::IServiceClient> client) {
        // 1. register to master first
        bool ok = service_discovery_->addServiceClient(client);
        if (ok) {
            SSPD_LOG_INFO << "register client " << client->get_service_name() << " to master success";
            // 2. register to memory
            impl_->registerClient(client);
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
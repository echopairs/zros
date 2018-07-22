//
// Created by pairs on 7/13/18.
//

#include <zros/service_discovery.h>
#include <zros/error.h>
#include "impl/service_discovery_impl.h"

namespace zros {
    ServiceDiscovery::ServiceDiscovery(const std::string &master_address, const std::string &agent_address) {
        impl_ = std::make_shared<ServiceDiscoveryImpl>(master_address, agent_address);
    }

    void ServiceDiscovery::init() {
        if (!impl_->isConnectedToMaster()) {
            throw initialize_error("Cannot connect to Master");
        }
    }

    void ServiceDiscovery::spin() {
        impl_->spin();
    }

    ServiceDiscovery::ServiceDiscovery() {
        if (!impl_) {
            throw initialize_error("please init service discovery first");
        }
    }

    bool ServiceDiscovery::addServiceServer(const std::shared_ptr<IServiceServer> server) {

        return impl_->addServiceServer(server);
    }

    bool ServiceDiscovery::addServiceClient(const std::shared_ptr<IServiceClient> client) {
        // todo
        return impl_->addServiceClient(client);
    }

    void ServiceDiscovery::set_register_service_server_cb(DealRegisterServiceServerCb cb) {
        impl_->set_register_service_server_cb(cb);
    }

    void ServiceDiscovery::set_unregister_service_server_cb(DealRegisterServiceServerCb cb) {
        impl_->set_unregister_service_server_cb(cb);
    }

    void ServiceDiscovery::set_register_subscriber_cb(DealRegisterSubscriberCb cb) {
        impl_->set_register_subscriber_cb(cb);
    }

    void ServiceDiscovery::set_unregister_subscriber_cb(DealRegisterSubscriberCb cb) {
        impl_->set_unregister_subscriber_cb(cb);
    }
}
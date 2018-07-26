//
// Created by pairs on 7/12/18.
//

#pragma once

#include <memory>
#include <zros/service_server.h>
#include <zros/service_client.h>
#include <zros/publisher.h>

namespace zros {
    using DealRegisterServiceServerCb = std::function<void(const zros_rpc::ServiceServerInfo *serverInfo, zros_rpc::Status *status)>;
    using DealRegisterSubscriberCb = std::function<void(const zros_rpc::SubscriberInfo *subInfo, zros_rpc::Status *status)>;

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
        bool addPublisher(const std::shared_ptr<IPublisher> publisher);

        void set_register_service_server_cb(DealRegisterServiceServerCb cb);
        void set_unregister_service_server_cb(DealRegisterServiceServerCb cb);
        void set_register_subscriber_cb(DealRegisterSubscriberCb cb);
        void set_unregister_subscriber_cb(DealRegisterSubscriberCb cb);

    private:
        std::shared_ptr<ServiceDiscoveryImpl> impl_;
    };
}   // namespace zros

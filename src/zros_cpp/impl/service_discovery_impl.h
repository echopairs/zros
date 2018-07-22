//
// Created by pairs on 7/13/18.
//

#pragma once

#include "zros.grpc.pb.h"
#include <grpc++/grpc++.h>
#include <grpc++/create_channel.h>
#include <thread>
#include <zros/service_server_interface.h>
#include <zros/service_client_interface.h>


using grpc::Channel;
using grpc::Status;

namespace zros {

    class ServiceDiscoveryImpl : public zros_rpc::ServiceDiscoveryRPC::Service {
    public:
        ServiceDiscoveryImpl(const std::string & masterAddress, const std::string & agentAddress);
        bool isConnectedToMaster();

        // todo add pub/sub/client/server
        void addPublisher();
        void addSubscriber();
        bool addServiceServer(const std::shared_ptr<IServiceServer> server);
        bool addServiceClient(const std::shared_ptr<IServiceClient> client);


        // rpc server
        grpc::Status RegisterSubscriber(grpc::ServerContext * context, const zros_rpc::SubscriberInfo * request, zros_rpc::Status * response) override;
        grpc::Status UnregisterSubscriber(grpc::ServerContext * context, const zros_rpc::SubscriberInfo * request, zros_rpc::Status * response) override;
        grpc::Status RegisterServiceServer(grpc::ServerContext * context, const zros_rpc::ServiceServerInfo * request, zros_rpc::Status * response) override;
        grpc::Status UnregisterServiceServer(grpc::ServerContext * context, const zros_rpc::ServiceServerInfo * request, zros_rpc::Status * response) override;
        grpc::Status Ping(grpc::ServerContext * context, const zros_rpc::PingRequest * request, zros_rpc::Status *response) override ;

        void spin();

        ~ServiceDiscoveryImpl();

    private:
        std::unique_ptr<zros_rpc::MasterRPC::Stub> master_rpc_stub_;
        std::unique_ptr<grpc::Server> grpc_server_;
        std::string master_address_;
        std::string agent_address_;
    };
}
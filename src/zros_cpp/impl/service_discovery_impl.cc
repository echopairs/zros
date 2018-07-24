//
// Created by pairs on 7/13/18.
//

#include <sspdlog/sspdlog.h>
#include "service_discovery_impl.h"
#include <zros/node_handle.h>

namespace zros {
    grpc::Status
    ServiceDiscoveryImpl::RegisterServiceServer(grpc::ServerContext *context,
                                                             const zros_rpc::ServiceServerInfo *request,
                                                zros_rpc::Status *response) {
        SSPD_LOG_INFO << "receive register service server " << request->service_name();
        deal_register_service_server_cb_(request, response);
        return grpc::Status::OK;
    }

    grpc::Status
    ServiceDiscoveryImpl::RegisterSubscriber(grpc::ServerContext *context, const zros_rpc::SubscriberInfo *request,
                                             zros_rpc::Status *response) {
        deal_register_subscriber_cb_(request, response);
        return grpc::Status::OK;
    }

    grpc::Status
    ServiceDiscoveryImpl::UnregisterSubscriber(grpc::ServerContext *context, const zros_rpc::SubscriberInfo *request,
                                               zros_rpc::Status *response) {
        deal_unregister_subscriber_cb_(request, response);
        return grpc::Status::OK;
    }

    grpc::Status
    ServiceDiscoveryImpl::UnregisterServiceServer(grpc::ServerContext *context, const zros_rpc::ServiceServerInfo *request,
                                                  zros_rpc::Status *response) {
        deal_unregister_service_server_cb_(request, response);
        return grpc::Status::OK;
    }

    grpc::Status
    ServiceDiscoveryImpl::Ping(grpc::ServerContext *context, const zros_rpc::PingRequest *request, zros_rpc::Status *response) {
        return grpc::Status::OK;
    }

    ServiceDiscoveryImpl::ServiceDiscoveryImpl(const std::string &master_address, const std::string &agent_address)
            :master_address_(master_address),
             agent_address_(agent_address) {
        grpc::ChannelArguments args;
        args.SetInt(GRPC_ARG_MAX_RECONNECT_BACKOFF_MS, 5*1000);
        master_rpc_stub_ = zros_rpc::MasterRPC::NewStub(grpc::CreateCustomChannel(master_address, grpc::InsecureChannelCredentials(), args));
    }

    bool ServiceDiscoveryImpl::isConnectedToMaster() {
        grpc::ClientContext context;
        context.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(5000));
        zros_rpc::PingRequest request;
        zros_rpc::Status response;
        auto status = master_rpc_stub_->Ping(&context, request, &response);
        return status.ok() && response.code() == zros_rpc::Status::OK;
    }

    void ServiceDiscoveryImpl::spin() {
        if (spin_thread_) {
            SSPD_LOG_WARNING << "service discovery already spin";
            return;
        }
        spin_thread_ = std::make_shared<std::thread>([this]() {
            grpc::ServerBuilder builder;
            builder.AddListeningPort(this->agent_address_, grpc::InsecureServerCredentials());
            builder.RegisterService(this);
            grpc_server_ = std::move(builder.BuildAndStart());
            SSPD_LOG_INFO << "service discovery agent to work...";
            grpc_server_->Wait();
        });
    }

    ServiceDiscoveryImpl::~ServiceDiscoveryImpl() {
        grpc_server_->Shutdown();
        if (spin_thread_) {
            spin_thread_->join();
        }
    }

    bool ServiceDiscoveryImpl::addServiceServer(const std::shared_ptr<IServiceServer> server) {
        grpc::ClientContext context;
//        context.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(5000));
        zros_rpc::ServiceServerInfo request;
        zros_rpc::Status response;

        request.set_service_name(server->get_service_name());
        request.mutable_physical_node_info()->set_agent_address(agent_address_);
        request.mutable_physical_node_info()->set_real_address(server->get_node_handle()->get_node_address());
        request.mutable_physical_node_info()->set_name(server->get_node_handle()->get_node_name());
        auto status = master_rpc_stub_->RegisterServiceServer(&context, request, &response);
        return status.ok() && response.code() == zros_rpc::Status::OK;
    }

    bool ServiceDiscoveryImpl::addServiceClient(const std::shared_ptr<IServiceClient> client) {
        grpc::ClientContext context;
        context.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(5000));
        zros_rpc::ServiceClientInfo request;
        zros_rpc::Status response;
        request.set_service_name(client->get_service_name());
        request.mutable_physical_node_info()->set_agent_address(agent_address_);
        request.mutable_physical_node_info()->set_name(client->get_node_handle()->get_node_name());
        auto status = master_rpc_stub_->RegisterServiceClient(&context, request, &response);

        return status.ok() && response.code() == zros_rpc::Status::OK;
    }
} // namespace zros
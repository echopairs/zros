//
// Created by pairs on 7/18/18.
//
#include "service_clients_impl.h"
#include <sspdlog/sspdlog.h>

namespace zros {

    GrpcStub::GrpcStub(const std::string &address):
            RpcStub(address),
            node_stub_(std::make_shared<NodeStub >(createNodeStub(address))) {
    }

    std::shared_ptr<zros_rpc::ServiceResponse> GrpcStub::call(const std::string &service_name,
                                                              const std::string &content, const std::string &cli_info,
                                                              int timeout_mseconds) {
        auto service_response = std::make_shared<zros_rpc::ServiceResponse>();
        auto service_request = std::make_shared<zros_rpc::ServiceRequest>();
        service_request->set_service_name(service_name);
        service_request->set_request_data(content);
        service_request->mutable_cli_node_info()->set_address(cli_info);

        auto context = std::make_shared<grpc::ClientContext>();
        if (timeout_mseconds >= 0) {
            context->set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeout_mseconds));
        }
        auto status = (*node_stub_)->InvokeService(context.get(), *service_request, service_response.get());
        if (!status.ok()) {
            service_response->mutable_status()->set_code(zros_rpc::Status_Code_UNKNOWN);
            service_response->mutable_status()->set_details(std::string("GRPC INNER ERROR: ") + status.error_message());
        }
        return service_response;
    }

    NodeStub GrpcStub::createNodeStub(const std::string &address) {
        grpc::ChannelArguments args;
        args.SetInt(GRPC_ARG_MAX_RECONNECT_BACKOFF_MS, 5*1000);
        return zros_rpc::ServiceRPC::NewStub(
                grpc::CreateCustomChannel(address, grpc::InsecureChannelCredentials(), args)
        );
    }

    void ServiceClientsImpl::registerServiceClient(const zros_rpc::ServiceServerInfo *serverInfo, zros_rpc::Status *status) {
        std::string service_name = serverInfo->service_name();
        std::lock_guard<std::mutex> lk1(clients_mutex_);
        if (clients_.find(service_name) == clients_.end()) {
            SSPD_LOG_WARNING << "please check registerServiceClient logical error " << service_name;
            status->set_code(status->NOT_FOUND);
            status->set_details(service_name + " client not register on");
            return;
        }
        clients_[service_name]->set_ready(true);
        std::lock_guard<std::mutex> lk2(services_mutex_);
        services_address_[service_name] = serverInfo->physical_node_info().real_address();
        status->set_code(status->OK);
        SSPD_LOG_INFO << "register client : " << service_name << "on address: " << serverInfo->physical_node_info().real_address() << "success";
    }

    void ServiceClientsImpl::unregisterServiceClient(const zros_rpc::ServiceServerInfo *serverInfo,
                                                     zros_rpc::Status *status) {
        // todo
    }

    bool ServiceClientsImpl::unregisterServiceClient(const std::string &service_name) {
        // todo
        return false;
    }

    std::shared_ptr<zros_rpc::ServiceResponse>
    ServiceClientsImpl::call(const std::string &service_name, const std::string &content, const std::string &cli_info,
                             int timeout_mseconds) {
        std::lock_guard<std::mutex> lk (clients_mutex_);
        if (clients_.find(service_name) != clients_.end()) {
            SSPD_LOG_ERROR << "please check  if register this client ";
            return nullptr;
        }
        // already ready
        auto client = clients_[service_name];
        auto stub = get_rpc_stub(service_name);
        if (stub == nullptr) {
            SSPD_LOG_WARNING << "stub is nullptr ";
            auto tmp_res = std::make_shared<zros_rpc::ServiceResponse>();
            tmp_res->mutable_status()->set_code(zros_rpc::Status_Code_NOT_FOUND);
            tmp_res->mutable_status()->set_details("service call stub not found, probably waiting for establishing yet.");
            return tmp_res;
        }
        return stub->call(service_name, content, cli_info, timeout_mseconds);
    }

    bool ServiceClientsImpl::registerClient(const std::shared_ptr<IServiceClient> client) {
        std::lock_guard<std::mutex> lk(clients_mutex_);
        if (clients_.find(client->get_service_name()) != clients_.end()) {
            SSPD_LOG_WARNING << client->get_service_name() << " already register";
            return false;
        }
        return true;
    }

    std::shared_ptr<RpcStub> ServiceClientsImpl::get_rpc_stub(const std::string &service_name) {
        std::lock_guard<std::mutex> lk1(services_mutex_);
        std::lock_guard<std::mutex> lk2(stubs_mutex_);
        if(services_address_.find(service_name) == services_address_.end()) {
            SSPD_LOG_ERROR << "get rpc stub failed: there is no this " << service_name << "service client";
            return nullptr;
        }
        std::string address = services_address_[service_name];
        auto stub = stubs_.find(address);
        if (stub == stubs_.end()) {
            // first use
            stubs_[address] = std::make_shared<GrpcStub>(address);
        }
        return stubs_[address];
    }

}   // namespace zros
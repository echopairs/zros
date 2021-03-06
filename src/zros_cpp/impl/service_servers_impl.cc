//
// Created by pairs on 7/16/18.
//

#include "service_servers_impl.h"
#include <sspdlog/sspdlog.h>
#include <zros/zros.h>
#include <zros/error.h>

namespace zros {

    //////////// IServersImpl ///////////////////
    bool IServersImpl::registerServer(const std::shared_ptr<IServiceServer> server) {
        std::lock_guard<std::mutex> lk (servers_mutex_);
        if (servers_.find(server->get_service_name()) != servers_.end()) {
            SSPD_LOG_WARNING << server->get_service_name() << " already register";
        }
        servers_[server->get_service_name()] = server;
        return true;
    }

    bool IServersImpl::unregisterServer(const std::string &service_name) {
        std::lock_guard<std::mutex> lk (servers_mutex_);
        servers_.erase(service_name);
        return true;
    }

    IServersImpl::IServersImpl(const std::string &address):service_address_(address) {

    }


    //////////// GrpcServersImpl ///////////////////
    const std::string& GrpcServersImpl::start() {
        if (!is_working_) {
            SSPD_LOG_INFO << "grpc server to work...";
            grpc::ServerBuilder builder;
            int select_port;
            builder.AddListeningPort(service_address_, grpc::InsecureServerCredentials(), &select_port);
            builder.RegisterService(this);
            grpc_server_ = std::move(builder.BuildAndStart());
            if (select_port == 0) {
                throw initialize_error("grpc server bind port failed");
            }
            if (service_address_ == "[::]:") {
                service_address_ = "localhost:" + std::to_string(select_port);
            }
            work_thread_ = std::make_shared<std::thread>([this]() {
                grpc_server_->Wait();
            });
            is_working_ = true;
        } else {
            SSPD_LOG_WARNING << "grpc server already work...";
        }
        return service_address_;
    }

    void GrpcServersImpl::stop() {
        grpc_server_->Shutdown();
        if (work_thread_) {
            work_thread_->join();
        }
    }

    GrpcServersImpl::GrpcServersImpl(const std::string &service_address):
            IServersImpl(service_address),
            is_working_(false){

    }

    grpc::Status GrpcServersImpl::InvokeService(grpc::ServerContext *context, const zros_rpc::ServiceRequest *request,
                                                zros_rpc::ServiceResponse *response) {
        SSPD_LOG_INFO << "receive rpc call " << request->service_name();
        auto existing_server = servers_.find(request->service_name());
        if (existing_server == servers_.end()) {
            response->mutable_status()->set_code(zros_rpc::Status_Code_FAILED_PRECONDITION);
            response->mutable_status()->set_details("There is no server for this service");
        } else {
            existing_server->second->invoke(request, response);
        }
        return grpc::Status::OK;
    }

    GrpcServersImpl::~GrpcServersImpl() {
        stop();
    }

}
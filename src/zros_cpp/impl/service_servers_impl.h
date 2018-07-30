//
// Created by pairs on 7/16/18.
//

#pragma once

#include <memory>
#include <zros/service_server_interface.h>
#include <grpc++/grpc++.h>
#include <thread>
#include <atomic>

namespace zros {

    class IServersImpl {
    public:
        IServersImpl(const std::string & address);
        virtual const std::string& start() = 0;
        virtual void stop() = 0;
        bool registerServer(const std::shared_ptr<IServiceServer> server);
        bool unregisterServer(const std::string & service_name);
    protected:
        std::string service_address_;
        std::mutex servers_mutex_;
        std::map<std::string, std::shared_ptr<IServiceServer>> servers_;
    };

    class GrpcServersImpl : public IServersImpl, public zros_rpc::ServiceRPC::Service {
    public:
        GrpcServersImpl(const std::string &service_address);
        const std::string& start() override;
        void stop() override;
        grpc::Status InvokeService(grpc::ServerContext * context, const zros_rpc::ServiceRequest * request, zros_rpc::ServiceResponse * response) override;
        ~GrpcServersImpl();
    private:
        std::atomic<bool> is_working_;
        std::shared_ptr<std::thread> work_thread_;
        std::unique_ptr<grpc::Server> grpc_server_;
    };
}

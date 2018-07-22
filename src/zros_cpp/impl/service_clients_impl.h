//
// Created by pairs on 7/18/18.
//

#pragma once

#include <grpc++/grpc++.h>
#include <zros.grpc.pb.h>
#include <zros/service_client_interface.h>

namespace zros {

    using NodeStub = std::unique_ptr<zros_rpc::ServiceRPC::Stub>;

    class RpcStub {
    public:
        RpcStub(const std::string & address): address_(address) {
        }

        virtual std::shared_ptr<zros_rpc::ServiceResponse> call(const std::string & service_name, const std::string & content,
                                                                const std::string &cli_info, int timeout_mseconds) = 0;
    protected:
        std::string address_;
    };

    class GrpcStub : public RpcStub {
    public:
        GrpcStub(const std::string & address);
        virtual std::shared_ptr<zros_rpc::ServiceResponse> call(const std::string &service_name, const std::string & content,
                                                                       const std::string &cli_info, int timeout_mseconds) override;
    protected:
        NodeStub createNodeStub(const std::string & address);
        std::shared_ptr<NodeStub> node_stub_;
    };

    class ServiceClientsImpl {
    public:
        bool registerClient(const std::shared_ptr<IServiceClient> client);
        bool registerServiceClient(const std::string & service_name, const std::string & address);
        bool unregisterServiceClient(const std::string & service_name);
        std::shared_ptr<zros_rpc::ServiceResponse> call(const std::string &service_name, const std::string & content,
                                                        const std::string &cli_info, int timeout_mseconds);
    private:
        std::shared_ptr<RpcStub> get_rpc_stub(const std::string & service_name);

        std::mutex clients_mutex_;
        std::map<std::string, std::shared_ptr<IServiceClient> > clients_;

        std::mutex services_mutex_;
        std::map<std::string, std::string> services_address_;   // key: service_name, value: address

        // RpcStub can reusable, and thread safety so can just use one stub to the specified node
        // for grpc https//github.com/grpc/grpc/issues/5649
        std::mutex stubs_mutex_;
        std::map<std::string, std::shared_ptr<RpcStub>> stubs_; // key: address, value: rpc stubs
    };
}

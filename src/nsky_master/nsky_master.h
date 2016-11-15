// impl grpc interface
#include <thread>
#include <memory>
#include <string>
#include <thread>
#include <chrono>

#include <grpc++/grpc++.h>
#include "nsky_rpc.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

namespace nsky {
	class MasterServiceImpl final : public nsky_rpc::MasterRPC::Service
	{
	public:
		MasterServiceImpl(std::string &address);
		grpc::Status RegisterServer(::grpc::ServerContext* context, const ::nsky_rpc::ServerInfo* request, ::nsky_rpc::Status* response) override;
		grpc::Status UnregisterServer(::grpc::ServerContext* context, const ::nsky_rpc::ServerInfo* request, ::nsky_rpc::Status* response) override;
		grpc::Status RegisterClient(::grpc::ServerContext* context, const ::nsky_rpc::ClientInfo* request, ::nsky_rpc::Status* response) override;
		grpc::Status UnregisterClient(::grpc::ServerContext* context, const ::nsky_rpc::ClientInfo* request, ::nsky_rpc::Status* response) override;
		grpc::Status Ping(::grpc::ServerContext* context, const ::nsky_rpc::PingRequest* request, ::nsky_rpc::Status* response) override;
		void RunServer();
	private:
		std::string server_address_;
		grpc::ServerBuilder builder_;
		std::unique_ptr<grpc::Server> server_;

	};
}

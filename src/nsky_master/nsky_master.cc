#include "master.h"


namespace nsky {

	MasterServiceImpl::MasterServiceImpl(const std::string &address):server_address_(address)
	{
	}
	grpc::Status MasterServiceImpl::RegisterServer(::grpc::ServerContext* context, const ::nsky_rpc::ServerInfo* request, ::nsky_rpc::Status* response)
	{
		// TODO
     return grpc::Status::OK;
	}
	grpc::Status MasterServiceImpl::UnregisterServer(::grpc::ServerContext* context, const ::nsky_rpc::ServerInfo* request, ::nsky_rpc::Status* response)
	{
		// TODO
     return grpc::Status::OK;
	}
	grpc::Status MasterServiceImpl::RegisterClient(::grpc::ServerContext* context, const ::nsky_rpc::ClientInfo* request, ::nsky_rpc::Status* response)
	{
		// TODO
     return grpc::Status::OK;
	}
	grpc::Status MasterServiceImpl::UnregisterClient(::grpc::ServerContext* context, const ::nsky_rpc::ClientInfo* request, ::nsky_rpc::Status* response)
	{
		// TODO
     return grpc::Status::OK;
	}
	grpc::Status MasterServiceImpl::Ping(::grpc::ServerContext* context, const ::nsky_rpc::PingRequest* request, ::nsky_rpc::Status* response)
	{
		// TODO
     return grpc::Status::OK;
	}
	void MasterServiceImpl::RunServer()
	{
      builder_.AddListeningPort(server_address_, grpc::InsecureServerCredentials());
      builder_.RegisterService(this);
      server_ = std::move(builder_.BuildAndStart());
      server_->Wait();
	}

} // endof nsky

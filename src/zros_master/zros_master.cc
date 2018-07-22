#include <sspdlog/sspdlog.h>
#include "zros_master.h"


namespace zros {

	MasterServiceImpl::MasterServiceImpl(const std::string &address):server_address_(address) {
	}

	grpc::Status MasterServiceImpl::RegisterServiceServer(::grpc::ServerContext* context, const ::zros_rpc::ServiceServerInfo* request, ::zros_rpc::Status* response) {
		// TODO
		SSPD_LOG_INFO << "RegisterServiceServer ";
		response->set_code(response->OK);
		return grpc::Status::OK;
	}

	grpc::Status MasterServiceImpl::UnregisterServiceServer(::grpc::ServerContext* context, const ::zros_rpc::ServiceServerInfo* request, ::zros_rpc::Status* response) {
		// TODO
		return grpc::Status::OK;
	}

	grpc::Status MasterServiceImpl::RegisterServiceClient(::grpc::ServerContext* context, const ::zros_rpc::ServiceClientInfo* request, ::zros_rpc::Status* response) {
		// TODO
		SSPD_LOG_INFO << "RegisterServiceClient";
		response->set_code(response->OK);
     	return grpc::Status::OK;
	}

	grpc::Status MasterServiceImpl::UnregisterServiceClient(::grpc::ServerContext* context, const ::zros_rpc::ServiceClientInfo* request, ::zros_rpc::Status* response) {
		// TODO
     	return grpc::Status::OK;
	}

	grpc::Status MasterServiceImpl::Ping(::grpc::ServerContext* context, const ::zros_rpc::PingRequest* request, ::zros_rpc::Status* response) {
		SSPD_LOG_INFO << "master ping";
		response->set_code(response->OK);
     	return grpc::Status::OK;
	}

	void MasterServiceImpl::RunServer() {
      builder_.AddListeningPort(server_address_, grpc::InsecureServerCredentials());
      builder_.RegisterService(this);
      server_ = std::move(builder_.BuildAndStart());
      SSPD_LOG_INFO << "Master run on " << server_address_;
      server_->Wait();
	}

	MasterServiceImpl::~MasterServiceImpl() {
		SSPD_LOG_INFO << "Master End";
	}

} // namespace nsky

#include <sspdlog/sspdlog.h>
#include "zros_master.h"
#include "connect_task.h"

namespace zros {

	MasterServiceImpl::MasterServiceImpl(const std::string &address):
            server_address_(address),
            thread_pool_(4),
            health_check_(false) {
		nodeManager_ = std::make_shared<NodeManager>();
		serviceManager_ = std::make_shared<ServiceManager>();

	}

	grpc::Status MasterServiceImpl::RegisterServiceServer(::grpc::ServerContext* context, const ::zros_rpc::ServiceServerInfo* serverInfo, ::zros_rpc::Status* status) {
        SSPD_LOG_INFO << "RegisterServiceServer ";
	    nodeManager_->addNode(serverInfo->physical_node_info().agent_address());
	    auto clientList = serviceManager_->addServer(*serverInfo);
	    for (auto client : clientList) {
	        auto task = std::make_shared<ServiceConnectTask>(ServiceConnectTask::taskType::connect,
                                                             *serverInfo, client, nodeManager_, serviceManager_);
	        thread_pool_.enqueue([task, this](){
	            // todo
	            auto status = task->performTask();
	        });
	    }
        status->set_code(status->OK);
		return grpc::Status::OK;
	}

	grpc::Status MasterServiceImpl::UnregisterServiceServer(::grpc::ServerContext* context, const ::zros_rpc::ServiceServerInfo* serverInfo, ::zros_rpc::Status* status) {
		// TODO
		return grpc::Status::OK;
	}

	grpc::Status MasterServiceImpl::RegisterServiceClient(::grpc::ServerContext* context, const ::zros_rpc::ServiceClientInfo* clientInfo, ::zros_rpc::Status* status) {
		SSPD_LOG_INFO << "RegisterServiceClient";
		nodeManager_->addNode(clientInfo->physical_node_info().agent_address());
		auto server = serviceManager_->addClient(*clientInfo);
		if (server.has_physical_node_info() && !server.physical_node_info().agent_address().empty()) {
			auto task = std::make_shared<ServiceConnectTask>(ServiceConnectTask::taskType::connect,
															 server, *clientInfo, nodeManager_, serviceManager_);
			thread_pool_.enqueue([task, this](){
				auto status = task->performTask();
				if (status.flag_ == status.Error) {
				    SSPD_LOG_WARNING << "registerServiceClient task failed, the reason is ";
				    for (auto s : status.details_) {
				        SSPD_LOG_WARNING << s;
				    }
				}
			});
		}
        status->set_code(status->OK);
     	return grpc::Status::OK;
	}

	grpc::Status MasterServiceImpl::UnregisterServiceClient(::grpc::ServerContext* context, const ::zros_rpc::ServiceClientInfo* clientInfo, ::zros_rpc::Status* status) {
		// TODO
     	return grpc::Status::OK;
	}

	grpc::Status MasterServiceImpl::Ping(::grpc::ServerContext* context, const ::zros_rpc::PingRequest* pingRequest, ::zros_rpc::Status* status) {
        status->set_code(status->OK);
     	return grpc::Status::OK;
	}

	void MasterServiceImpl::runServer() {
	    health_check_thread_ = std::make_shared<std::thread>([this](){
            health_check_ = true;
			while(health_check_) {
				nodeManager_->healthCheck();
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
	    });

        builder_.AddListeningPort(server_address_, grpc::InsecureServerCredentials());
        builder_.RegisterService(this);
        server_ = std::move(builder_.BuildAndStart());
        SSPD_LOG_INFO << "master run on " << server_address_;
        server_->Wait();
	}

	MasterServiceImpl::~MasterServiceImpl() {
	    health_check_ = false;
	    if (health_check_thread_) {
	        health_check_thread_->join();
	    }
		SSPD_LOG_INFO << "master End";
	}

} // namespace zros

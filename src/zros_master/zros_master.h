/*********************************************************************
  Copyright (C), 1990-2016, HangZhou RED HEAT Tech. Co., Ltd.
  FileName: nsky_master.h
  Author  : pairs & 736418319@qq.com
  Version : 1.0
  Date    : 2016/11/11
  Description:
      inheritance grpc interface
  Function List:
		1. MasterServiceImpl(const std::string &address)   // e.g: address 127.0.0.1:40000
		2. grpc::Status RegisterServer(::grpc::ServerContext* context, const ::zros_rpc::ServerInfo* request, ::zros_rpc::Status* response)
			Des: server register to master tell master it can dealwith the  request
			param1:
			1. grpc::ServerContext* context  [IN]						// out-of-band data not use now
			2. const ::zros_rpc::ServerInfo* request [IN]		// request data
			3. zros_rpc::Status* response	[OUT]							// response data
		......
		3. runServer // just listening in the port and accept request
  History:
      <author>    <time>    <version>    <desc>
        pairs     16/11/12      1.0     build this moudle
				pairs     16/11/15			1.1			add grpc interface
*********************************************************************/
#pragma once

#include <thread>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <zros/thread_pool.h>
#include <grpc++/grpc++.h>
#include "zros.grpc.pb.h"
#include "service_manager.h"
#include "node_manager.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

namespace zros {
	class MasterServiceImpl final : public zros_rpc::MasterRPC::Service
	{
	public:
		MasterServiceImpl(const std::string &address);
		grpc::Status RegisterServiceServer(::grpc::ServerContext* context, const ::zros_rpc::ServiceServerInfo* request, ::zros_rpc::Status* response) override;
		grpc::Status UnregisterServiceServer(::grpc::ServerContext* context, const ::zros_rpc::ServiceServerInfo* request, ::zros_rpc::Status* response) override;
		grpc::Status RegisterServiceClient(::grpc::ServerContext* context, const ::zros_rpc::ServiceClientInfo* request, ::zros_rpc::Status* response) override;
		grpc::Status UnregisterServiceClient(::grpc::ServerContext* context, const ::zros_rpc::ServiceClientInfo* request, ::zros_rpc::Status* response) override;
		grpc::Status Ping(::grpc::ServerContext* context, const ::zros_rpc::PingRequest* request, ::zros_rpc::Status* response) override;
		void runServer();
		virtual ~MasterServiceImpl();
	private:
        std::atomic<bool> health_check_;
        std::shared_ptr<std::thread> health_check_thread_;

		std::string server_address_;
		grpc::ServerBuilder builder_;
		std::unique_ptr<grpc::Server> server_;

		std::shared_ptr<NodeManager> nodeManager_;
		std::shared_ptr<ServiceManager> serviceManager_;

		ThreadPool thread_pool_;
	};
}


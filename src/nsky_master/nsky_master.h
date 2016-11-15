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
		2. grpc::Status RegisterServer(::grpc::ServerContext* context, const ::nsky_rpc::ServerInfo* request, ::nsky_rpc::Status* response)
			Des: server register to master tell master it can dealwith the  request
			param1:
			1. grpc::ServerContext* context  [IN]						// out-of-band data not use now
			2. const ::nsky_rpc::ServerInfo* request [IN]		// request data
			3. nsky_rpc::Status* response	[OUT]							// response data
		......
		3. RunServer // just listening in the port and accept request
  History:
      <author>    <time>    <version>    <desc>
        pairs     16/11/12      1.0     build this moudle
				pairs     16/11/15			1.1			add grpc interface
*********************************************************************/
#ifndef __NSKY_MASTER_H__
#define __NSKY_MASTER_H__

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
		MasterServiceImpl(const std::string &address);
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
#endif

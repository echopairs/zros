#pragma once

#include <map>
#include <tuple>
#include <mutex>
#include <thread>
#include <grpc++/grpc++.h>
#include "zros.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


namespace zros {

  class NodeManager {
  public:
    NodeManager();
    ~NodeManager();

    void addNode(const std::string& address);
    void removeNode(const std::string& address);
    std::shared_ptr<std::tuple<std::chrono::system_clock::time_point,
            std::shared_ptr<grpc::Channel>, std::shared_ptr<zros_rpc::ServiceDiscoveryRPC::Stub> > > \
    getNode(const std::string& address);

    std::shared_ptr<zros_rpc::ServiceDiscoveryRPC::Stub> getNodeStub(const std::string& address);

    std::shared_ptr<grpc::Channel> getNodeChannel(const std::string& address);
    std::chrono::system_clock::time_point getNodeLastModifyTime(const std::string& address);
    void removeAllNode();
    void healthCheck();
  private:
    std::mutex mtx_;
    std::map<std::string, std::shared_ptr<std::tuple<std::chrono::system_clock::time_point, std::shared_ptr<grpc::Channel>, std::shared_ptr<zros_rpc::ServiceDiscoveryRPC::Stub>> > > nodes_;
  };

} // namespace zros


#include "node_manager.h"
#include <chrono>
#include <ratio>


namespace zros {

    NodeManager::NodeManager() {
        nodes_.clear();
    }

    NodeManager::~NodeManager() {
      std::unique_lock<std::mutex> lk(mtx_);
      removeAllNode();
    }

    void NodeManager::addNode(const std::string& address) {
      std::unique_lock<std::mutex> lk(mtx_);
      if (nodes_.find(address) == nodes_.end()) {
          auto channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
          auto stub = zros_rpc::ServiceDiscoveryRPC::NewStub(channel);
          nodes_[address] = std::make_shared<std::tuple<std::chrono::system_clock::time_point, std::shared_ptr<grpc::Channel>,\
           std::shared_ptr<zros_rpc::ServiceDiscoveryRPC::Stub> > >(std::chrono::system_clock::now(),channel, move(stub));
      }
    }

    void NodeManager::removeNode(const std::string &addr) {
        std::unique_lock<std::mutex> lk(mtx_);
        nodes_.erase(addr);
    }


    std::shared_ptr<std::tuple<std::chrono::system_clock::time_point, std::shared_ptr<grpc::Channel>, std::shared_ptr<zros_rpc::ServiceDiscoveryRPC::Stub> > > \
    NodeManager::getNode(const std::string& address) {
        std::unique_lock<std::mutex> lk(mtx_);
        auto it = nodes_.find(address);
        if (it == nodes_.end()) {
            return nullptr;
        }
        return it->second;
    }

    std::shared_ptr<zros_rpc::ServiceDiscoveryRPC::Stub> NodeManager::getNodeStub(const std::string& address) {
        auto it = getNode(address);
        if (it == nullptr) {
          return nullptr;
        }
        return std::get<2>(*it);
    }

    std::shared_ptr<grpc::Channel> NodeManager::getNodeChannel(const std::string& address) {
        auto it = getNode(address);
        if (it == nullptr) {
            return nullptr;
        }
        return std::get<1>(*it);
    }

    std::chrono::system_clock::time_point NodeManager::getNodeLastModifyTime(const std::string& address) {
        auto it = getNode(address);
        if (it != nullptr) {
            return std::get<0>(*it);
        }
    }

    void NodeManager::removeAllNode() {
        std::unique_lock<std::mutex> lk(mtx_);
        nodes_.clear();
    }

    void NodeManager::healthCheck() {
      // TODO
    }
}  // end of namespace nsky

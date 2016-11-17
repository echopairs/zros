#include "node_mgr.h"
#include <chrono>
#include <ratio>

// private:
//   // <ip,<last Modify time, channel, stub >>
//   std::mutex mtx_;
//   std::map<std::string, std::shared_ptr<tuple<int, std::shared_ptr<grpc::Channel>, std::shared_ptr<nsky::PhysicalNodeRPC::Stub>> > > nodes_;
// };

namespace nsky {

NodeManger::NodeManger()
{
    nodes_.clear();
}
NodeManger::~NodeManger()
{
  std::unique_lock<std::mutex> lk(mtx_);
  RemoveAllNode();
}

void NodeManger::AddNode(const std::string &addr)
{
  std::unique_lock<std::mutex> lk(mtx_);
  if (nodes_.find(addr) == nodes_.end())
  {
      auto channel = grpc::CreateChannel(addr, grpc::InsecureChannelCredentials());
      auto stub = nsky_rpc::PhysicalNodeRPC::NewStub(channel);
      nodes_[addr] = std::make_shared<std::tuple<std::chrono::system_clock::time_point, std::shared_ptr<grpc::Channel>,\
       std::shared_ptr<nsky_rpc::PhysicalNodeRPC::Stub> > >(std::chrono::system_clock::now(),channel, move(stub));
  }
}

void NodeManger::RemoveNode(const std::string &addr)
{
  std::unique_lock<std::mutex> lk(mtx_);
  nodes_.erase(addr);
}

std::shared_ptr<std::tuple<std::chrono::system_clock::time_point, std::shared_ptr<grpc::Channel>, std::shared_ptr<nsky_rpc::PhysicalNodeRPC::Stub> > > \
NodeManger::GetNode(const std::string &addr)
{
  std::unique_lock<std::mutex> lk(mtx_);
  auto it = nodes_.find(addr);
  if (it == nodes_.end())
  {
      return nullptr;
  }
  return it->second;
}

std::shared_ptr<nsky_rpc::PhysicalNodeRPC::Stub> NodeManger::GetNodeStub(std::string &addr)
{
  auto it = GetNode(addr);
  if (it == nullptr)
  {
      return nullptr;
  }
  return std::get<2>(*it);
}
std::shared_ptr<grpc::Channel> NodeManger::GetNodeChannel(std::string &addr)
{
  auto it = GetNode(addr);
  if (it == nullptr)
  {
      return nullptr;
  }
  return std::get<1>(*it);
}
std::chrono::system_clock::time_point NodeManger::GetNodeLastModifyTime(std::string &addr)
{
    auto it = GetNode(addr);;
    if (it != nullptr)
    {
        return std::get<0>(*it);
    }
}
void NodeManger::RemoveAllNode()
{
  std::unique_lock<std::mutex> lk(mtx_);
  nodes_.clear();
}
void NodeManger::HealthCheck()
{
  // TODO
}

}  // end of namespace nsky

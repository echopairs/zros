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
}

void NodeManger::RemoveNode(const std::string &addr)
{
  std::unique_lock<std::mutex> lk(mtx_);
  nodes_.erase(addr);
}

std::shared_ptr<nsky_rpc::PhysicalNodeRPC::Stub> NodeManger::GetNodeStub(std::string &addr)
{
  // TODO
  std::unique_lock<std::mutex> lk(mtx_);
}
std::shared_ptr<grpc::Channel> NodeManger::GetNodeChannel(std::string &addr)
{
  //TODO
  std::unique_lock<std::mutex> lk(mtx_);
}
int NodeManger::GetNodeLastModifyTime(std::string &addr)
{
  // TODO
  std::unique_lock<std::mutex> lk(mtx_);
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

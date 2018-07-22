/*********************************************************************
  Copyright (C), 1990-2016, HangZhou RED HEAT Tech. Co., Ltd.
  FileName: node_mgr.h
  Author  : pairs & 736418319@qq.com
  Version : 1.0
  Date    : 2016/11/17
  Description:
      manage the other pysical node whose register to master node
  Function List:
  History:
      <author>    <time>    <version>    <desc>
        pairs     16/11/17      1.0     build this moudle
*********************************************************************/

#ifndef __NSKY_NODE_MGR_H__
#define __NSKY_NODE_MGR_H__
#include <map>
#include <tuple>
#include <mutex>
#include <thread>
#include <grpc++/grpc++.h>
#include "nsky_rpc.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


namespace nsky {

class NodeManger
{
public:
  NodeManger();
  ~NodeManger();
  void AddNode(const std::string &addr);
  void RemoveNode(const std::string &addr);
  std::shared_ptr<std::tuple<std::chrono::system_clock::time_point, std::shared_ptr<grpc::Channel>, std::shared_ptr<nsky_rpc::PhysicalNodeRPC::Stub> > > \
  GetNode(const std::string &addr);

  std::shared_ptr<nsky_rpc::PhysicalNodeRPC::Stub> GetNodeStub(std::string &addr);
  std::shared_ptr<grpc::Channel> GetNodeChannel(std::string &addr);
  std::chrono::system_clock::time_point GetNodeLastModifyTime(std::string &addr);
  void RemoveAllNode();
  void HealthCheck();

private:
  // <addr,<last Modify time, channel, stub >>
  std::mutex mtx_;
  std::map<std::string, std::shared_ptr<std::tuple<std::chrono::system_clock::time_point, std::shared_ptr<grpc::Channel>, std::shared_ptr<nsky_rpc::PhysicalNodeRPC::Stub>> > > nodes_;
};

} // end of namespace nsky

#endif

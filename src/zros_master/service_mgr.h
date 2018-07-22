/*********************************************************************
  Copyright (C), 1990-2016, HangZhou RED HEAT Tech. Co., Ltd.
  FileName: service_mgr.h
  Author  : pairs & 736418319@qq.com
  Version : 1.0
  Date    : 2016/11/18
  Description:
      manage the services mapping service/client
  Function List:
  History:
      <author>    <time>    <version>    <desc>
        pairs     16/11/18      1.0     build this moudle
*********************************************************************/

#pragma once
#include <map>
#include <list>
#include <mutex>
#include <nsky_rpc.grpc.pb.h>
#include <nsky_rpc.pb.h>

namespace nsky {

class ServiceManager
{
public:
  ServiceManager();
  ~ServiceManager();
  std::list<nsky_rpc::ServiceClientInfo> AddService(const nsky_rpc::ServiceServerInfo &);
  std::list<nsky_rpc::ServiceServerInfo> AddClient(const nsky_rpc::ServiceClientInfo &);
  void RemoveService(const nsky_rpc::ServiceServerInfo &);
  void RemoveClient(const nsky_rpc::ServiceClientInfo &);

  bool IsHasClient(const nsky_rpc::ServiceClientInfo&);
  bool IsHasService(const nsky_rpc::ServiceServerInfo&);

  void RemoveAllService();
  void RemoveAllClient();

private:
    // <topic,<ip,phsicalNode> >
    std::mutex smtx_;
    std::mutex cmtx_;
    std::map<std::string, std::map<std::string, nsky_rpc::ServiceServerInfo> > services_;
    std::map<std::string, std::map<std::string, nsky_rpc::ServiceClientInfo> > clients_;
};
}


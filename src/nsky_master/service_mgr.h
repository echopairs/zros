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

#ifndef __NSKY_SERVICE_MGR_H__
#define __NSKY_SERVICE_MGR_H__
#include <map>
#include <list>
#include <mutex>
#include <nsky_rpc.grpc.pb.h>

namespace nsky {

class ServiceManager
{
public:
  ServiceManager();
  ~ServiceManager();
  std::list<nsky_rpc::ClientInfo> AddService(const nsky_rpc::ServerInfo &);
  std::list<nsky_rpc::ServerInfo> AddClient(const nsky_rpc::ClientInfo &);
  void RemoveService(const nsky_rpc::ServerInfo &);
  void RemoveClient(const nsky_rpc::ClientInfo &);

  bool IsHasClient(const nsky_rpc::ClientInfo&);
  bool IsHasService(const nsky_rpc::ServerInfo&);

  void RemoveAllService();
  void RemoveAllClient();

private:
    // <topic,<ip,phsicalNode> >
    std::mutex smtx_;
    std::mutex cmtx_;
    std::map<std::string, std::map<std::string, nsky_rpc::ServerInfo> > services_;
    std::map<std::string, std::map<std::string, nsky_rpc::ClientInfo> > clients_;
};
}

#endif

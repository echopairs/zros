#pragma once
#include <map>
#include <list>
#include <mutex>
#include "zros.grpc.pb.h"

namespace zros {

    class ServiceManager {
    public:
      ServiceManager();
      ~ServiceManager();

      std::list<zros_rpc::ServiceClientInfo> addServer(const zros_rpc::ServiceServerInfo &);
      zros_rpc::ServiceServerInfo addClient(const zros_rpc::ServiceClientInfo &);

      void removeServer(const zros_rpc::ServiceServerInfo &);
      void removeClient(const zros_rpc::ServiceClientInfo &);

      bool isHasClient(const zros_rpc::ServiceClientInfo&);
      bool isHasServer(const zros_rpc::ServiceServerInfo&);

      void removeAllServer();
      void removeAllClient();

    private:
        std::mutex smtx_;
        std::mutex cmtx_;
        std::map<std::string, zros_rpc::ServiceServerInfo> servers_;   // key: service_name, value: serverInfo
        std::map<std::string, std::map<std::string, zros_rpc::ServiceClientInfo> > clients_;    // key: service_name, value: (key: agent_address, value: clientInfo)
    };
}


#include "service_mgr.h"

namespace nsky
{
    ServiceManager::ServiceManager()
    {
      // TODO
    }
    ServiceManager::~ServiceManager()
    {
      // TODO
    }
    std::list<nsky_rpc::ClientInfo> ServiceManager::AddService(const nsky_rpc::ServerInfo &sinfo)
    {
      // TODO
    }

    std::list<nsky_rpc::ServerInfo> ServiceManager::AddClient(const nsky_rpc::ClientInfo &cinfo)
    {
      // TODO
    }

    void ServiceManager::RemoveService(const nsky_rpc::ServerInfo &sinfo)
    {
      // TODO
    }

    void ServiceManager::RemoveClient(const nsky_rpc::ClientInfo &cinfo)
    {
        // TODO
    }
    bool ServiceManager::IsHasClient(const nsky_rpc::ClientInfo& cinfo)
    {
        // TODO
    }
    bool ServiceManager::IsHasService(const nsky_rpc::ServerInfo)
    {
        // TODO
    }
} // end of namespace nsky

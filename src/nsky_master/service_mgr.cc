#include "service_mgr.h"

namespace nsky
{
    ServiceManager::ServiceManager()
    {
        services_.clear();
        clients_.clear();
    }
    ServiceManager::~ServiceManager()
    {
        RemoveAllService();
        RemoveAllClient();
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
        std::unique_lock<std::mutex> lk(smtx_);

    }

    void ServiceManager::RemoveClient(const nsky_rpc::ClientInfo &cinfo)
    {
        if (IsHasClient(cinfo))
        {
            // TODO
        }
    }
    bool ServiceManager::IsHasClient(const nsky_rpc::ClientInfo &cinfo)
    {
        std::unique_lock<std::mutex> lk(cmtx_);
        auto cmap = clients_.find(cinfo.service_name());
        if (cmap != clients_.end())
        {
            auto c = cmap->second.find(cinfo.physical_node_info().address());
            if (c != cmap->second.end())
            {
              return true;
            }
        }
        return false;
    }

    bool ServiceManager::IsHasService(const nsky_rpc::ServerInfo &sinfo)
    {
        std::unique_lock<std::mutex> lk(smtx_);
        auto smap = services_.find(sinfo.service_name());
        if (smap != services_.end())
        {
            auto s = smap->second.find(sinfo.physical_node_info().address());
            if (s != smap->second.end())
            {
              return true;
            }
        }
        return false;
    }

    void ServiceManager::RemoveAllService()
    {
        std::unique_lock<std::mutex> lk(smtx_);
        services_.clear();
    }
    void ServiceManager::RemoveAllClient()
    {
        std::unique_lock<std::mutex> lk(cmtx_);
        clients_.clear();
    }
} // end of namespace nsky

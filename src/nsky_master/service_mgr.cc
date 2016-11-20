#include "service_mgr.h"
#include <algorithm>

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
        std::list<nsky_rpc::ClientInfo> clts;
        std::unique_lock<std::mutex> slk(smtx_);
        std::unique_lock<std::mutex> clk(cmtx_);
        auto smap = services_.find(sinfo.service_name());

        // add to services_
        if (smap != services_.end())
        {
            smap->second[sinfo.physical_node_info().address()] = sinfo;
        }
        else
        {
            // no this topic
            std::map<std::string,nsky_rpc::ServerInfo> maps;
            maps[sinfo.physical_node_info().address()] = sinfo;
            services_[sinfo.service_name()] = maps;
        }

        // get client from clients_
        auto clients = clients_.find(sinfo.service_name());
        if (clients != clients_.end())
        {
            std::transform(clients->second.begin(), clients->second.end(), back_inserter(clts),\
              [](std::map<std::string, nsky_rpc::ClientInfo>::value_type & value) {
                  return value.second;
              });
        }
        return clts;
    }

    std::list<nsky_rpc::ServerInfo> ServiceManager::AddClient(const nsky_rpc::ClientInfo &cinfo)
    {
        std::unique_lock<std::mutex> slk(smtx_);
        std::unique_lock<std::mutex> clk(cmtx_);
        std::list<nsky_rpc::ServerInfo> slts;

        auto cmap = clients_.find(cinfo.service_name());

        // add to clients_
        if (cmap != clients_.end())
        {
            cmap->second[cinfo.physical_node_info().address()] = cinfo;
        }
        else
        {
            // no this topic
            std::map<std::string,nsky_rpc::ClientInfo> maps;
            maps[cinfo.physical_node_info().address()] = cinfo;
            clients_[cinfo.service_name()] = maps;
        }

        // get client from clients_
        auto services = services_.find(cinfo.service_name());
        if (services != services_.end())
        {
            std::transform(services->second.begin(), services->second.end(), back_inserter(slts),\
              [](std::map<std::string, nsky_rpc::ServerInfo>::value_type & value) {
                  return value.second;
              });
        }
        return slts;
    }

    void ServiceManager::RemoveService(const nsky_rpc::ServerInfo &sinfo)
    {
        if (IsHasService(sinfo))
        {
            // true
            std::unique_lock<std::mutex> lk(smtx_);
            auto smap = services_.find(sinfo.service_name());
            smap->second.erase(sinfo.physical_node_info().address());
        }
    }

    void ServiceManager::RemoveClient(const nsky_rpc::ClientInfo &cinfo)
    {
        if (IsHasClient(cinfo))
        {
            // true
            std::unique_lock<std::mutex> lk(cmtx_);
            auto cmap = clients_.find(cinfo.service_name());
            cmap->second.erase(cinfo.physical_node_info().address());
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

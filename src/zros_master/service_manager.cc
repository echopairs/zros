#include <algorithm>
#include <zros.pb.h>
#include "service_manager.h"

namespace zros {

    ServiceManager::ServiceManager() {
        servers_.clear();
        clients_.clear();
    }

    ServiceManager::~ServiceManager() {
        removeAllServer();
        removeAllClient();
    }

    std::list<zros_rpc::ServiceClientInfo> ServiceManager::addServer(const zros_rpc::ServiceServerInfo& serverInfo) {
        std::unique_lock<std::mutex> slk(smtx_);
        std::unique_lock<std::mutex> clk(cmtx_);
        std::list<zros_rpc::ServiceClientInfo> clientList;
        servers_[serverInfo.service_name()] = serverInfo;
        auto clientMap = clients_.find(serverInfo.service_name());
        if (clientMap != clients_.end()) {
            std::transform(clientMap->second.begin(), clientMap->second.end(), std::back_inserter(clientList),
                           [](std::map<std::string, zros_rpc::ServiceClientInfo>::value_type& value) { return value.second;});
        }
        return clientList;
    }

    zros_rpc::ServiceServerInfo ServiceManager::addClient(const zros_rpc::ServiceClientInfo& clientInfo) {
        std::unique_lock<std::mutex> slk(smtx_);
        std::unique_lock<std::mutex> clk(cmtx_);
        auto clients = clients_.find(clientInfo.service_name());
        if (clients == clients_.end()) {
            // first client
            std::map<std::string, zros_rpc::ServiceClientInfo> clientMap;
            clientMap[clientInfo.physical_node_info().agent_address()] = clientInfo;
            clients_[clientInfo.service_name()] = clientMap;
        } else {
            clients->second[clientInfo.physical_node_info().agent_address()] = clientInfo;
        }
        auto server = servers_.find(clientInfo.service_name());
        if (server != servers_.end()) {
            return server->second;
        } else {
            return zros_rpc::ServiceServerInfo();
        }
    }

    void ServiceManager::removeServer(const zros_rpc::ServiceServerInfo& serverInfo) {
        if (isHasServer(serverInfo)) {
            std::unique_lock<std::mutex> lk(smtx_);
            servers_.erase(serverInfo.service_name());
        }
    }

    void ServiceManager::removeClient(const zros_rpc::ServiceClientInfo &cinfo) {
        if (isHasClient(cinfo)) {
            // true
            std::unique_lock<std::mutex> lk(cmtx_);
            auto cmap = clients_.find(cinfo.service_name());
            cmap->second.erase(cinfo.physical_node_info().agent_address());
        }
    }

    bool ServiceManager::isHasClient(const zros_rpc::ServiceClientInfo &cinfo) {
        std::unique_lock<std::mutex> lk(cmtx_);
        auto cmap = clients_.find(cinfo.service_name());
        if (cmap != clients_.end()) {
            auto c = cmap->second.find(cinfo.physical_node_info().agent_address());
            if (c != cmap->second.end()) {
              return true;
            }
        }
        return false;
    }

    bool ServiceManager::isHasServer(const zros_rpc::ServiceServerInfo& serverInfo) {
        std::unique_lock<std::mutex> lk(smtx_);
        if (servers_.find(serverInfo.service_name()) == servers_.end()) {
            return false;
        }
        return true;
    }

    void ServiceManager::removeAllServer() {
        std::unique_lock<std::mutex> lk(smtx_);
        servers_.clear();
    }

    void ServiceManager::removeAllClient() {
        std::unique_lock<std::mutex> lk(cmtx_);
        clients_.clear();
    }
} // end of namespace nsky

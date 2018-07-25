#include <sspdlog/sspdlog.h>
#include "connect_task.h"
#include "service_manager.h"
#include "node_manager.h"

namespace zros {


    ServiceConnectTask::ServiceConnectTask(const IConnectTask::taskType &type,
                                           const zros_rpc::ServiceServerInfo &serverInfo,
                                           const zros_rpc::ServiceClientInfo &clientInfo,
                                           std::shared_ptr<NodeManager> nodeManager,
                                           std::shared_ptr<ServiceManager> serviceManager):
                IConnectTask(type),
                serverInfo_(serverInfo),
                clientInfo_(clientInfo),
                nodeManager_(nodeManager),
                serviceManager_(serviceManager) {

    }

    TaskStatus ServiceConnectTask::performTask() {
        TaskStatus taskStatus(TaskStatus::TaskStatusFlag::OK);
        zros_rpc::Status response;
        grpc::Status status;

        // check server node
        bool hasServer = serviceManager_->isHasServer(serverInfo_);
        if (hasServer) {
            // server node already remove
            if (!nodeManager_->getNode(serverInfo_.physical_node_info().agent_address())) {
                taskStatus.flag_ = taskStatus.Error;
                taskStatus.details_.emplace_back(serverInfo_.physical_node_info().agent_address() + " node already remove");
                serviceManager_->removeServer(serverInfo_);
                hasServer = false;
            }
        } else {
            taskStatus.flag_ = taskStatus.Error;
            taskStatus.details_.emplace_back(serverInfo_.service_name() + " server already be remove");
        }

        // check client node
        bool hasClient = serviceManager_->isHasClient(clientInfo_);
        if (hasClient) {
            auto client = nodeManager_->getNode(clientInfo_.physical_node_info().agent_address());
            // client node already remove
            if (!client) {
                taskStatus.flag_ = taskStatus.Error;
                taskStatus.details_.emplace_back(clientInfo_.physical_node_info().agent_address() + " node already remove");
                serviceManager_->removeClient(clientInfo_);
                hasClient = false;
            }
        } else {
            taskStatus.flag_ = taskStatus.Error;
            taskStatus.details_.emplace_back(clientInfo_.service_name() + " client already be remove");
        }
        if (!hasServer || !hasClient) {
            return taskStatus;
        }

        auto context = std::make_shared<grpc::ClientContext>();
        context->set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(3));
        auto stub = nodeManager_->getNodeStub(clientInfo_.physical_node_info().agent_address());
        SSPD_LOG_INFO << "agent_address is " << clientInfo_.physical_node_info().agent_address();
        if (type_ == taskType::connect) {
            SSPD_LOG_INFO << serverInfo_.physical_node_info().agent_address();
            status = stub->RegisterServiceServer(context.get(), serverInfo_, &response);
        } else {
            status = stub->UnregisterServiceServer(context.get(), serverInfo_, &response);
        }
        if (!status.ok()) {
            SSPD_LOG_WARNING << "register service server failed" << status.error_details();
            SSPD_LOG_INFO << status.error_message();
            taskStatus.flag_ = taskStatus.Error;
            taskStatus.details_.emplace_back(status.error_message());
        }
        return taskStatus;
    }
}   // namespace zros
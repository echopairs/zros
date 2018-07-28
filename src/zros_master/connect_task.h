/*********************************************************************
  Copyright (C), 1990-2016, HangZhou RED HEAT Tech. Co., Ltd.
  FileName: connect_task.h
  Author  : pairs & 736418319@qq.com
  Version : 1.0
  Date    : 2016/11/21
  Description:
      help build  or disconnect connection with two node
  Function List:
  History:
      <author>    <time>    <version>    <desc>
        pairs     16/11/21      1.0     build this moudle
*********************************************************************/
#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include "zros.grpc.pb.h"

namespace zros {
    class NodeManager;
    class ServiceManager;
    class TopicManager;

    class TaskStatus {
    public:
        enum TaskStatusFlag {
            Error = 1 << 0,
            OK    = 1 << 1
        };

        TaskStatus(const TaskStatusFlag& flag)
                :flag_(flag) {
            details_.clear();
        }
        TaskStatusFlag flag_;
        std::vector<std::string> details_;
    };

    // may be take this as interface
    class IConnectTask {
    public:
        enum taskType {
            connect,
            disconnect
        };

        IConnectTask(const taskType& type):type_(type) {
        }
        virtual TaskStatus performTask() = 0;
        void set_dead_time(std::chrono::steady_clock::time_point dead_time) {
          dead_time_ = dead_time;
        }
    protected:
        taskType type_;
        std::chrono::steady_clock::time_point dead_time_ = std::chrono::steady_clock::now();
    };

    class ServiceConnectTask : public IConnectTask {
    public:
        ServiceConnectTask(const taskType& type,
                           const zros_rpc::ServiceServerInfo& serverInfo,
                           const zros_rpc::ServiceClientInfo& clientInfo,
                           std::shared_ptr<NodeManager> nodeManager,
                           std::shared_ptr<ServiceManager> serviceManager
        );
        TaskStatus performTask() override ;
    private:
        zros_rpc::ServiceServerInfo serverInfo_;
        zros_rpc::ServiceClientInfo clientInfo_;
        std::shared_ptr<NodeManager> nodeManager_;
        std::shared_ptr<ServiceManager> serviceManager_;
    };


    class TopicConnectTask : public IConnectTask {
    public:
        TopicConnectTask(const taskType& type,
                         const zros_rpc::PublisherInfo& publisherInfo,
                         const zros_rpc::SubscriberInfo& subscriberInfo,
                         std::shared_ptr<NodeManager>nodeManager, std::shared_ptr<TopicManager>);
        TaskStatus performTask() override ;

    private:
        zros_rpc::PublisherInfo publisherInfo_;
        zros_rpc::SubscriberInfo subscriberInfo_;
        std::shared_ptr<NodeManager> nodeManager_;
        std::shared_ptr<TopicManager> topicManager_;
    };
}  // namespace zros

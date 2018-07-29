//
// Created by pairs on 7/26/18.
//

#pragma once

#include <mutex>
#include <map>
#include <list>
#include <memory>

#include <zros.grpc.pb.h>

namespace zros {

    class TopicManager {
    public:
        std::list<zros_rpc::SubscriberInfo> addPublisher(const zros_rpc::PublisherInfo& publisherInfo);
        std::list<zros_rpc::PublisherInfo> addSubscriber(const zros_rpc::SubscriberInfo& subscriberInfo);
        void removePublisher(const zros_rpc::PublisherInfo& publisherInfo);
        void removeSubscriber(const zros_rpc::SubscriberInfo& subscriberInfo);

        bool isHasPublisher(const zros_rpc::PublisherInfo& publisherInfo);
        bool isHasSubscriber(const zros_rpc::SubscriberInfo& subscriberInfo);
    private:
        std::mutex mtx_;
        std::map<std::string, std::map<std::string, zros_rpc::PublisherInfo>> publishers_;      // key: topic, value: (key: agent_address, value: publishInfo)
        std::map<std::string, std::map<std::string, zros_rpc::SubscriberInfo>> subscribers_;    // key: topic, value: (key: agent_address, value: subscriberInfo)
    };
}   // namespace zros
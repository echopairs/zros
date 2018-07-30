//
// Created by pairs on 7/26/18.
//

#include "topic_manager.h"

namespace zros {

    std::list<zros_rpc::SubscriberInfo> TopicManager::addPublisher(const zros_rpc::PublisherInfo &publisherInfo) {
        std::lock_guard<std::mutex> lk(mtx_);
        auto existingPublisherMap = publishers_.find(publisherInfo.topic());
        if (existingPublisherMap == publishers_.end()) {
            std::map<std::string, zros_rpc::PublisherInfo> publisherMap;
            publisherMap[publisherInfo.physical_node_info().agent_address()] = publisherInfo;
            publishers_[publisherInfo.topic()] = publisherMap;
        } else {
            existingPublisherMap->second[publisherInfo.physical_node_info().agent_address()] = publisherInfo;
        }
        std::list<zros_rpc::SubscriberInfo> subscriberList;
        auto subscriberMap = subscribers_.find(publisherInfo.topic());
        if (subscriberMap != subscribers_.end()) {
            std::transform(subscriberMap->second.begin(), subscriberMap->second.end(), std::back_inserter(subscriberList),
            [](std::map<std::string, zros_rpc::SubscriberInfo>::value_type & value ){ return value.second; });
        }
        return subscriberList;
    }

    std::list<zros_rpc::PublisherInfo> TopicManager::addSubscriber(const zros_rpc::SubscriberInfo &subscriberInfo) {
        std::lock_guard<std::mutex> lk(mtx_);
        auto existingSubscriberMap = subscribers_.find(subscriberInfo.topic());
        if (existingSubscriberMap == subscribers_.end()) {
            std::map<std::string, zros_rpc::SubscriberInfo> subscriberMap;
            subscriberMap[subscriberInfo.physical_node_info().agent_address()] = subscriberInfo;
            subscribers_[subscriberInfo.topic()] = subscriberMap;
        }
        else {
            existingSubscriberMap->second[subscriberInfo.physical_node_info().agent_address()] = subscriberInfo;
        }
        std::list<zros_rpc::PublisherInfo> publisherList;
        auto publisherMap = publishers_.find(subscriberInfo.topic());
        if (publisherMap != publishers_.end()) {
            std::transform(publisherMap->second.begin(), publisherMap->second.end(), back_inserter(publisherList),
                      [](std::map<std::string, zros_rpc::PublisherInfo>::value_type  & value){return value.second; });
        }
        return publisherList;
    }

    void TopicManager::removePublisher(const zros_rpc::PublisherInfo &publisherInfo) {
        std::lock_guard<std::mutex> lk(mtx_);
        auto existingPublisherMap = publishers_.find(publisherInfo.topic());
        if (existingPublisherMap != publishers_.end()) {
            auto existingPublisher = existingPublisherMap->second.find(publisherInfo.physical_node_info().agent_address());
            if (existingPublisher != existingPublisherMap->second.end())
                existingPublisherMap->second.erase(existingPublisher);
        }
    }

    void TopicManager::removeSubscriber(const zros_rpc::SubscriberInfo &subscriberInfo) {
        std::lock_guard<std::mutex> lk(mtx_);
        auto existingSubscriberMap = subscribers_.find(subscriberInfo.topic());
        if (existingSubscriberMap != subscribers_.end()) {
            auto existingSubscriber = existingSubscriberMap->second.find(subscriberInfo.physical_node_info().agent_address());
            if (existingSubscriber != existingSubscriberMap->second.end())
                existingSubscriberMap->second.erase(existingSubscriber);
        }
    }

    bool TopicManager::isHasPublisher(const zros_rpc::PublisherInfo &publisherInfo) {
        std::lock_guard<std::mutex> lk(mtx_);
        auto existingPublisherMap = publishers_.find(publisherInfo.topic());
        if (existingPublisherMap == publishers_.end()) {
            return false;
        } else {
            return existingPublisherMap->second.find(publisherInfo.physical_node_info().agent_address()) !=
                   existingPublisherMap->second.end();
        }

    }

    bool TopicManager::isHasSubscriber(const zros_rpc::SubscriberInfo &subscriberInfo) {
        std::lock_guard<std::mutex> lk(mtx_);
        auto existingSubscriberMap = subscribers_.find(subscriberInfo.topic());
        if (existingSubscriberMap == subscribers_.end()){
            return false;
        } else {
            return existingSubscriberMap->second.find(subscriberInfo.physical_node_info().agent_address()) !=
                   existingSubscriberMap->second.end();
        }
    }
}   // namespace zros

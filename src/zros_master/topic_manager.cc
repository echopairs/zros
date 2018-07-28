//
// Created by pairs on 7/26/18.
//

#include "topic_manager.h"

namespace zros {

    std::list<zros_rpc::SubscriberInfo> TopicManager::addPublisher(const zros_rpc::PublisherInfo &publisherInfo) {

    }

    std::list<zros_rpc::PublisherInfo> TopicManager::addSubscriber(const zros_rpc::SubscriberInfo &subscriberInfo) {
        return std::list<zros_rpc::PublisherInfo>();
    }

    void TopicManager::removePublisher(const zros_rpc::PublisherInfo &publisherInfo) {

    }

    void TopicManager::removeSubscriber(const zros_rpc::SubscriberInfo &subscriberInfo) {

    }

    bool TopicManager::isHasPublisher(const zros_rpc::PublisherInfo &publisherInfo) {
        return false;
    }

    bool TopicManager::isHasSubscriber(const zros_rpc::SubscriberInfo &subscriberInfo) {
        return false;
    }
}   // namespace zros

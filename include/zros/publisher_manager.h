//
// Created by pairs on 7/25/18.
//

#pragma once

#include "service_discovery.h"
#include "publisher_interface.h"
#include "publisher.h"

namespace zros {
    // todo
    class PublishersImpl;
    class PublisherManager {
    public:
        PublisherManager();
        bool registerPublisher(const std::shared_ptr<IPublisher> publisher);
        bool unregisterPublisher(const std::string& topic);
        void publish(const std::string& topic, const std::string& content);
    protected:
        std::shared_ptr<ServiceDiscovery> service_discovery_;
        std::shared_ptr<PublishersImpl> impl_;
    };
}   // namespace zros

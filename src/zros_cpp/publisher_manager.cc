//
// Created by pairs on 7/25/18.
//

#include <zros/publisher_manager.h>
#include <zros/singleton.h>
#include <sspdlog/sspdlog.h>
#include "impl/publisher_impl.h"

namespace zros {
    void PublisherManager::publish(const std::string &topic, const std::string &content) {
        impl_->publish(topic, content);
    }

    PublisherManager::PublisherManager() {
        impl_ = std::make_shared<PublishersImpl>();
        service_discovery_ = SingleTon<ServiceDiscovery>::Instance();
    }

    bool PublisherManager::registerPublisher(const std::shared_ptr<IPublisher> publisher) {
        // 1. register to memory first (should bind port first)
        impl_->registerPublisher(publisher);
        // 2. register to master
        bool ok = service_discovery_->addPublisher(publisher);
        if (!ok) {
            SSPD_LOG_WARNING << "register publisher " << publisher->get_topic() << " to master failed";
            return false;
        }
        return true;
    }

    bool PublisherManager::unregisterPublisher(const std::string &topic) {
        return false;
    }
}
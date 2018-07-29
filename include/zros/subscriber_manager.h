//
// Created by pairs on 7/28/18.
//

#pragma once

#include "subscriber.h"
#include "service_discovery.h"

namespace zros {
    class SubscribersImpl;
    class SubscriberManager {
    public:
        SubscriberManager();
        bool registerSubscriber(const std::shared_ptr<ISubscriber> subscriber);
        bool unregisterSubscriber(const std::string& topic);


    private:
        std::shared_ptr<ServiceDiscovery> service_discovery_;
        std::shared_ptr<SubscribersImpl> impl_;
    };
}

//
// Created by pairs on 7/28/18.
//

#include <zros/subscriber_manager.h>
#include <zros/singleton.h>
#include <sspdlog/sspdlog.h>
#include "impl/subscriber_impl.h"

namespace zros {
    // todo
    SubscriberManager::SubscriberManager() {
        impl_ = std::make_shared<SubscribersImpl>();
        service_discovery_ = SingleTon<ServiceDiscovery>::Instance();
        service_discovery_->set_register_publisher_cb([this](const zros_rpc::PublisherInfo* publisherInfo, zros_rpc::Status* status){
            impl_->registerPublisher(publisherInfo, status);
        });
        service_discovery_->set_unregister_publisher_cb([this](const zros_rpc::PublisherInfo* publisherInfo, zros_rpc::Status* status){
           impl_->unregisterPublisher(publisherInfo, status);
        });
    }

    bool SubscriberManager::registerSubscriber(const std::shared_ptr<ISubscriber> subscriber) {
        // 1. register to master first
        bool ok = service_discovery_->addSubscriber(subscriber);
        if (ok) {
            // 2. register to memory
            SSPD_LOG_INFO << "register subscriber " << subscriber->get_topic() << " to master success";
            return impl_->registerSubscriber(subscriber);
        }
        SSPD_LOG_WARNING << "register subscriber " << subscriber->get_topic() << " to master failed";
    }

    bool SubscriberManager::unregisterSubscriber(const std::string &topic) {
        return impl_->unregisterSubscriber(topic);
    }

}
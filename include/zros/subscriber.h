//
// Created by pairs on 7/28/18.
//

#pragma once

#include "subscriber_interface.h"
#include "serialization_helper.h"
#include <functional>

namespace zros {
    template <typename TMessage>
    class Subscriber : public ISubscriber {
    public:
        Subscriber(const std::string& topic, std::function<void(const TMessage&)> cb, std::shared_ptr<ThreadPool> thread_pool):
                ISubscriber(topic, thread_pool), subscriber_cb_(cb){
        }
        void handleRawMessage(const std::string& message) {
            TMessage type_message;
            parseFromString(message, &type_message);
            thread_pool_->enqueue(subscriber_cb_, type_message);
        }
    protected:
        std::function<void(const TMessage& )> subscriber_cb_;
    };
}   // namespace zros


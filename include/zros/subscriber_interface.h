//
// Created by pairs on 7/28/18.
//

#pragma once

#include <string>
#include "thread_pool.h"

namespace zros {
    class NodeHandle;
    class ISubscriber {
    public:
        ISubscriber(const std::string& topic, std::shared_ptr<ThreadPool> thread_pool):
                topic_(topic),thread_pool_(thread_pool) {}
        virtual void handleRawMessage(const std::string& message) = 0;
        std::string get_topic() {
            return topic_;
        }
    protected:
        std::shared_ptr<ThreadPool> thread_pool_;
        std::string topic_;
    };
}   // namespace zros

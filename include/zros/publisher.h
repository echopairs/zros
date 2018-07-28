//
// Created by pairs on 7/25/18.
//

#pragma once

#include <string>
#include "publisher_interface.h"

namespace zros {
    class NodeHandle;
    template <typename TMessage>
    class Publisher : public IPublisher {
    public:
        Publisher(const std::string& topic, std::shared_ptr<NodeHandle> node_handle)
                : IPublisher(topic, node_handle){}

        void publish(TMessage& message) {
            // todo check message type
            auto pub_data = serializeToString(message);
            publish_(pub_data);
        }
    protected:
    };
}   // namespace zros
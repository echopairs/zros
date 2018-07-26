//
// Created by pairs on 7/25/18.
//

#pragma once

#include <string>
#include "publisher_interface.h"

namespace zros {

    template <typename TMessage>
    class Publisher : public IPublisher {
    public:
        Publisher(const std::string& topic, std::shared_ptr<NodeHandle> node_handle)
                : IPublisher(topic, node_handle){}
    protected:
    };
}   // namespace zros
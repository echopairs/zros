//
// Created by pairs on 7/20/18.
//

#include <zros/publisher_interface.h>
#include <zros/node_handle.h>

namespace zros {
    void IPublisher::publish_(const std::string &message) {
        node_handle_->publish(topic_, message);
    }
}
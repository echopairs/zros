//
// Created by pairs on 7/25/18.
//

#pragma once

#include <string>
#include <memory>

namespace zros {
    class NodeHandle;
    class IPublisher {
    public:
        IPublisher(const std::string& topic, std::shared_ptr<NodeHandle> node_handle):
                topic_(topic),
                node_handle_(node_handle) {
        }

        const std::shared_ptr <NodeHandle> &get_node_handle() const {
            return node_handle_;
        }

        void set_node_handle(const std::shared_ptr <NodeHandle> &node_handle_) {
            IPublisher::node_handle_ = node_handle_;
        }

        const std::string &get_address() const {
            return address_;
        }

        void set_address(const std::string &address_) {
            IPublisher::address_ = address_;
        }

        const std::string &get_topic() const {
            return topic_;
        }

        void set_topic(const std::string &topic_) {
            IPublisher::topic_ = topic_;
        }

    protected:
        void publish_(const std::string& message);
        std::shared_ptr<NodeHandle> node_handle_;
        std::string address_;
        std::string topic_;
    };
}   // namespace zros
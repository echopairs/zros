//
// Created by pairs on 7/25/18.
//

#pragma once
#include <mutex>
#include <memory>
#include <map>

#include <zmq.hpp>

#include <zros/publisher.h>

namespace zros {
    class PubStub {
    public:
        PubStub(const std::string& topic):topic_(topic) {
        }
        virtual ~PubStub() {}

        virtual bool publish(const std::string& message) = 0;

        void set_address(const std::string& address) {
            address_ = address;
        }
        const std::string & get_address() {
            return address_;
        }
    private:
        std::string topic_;
        std::string address_;
    };

    class ZmqPubStub : public PubStub {
    public:
        ZmqPubStub(const std::string& topic);
        bool publish(const std::string& message) override ;
    private:
        std::shared_ptr<zmq::context_t> context_;
        std::shared_ptr<zmq::socket_t> pub_stub_;
        // zeromq socket_t is not thread safety
        std::mutex mtx_;
    };


    class PublishersImpl {
    public:
        void registerPublisher(const std::shared_ptr<IPublisher> publisher);
        PublishersImpl();
        bool publish(const std::string& topic, const std::string& message);
    private:
        const std::string& _create_publisher(const std::string& topic);
        std::mutex pubs_mutex_;
        std::map<std::string, std::shared_ptr<PubStub>> publishers_;
    };
}   // namespace zros

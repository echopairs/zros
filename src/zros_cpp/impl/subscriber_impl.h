//
// Created by pairs on 7/28/18.
//

#pragma once

#include <string>
#include <zmq.hpp>
#include <mutex>
#include <map>
#include <zros.grpc.pb.h>
#include <zros/subscriber_interface.h>

namespace zros {

    using MessageCb = std::function<void(std::string&) >;
    // one sub multiple pub
    class ISubStub {
    public:
        ISubStub(const std::string& topic):
                topic_(topic){}
        virtual bool start() = 0;
        virtual bool stop() = 0;
        virtual bool addPublisher(const std::string& address) = 0;
        virtual bool removePublisher(const std::string& address) = 0;
        virtual ~ISubStub(){}
    protected:
        std::string topic_;
        std::mutex pub_mutex_;
        std::map<std::string, int> publishers_address_;
    };

    class ZmqSubStub : public ISubStub {
    public:
        ZmqSubStub(const std::string& topic, MessageCb cb);
        bool start() override ;
        bool stop() override ;
        bool addPublisher(const std::string& address) override;
        bool removePublisher(const std::string& address) override;
        ~ZmqSubStub();
    private:
        void recvMessage();
        MessageCb message_cb_;
        std::shared_ptr<zmq::context_t> context_;
        std::shared_ptr<zmq::socket_t> subscriber_;
        std::atomic<bool> running_;
        std::atomic<bool> stop_;
        std::shared_ptr<std::thread> recvThread_;
    };

    class SubscribersImpl {
    public:
        SubscribersImpl();
        bool registerSubscriber(std::shared_ptr<ISubscriber> subscriber);
        bool unregisterSubscriber(const std::string& topic);
        void registerPublisher(const zros_rpc::PublisherInfo* publisherInfo, zros_rpc::Status* status);
        void unregisterPublisher(const zros_rpc::PublisherInfo* publisherInfo, zros_rpc::Status* status);
    private:
        std::mutex subscriber_mutex_;
        std::map<std::string, std::pair<std::shared_ptr<ISubscriber>, std::shared_ptr<ZmqSubStub> > > subscribers_; // key: topic, value: sub pair
    };
}   // namespace zros

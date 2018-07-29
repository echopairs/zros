//
// Created by pairs on 7/28/18.
//

#include "subscriber_impl.h"
#include "zeromq_help.h"

namespace zros {

    ZmqSubStub::ZmqSubStub(const std::string &topic, MessageCb cb):
            ISubStub(topic),
            message_cb_(cb),
            running_(false),
            stop_(false) {
        context_ = std::make_shared<zmq::context_t>(1);
        subscriber_ = std::make_shared<zmq::socket_t>(*context_.get(), ZMQ_SUB);
    }

    bool ZmqSubStub::start() {
        if (running_) {
            SSPD_LOG_INFO << "the topic: " << topic_ << "was already recv message";
            return true;
        }
        subscriber_->setsockopt(ZMQ_SUBSCRIBE, NULL, 0);
        recvThread_ = std::make_shared<std::thread>(&ZmqSubStub::recvMessage, this);
        running_ = true;
        return true;
    }

    bool ZmqSubStub::stop() {
        stop_ = true;
        running_ = false;
        return true;
    }

    bool ZmqSubStub::addPublisher(const std::string &address) {
        std::lock_guard<std::mutex> lk(pub_mutex_);
        if (publishers_address_.find(address) == publishers_address_.end()) {
            subscriber_->connect(address.c_str());
            publishers_address_[address] = 1;
        }
        start();
        return true;
    }

    bool ZmqSubStub::removePublisher(const std::string &address) {
        std::lock_guard<std::mutex> lk(pub_mutex_);
        if (publishers_address_.find(address) != publishers_address_.end()) {
            subscriber_->unbind(address);
            publishers_address_.erase(address);
        }
        return true;
    }

    void ZmqSubStub::recvMessage() {
        while(!stop_) {
            try {
                std::string data;
                // blocking waiting publish data
                data = zeromq::ZmqHelp::s_recv(*(subscriber_.get()));
                message_cb_(data);
            } catch(std::exception& e) {
                SSPD_LOG_ERROR << "zmq recv message thread throw exception, because of " << e.what();
            }
        }
    }

    ZmqSubStub::~ZmqSubStub() {
        stop();
        if (recvThread_) {
            recvThread_->join();
        }
    }

    ////////////// subscriberImpl ///////////////////
    SubscribersImpl::SubscribersImpl() {

    }

    void SubscribersImpl::registerPublisher(const zros_rpc::PublisherInfo *publisherInfo, zros_rpc::Status *status) {
        std::lock_guard<std::mutex> lk(subscriber_mutex_);
        auto topic = publisherInfo->topic();
        if (subscribers_.find(topic) == subscribers_.end()) {
            SSPD_LOG_WARNING << "please check registerPublisher logical error " << topic;
            status->set_code(status->NOT_FOUND);
            status->set_details(topic + " publisher not register on");
            return;
        }
        auto publish_address = publisherInfo->physical_node_info().real_address();
        auto pair = subscribers_[topic];
        auto stub = pair.second;
        stub->addPublisher(publish_address);
        status->set_code(status->OK);
        SSPD_LOG_INFO << "register publisher to : " << topic << " on address: " << publish_address << " success";
    }

    void SubscribersImpl::unregisterPublisher(const zros_rpc::PublisherInfo *publisherInfo, zros_rpc::Status *status) {

    }

    bool SubscribersImpl::registerSubscriber(std::shared_ptr<ISubscriber> subscriber) {
        std::lock_guard<std::mutex> lk(subscriber_mutex_);
        auto topic = subscriber->get_topic();
        if (subscribers_.find(topic) != subscribers_.end()) {
            SSPD_LOG_WARNING << "please check this subscriber already register " << topic;
            return false;
        }
        auto stub = std::make_shared<ZmqSubStub>(topic, [subscriber](const std::string& message){
            subscriber->handleRawMessage(message);
        });
        auto pair = std::make_pair<std::shared_ptr<ISubscriber>, std::shared_ptr<ZmqSubStub> >(std::move(subscriber), std::move(stub));
        subscribers_[topic] = pair;
        return true;
    }

    bool SubscribersImpl::unregisterSubscriber(const std::string &topic) {
        std::lock_guard<std::mutex> lk(subscriber_mutex_);
        subscribers_.erase(topic);
        return true;
    }
}   // namespace zros
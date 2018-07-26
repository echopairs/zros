//
// Created by pairs on 7/25/18.
//

#include <sspdlog/sspdlog.h>
#include "publisher_impl.h"
#include "zeromq_help.h"

namespace zros {

    ZmqPubStub::ZmqPubStub(const std::string& topic):PubStub(topic) {
        context_ = std::make_shared<zmq::context_t>(1);
        pub_stub_ = std::make_shared<zmq::socket_t>(*context_.get(), ZMQ_PUB);
        pub_stub_->bind("tcp://*:*");
        const size_t address_size = 32;
        char address[address_size];
        auto rc = zmq_getsockopt(*pub_stub_.get(), ZMQ_LAST_ENDPOINT, address, (size_t*)&address_size);
        assert(rc == 0);
        this->set_address(address);
    }

    bool ZmqPubStub::publish(const std::string& message) {
        std::lock_guard<std::mutex> lk(mtx_);
        return zeromq::ZmqHelp::s_send(*pub_stub_.get(), message);
    }

    //////////////////// publishersImpl///////////////////////////
    PublishersImpl::PublishersImpl() {
    }

    bool PublishersImpl::publish(const std::string &topic, const std::string &message) {
        return false;
    }

    void PublishersImpl::registerPublisher(const std::shared_ptr<IPublisher> publisher) {
        std::lock_guard<std::mutex> lk(pubs_mutex_);
        if (publishers_.find(publisher->get_topic()) != publishers_.end()) {
            SSPD_LOG_WARNING << "the publisher " << publisher->get_topic() << " already register";
            publisher->set_address(publishers_[publisher->get_topic()]->get_address());
            return;
        }
        auto address = _create_publisher(publisher->get_topic());
        publisher->set_address(address);
    }

    // return pub address
    const std::string &PublishersImpl::_create_publisher(const std::string &topic) {
        publishers_[topic] = std::make_shared<ZmqPubStub>(topic);
        const std::string& address = publishers_[topic]->get_address();
        SSPD_LOG_INFO << "registerPublisher: " << topic << " on address " << address << " success";
        return address;
    }
}   // namespace zros
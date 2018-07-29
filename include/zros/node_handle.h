//
// Created by pairs on 7/15/18.
//

#pragma once

#include <string>
#include <memory>
#include <future>
#include <zros/thread_pool.h>

#include <zros/service_server_manager.h>
#include <zros/service_client_manager.h>
#include <zros/publisher_manager.h>
#include <zros/subscriber_manager.h>
#include <zros.pb.h>

namespace zros {

    class NodeHandle : public std::enable_shared_from_this<NodeHandle> {
    public:

        template <typename TRequest, typename TResponse>
        std::shared_ptr<ServiceServer<TRequest, TResponse> > advertiseService (
                const std::string &service_name,
                std::function<zros_rpc::Status(const TRequest *, TResponse *)> service_function
        ) {
            if (service_name.length() <= 0 || service_function == nullptr) {
                throw invalid_argument("service_name && service_function cannot be empty");
            }
            auto service_server = std::make_shared<ServiceServer<TRequest, TResponse> > (service_name, service_function, shared_from_this());
            bool ok = service_server_mgr_->registerServer(service_server);
            if (!ok) {
                SSPD_LOG_ERROR << "advertiseService " << service_name << " failed";
                return nullptr;
            }
            return service_server;
        };

        template <typename TRequest, typename TResponse>
        std::shared_ptr<ServiceClient<TRequest, TResponse> > serviceClient (
                const std::string &service_name,
                const std::string &client_node_info = ""
                ) {
            if (service_name.length() <= 0) {
                throw invalid_argument("service_name cannot be empty");
            }
            auto service_client = std::make_shared<ServiceClient<TRequest, TResponse>> (service_name, client_node_info, shared_from_this());
            bool ok = service_client_mgr_->registerClient(service_client);
            if (!ok) {
                SSPD_LOG_ERROR << "serviceClient " << service_name << " failed";
                return nullptr;
            }
            return service_client;
        };

        template <typename TMessage>
        std::shared_ptr<Publisher<TMessage> > advertise(const std::string& topic) {
            if (topic.length() <= 0) {
                throw invalid_argument("topic cannot be empty");
            }
            auto publisher = std::make_shared<Publisher<TMessage> > (topic, shared_from_this());
            bool ok = publisher_manager_->registerPublisher(publisher);
            if (!ok) {
                SSPD_LOG_ERROR << "advertise publisher " << topic << " failed";
                return nullptr;
            }
            return publisher;
        }

        template <typename TMessage>
        std::shared_ptr<Subscriber<TMessage> > subscribe(const std::string& topic, std::function<void(const TMessage& message)> cb) {
            if (topic.length() <= 0) {
                throw invalid_argument("topic cannot be empty");
            }
            auto subscriber = std::make_shared<Subscriber<TMessage> > (topic, cb, thread_pool_);
            bool ok = subscriber_manager_->registerSubscriber(subscriber);
            if (!ok) {
                SSPD_LOG_ERROR << "subscribe subscriber " << topic << " failed";
                return nullptr;
            }
            return subscriber;
        }

        std::shared_ptr<zros_rpc::ServiceResponse> call(const std::string & service_name, const std::string & content,
                                                        const std::string & cli_info, int timeout_mseconds);
        void publish(const std::string& topic, const std::string& content);
        // todo random generate
        NodeHandle(const std::string &node_address, const std::string &node_name);

        void spin();
        const string &get_node_address() const;
        const string &get_node_name() const;
    private:
        // rpc server address
        std::string node_address_;
        std::string node_name_;
        std::shared_ptr<ThreadPool> thread_pool_;

        std::shared_ptr<ServiceServerManager> service_server_mgr_;
        std::shared_ptr<ServiceClientManager> service_client_mgr_;
        std::shared_ptr<PublisherManager> publisher_manager_;
        std::shared_ptr<SubscriberManager> subscriber_manager_;

    };
}

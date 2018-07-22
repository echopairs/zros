//
// Created by pairs on 7/13/18.
//

#include <zros/zros.h>
#include <zros/singleton.h>
#include <zros/error.h>

namespace zros {

    std::shared_ptr<ServiceDiscovery> serviceDiscovery = nullptr;
    void init(const std::string &masterAddress, const std::string &agentAddress) {
        serviceDiscovery = SingleTon<ServiceDiscovery>::Instance(masterAddress, agentAddress);
    }

    void spin() {
        if (!serviceDiscovery) {
            throw zros_not_initialized("please check zros_not_initialized");
        }
        serviceDiscovery->spin();
    }
}
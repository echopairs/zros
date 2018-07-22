//
// Created by pairs on 7/16/18.
//

#pragma once

#include <memory>
#include <string>
#include <zros/service_discovery.h>

namespace zros {
    extern std::shared_ptr<ServiceDiscovery> serviceDiscovery;
    void init(const std::string &masterAddress, const std::string &agentAddress);
    void spin();
}
//
// Created by pairs on 7/13/18.
//

#include <zros/service_discovery.h>
#include <thread>
#include <iostream>

int main() {

    auto service_discovery = std::make_shared<zros::ServiceDiscovery>("0.0.0.0:23333", "0.0.0.0:23334");
    service_discovery->init();
    service_discovery->spin();
    getchar();
}

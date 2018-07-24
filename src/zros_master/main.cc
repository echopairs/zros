#include <iostream>
#include <zros/signal_handle.h>

#include "zros_master.h"
#include <sspdlog/sspdlog.h>

int main(int argc, char* argv[]) {
    try {
        zros::BaseServiceProcess base;
        auto address = argc > 1 ? argv[1] : "0.0.0.0:23333";
        zros::MasterServiceImpl master(address);
        master.runServer();
    }
    catch (const std::exception &e) {
        SSPD_LOG_ERROR << "catch fatal exception: " << e.what();
        throw;
    }
    return 0;
}

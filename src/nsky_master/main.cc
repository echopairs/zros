#include <iostream>
#include <nsky/common/signal_handle.h>
//#include <nsky/common/slog.h>

#include "nsky_master.h"

using namespace nsky;
int main(int argc, char* argv[])
{
    try
    {
        //slog::InitLog("nsky_master");
        BaseServiceProcess base;
        auto address = argc > 1 ? argv[1] : "0.0.0.0:23333";
        //nsky::MasterServiceImpl master(address);
        //master.RunServer();
    }
    catch (const std::exception &e)
    {
      //  NROS_LOG_ERROR << "Catch fatal exception: " << e.what();
        throw;
    }
    return 0;
}

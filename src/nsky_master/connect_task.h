/*********************************************************************
  Copyright (C), 1990-2016, HangZhou RED HEAT Tech. Co., Ltd.
  FileName: connect_task.h
  Author  : pairs & 736418319@qq.com
  Version : 1.0
  Date    : 2016/11/21
  Description:
      help build  or disconnect connection with two node
  Function List:
  History:
      <author>    <time>    <version>    <desc>
        pairs     16/11/21      1.0     build this moudle
*********************************************************************/
#ifdef  __CONNECT_TASK__
#define __CONNECT_TASK__

#include "nsky_rpc.grpc.pb.h"
#include <mutex>
#include <condition_variable>
#include <thread>

namespace nsky
{
// may be take this as interface
class CTask
{
public:
  CTask()
private:
  enum taskType
  {
    connect;
    disconnect;
  }
  taskType type_;
  std::shared_ptr<nsky_rpc::ServerInfo>sinfo_;
  std::shared_ptr<nsky_rpc::ClientInfo>cinfo_;
  std::shared_ptr<NodeManger>node_manger_;
  std::shared_ptr<ServiceManager>service_manager_;
};

class TaskManager
{
public:
  TaskManager(size_t nums);
  ~TaskManager();
private:
  std::atomic<bool> stop_;
  std::condition_variable cv_;
  std::mutex mtx_;
  std::vector<std::thread>wks_;
};

}  // end of namespace nsky
#endif

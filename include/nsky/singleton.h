/*********************************************************************
  Copyright (C), 1990-2016, HangZhou RED HEAT Tech. Co., Ltd.
  FileName: singleton.h
  Author  : pairs & 736418319@qq.com
  Version : 1.0
  Date    : 3/11/16
  Description:
      this is the template of singleton
      e.g:
      class test{xxx};
      std::shared_ptr<test> a = SingleTon<SingleTest>::instance(1,2);
      a.someMeth(xxx);
  Function List:
  History:
      <author>    <time>    <version>    <desc>
        pairs     16/11/03      1.0     build this moudle
*********************************************************************/
#ifndef __SIGLETON_H__
#define __SIGLETON_H__

#include <memory>
#include <mutex>
template <typename T>
class SingleTon
{
public:
  template<typename... Args>
  static std::shared_ptr<T> Instance(Args&&... args)
  {
    if (pInstance_ == nullptr)
    {
      std::lock_guard<std::mutex> lock(mtx_);
      if (pInstance_ == nullptr)
      {
	  std::shared_ptr<T> tmp(new T(std::forward<Args>(args)...));
          pInstance_ = std::move(tmp);
      }
    }
    return pInstance_;
  }

private:
  static std::mutex mtx_;
  SingleTon();
  virtual ~SingleTon();
  static std::shared_ptr<T> pInstance_;
};
template <typename T> std::shared_ptr<T> SingleTon<T>::pInstance_ = nullptr;
template <typename T> std::mutex SingleTon<T>::mtx_;
#endif

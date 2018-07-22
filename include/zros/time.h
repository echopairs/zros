#ifndef UTILS_TIME_H
#define UTILS_TIME_H 

#include <chrono>
#include <iostream>
#include <thread>

namespace utils {
using namespace std;
using namespace std::chrono;

class Timer
{
public:
	Timer():_begin(high_resolution_clock::now()){}
	void Reset() { _begin = high_resolution_clock::now();}

	/// default milli
	int64_t Elapsed() const
	{
	     return duration_cast<chrono::milliseconds>(high_resolution_clock::now() - _begin).count();
	}

	/// micro 
	int64_t ElapsedMicro() const 
	{
		 return duration_cast<chrono::microseconds>(high_resolution_clock::now() - _begin).count();
	}

	/// nano
	int64_t ElapsedNano() const 
	{
		 return duration_cast<chrono::nanoseconds>(high_resolution_clock::now() - _begin).count();
	}

	/// seconds
	int64_t ElapsedSeconds() const 
	{
		return duration_cast<chrono::seconds>(high_resolution_clock::now() - _begin).count();
	}

	/// min
	int64_t ElapsedMinutes() const
	{
		 return duration_cast<chrono::minutes>(high_resolution_clock::now() - _begin).count();
	}

	/// hour
	int64_t ElapsedHours() const 
	{
		return duration_cast<chrono::hours>(high_resolution_clock::now() - _begin).count();
	}
private:
	time_point<high_resolution_clock> _begin;
};
}
#endif

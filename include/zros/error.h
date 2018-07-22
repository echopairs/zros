//
// Created by pairs on 7/11/18.
//

#pragma once
#include <stdexcept>
using namespace std;
namespace zros
{
    class initialize_again : public logic_error
    {
    public:
        explicit initialize_again(const string & message) : logic_error(message){}
        explicit initialize_again(const char * message)	: logic_error(message){}
    };
    class zros_not_initialized : public logic_error
    {
    public:
        explicit zros_not_initialized(const string & message) : logic_error(message){}
        explicit zros_not_initialized(const char * message)	: logic_error(message){}
    };
    class initialize_error : public logic_error
    {
    public:
        explicit initialize_error(const string & message) : logic_error(message){}
        explicit initialize_error(const char * message) : logic_error(message){}
    };
    class invalid_operation : public logic_error
    {
    public:
        explicit invalid_operation(const string & message) : logic_error(message){}
        explicit invalid_operation(const char * message) : logic_error(message){}
    };
}